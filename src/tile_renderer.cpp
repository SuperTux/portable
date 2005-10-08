/*  $Id$
**
**  SuperTux Portable
**  Copyright (C) 2005 Ingo Ruhnke <grumbel@gmx.de>
**
**  This program is free software; you can redistribute it and/or
**  modify it under the terms of the GNU General Public License
**  as published by the Free Software Foundation; either version 2
**  of the License, or (at your option) any later version.
**
**  This program is distributed in the hope that it will be useful,
**  but WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**  GNU General Public License for more details.
** 
**  You should have received a copy of the GNU General Public License
**  along with this program; if not, write to the Free Software
**  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
**  02111-1307, USA.
*/

#include <assert.h>
#include <string.h>
#include "gba_video.h"
#include "tileset.hpp"
#include "tile_map.hpp"
#include "tile_renderer.hpp"

#include "../build/font_img_raw.h"

TileRenderer::TileRenderer()
  : freelist(refcount),
    tileset(0)
{
  tilemap[0] = tilemap[1] = tilemap[2] = tilemap[3] = 0;

  for(uint16_t i = 0; i < MAX_TILE_ID; ++i)
    {
      rom2vram[i] = NO_TILE;
    }

  for(uint16_t i = 0; i < MAX_VRAM_ID; ++i)
    {
      freelist[i] = i + 1;
      vram2rom[i] = NO_TILE;
    }

  freelist[MAX_VRAM_ID-1] = 0;
  next_free = 0;

  // Upload letter tileset
  for(uint8_t rom_id = 0; rom_id < 95; ++rom_id)
    for(uint8_t y = 0; y < 8; ++y)
      for(uint8_t x = 0; x < 4; ++x)
        ((u16*)CHAR_BASE_ADR(0))[((MAX_VRAM_ID + rom_id) * (8*4) + y*4 + x)] =
          ((uint16_t*)font_img_raw)[(y + ((rom_id*4)/380)*8) * 380 + x + ((rom_id*4)%380)];
}

TileRenderer::~TileRenderer()
{
  
}

uint16_t
TileRenderer::create_vram_tile(uint16_t rom_id)
{
  if (rom2vram[rom_id] == NO_TILE)
    {
      uint16_t vram_id = next_free;

      rom2vram[rom_id]  = vram_id;
      vram2rom[vram_id] = rom_id;
     
      next_free = freelist[vram_id];
      refcount[vram_id] = 1;

      upload_tile(rom_id, vram_id);

      return rom2vram[rom_id];
    }
  else
    {
      refcount[rom_id] += 1;
      return rom2vram[rom_id];
    }
}

void
TileRenderer::upload_tile(uint16_t rom_id, uint16_t vram_id)
{
  // FIXME: Tileset should come in a different format so that we can
  // just use memcpy and don't have to blit
  for(uint8_t y = 0; y < 8; ++y)
    for(uint8_t x = 0; x < 4; ++x)
      ((u16*)CHAR_BASE_ADR(0))[(vram_id * (8*4) + y*4 + x)] =
        tileset->data[(y + ((rom_id*4)/128)*8) * 128 + x + ((rom_id*4)%128)];
}

void
TileRenderer::delete_vram_tile(uint16_t vram_id)
{
  if (refcount[vram_id] <= 1)
    {
      refcount[vram_id] = next_free;
      next_free = vram_id;
      rom2vram[vram2rom[vram_id]] = NO_TILE;
      vram2rom[vram_id] = NO_TILE;
    }
  else
    {
      refcount[vram_id] -= 1;
    }
}

void
TileRenderer::set_tileset(Tileset* tileset_)
{
  tileset = tileset_;
}

void
TileRenderer::set_tilemap(uint8_t layer_num, TileMap* tilemap_)
{
  tilemap[layer_num] = tilemap_;
}

void
TileRenderer::done()
{
  // places the set tilemaps into VRAM
  for(uint8_t layer = 0; layer < 4; layer += 1)
    {
      if (tilemap[layer])
        {
          TileMap*  rom = tilemap[layer];
          uint16_t* ram = (uint16_t*)MAP_BASE_ADR(28 + layer);

          for(uint8_t y = 0; y < 20; ++y)
            for(uint8_t x = 0; x < 32; ++x)
              {
                uint16_t tile = rom->get_data()[y * rom->get_width() + x];
                ram[y * 32 + x] = create_vram_tile(tile);
              }
        }
    }
}

void
TileRenderer::set_tilemap_offset(uint8_t layer_num, uint16_t x_offset, uint16_t y_offset)
{
  
}

void
TileRenderer::set_palette(const void* pal)
{
  for(uint16_t i = 0; i < 256; ++i)
    BG_COLORS[i] = ((u16*)pal)[i];
}

/* EOF */
