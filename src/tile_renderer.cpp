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
#include <stdlib.h>
#include "gba_video.h"
#include "tileset.hpp"
#include "tile_map.hpp"
#include "tile_renderer.hpp"
#include "console.hpp"
#include "math.hpp"

#include "../build/font_img_raw.h"

TileRenderer::TileRenderer()
  : freelist(refcount),
    tileset(0)
{
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
  next_free = 1;

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
  if (rom_id == 0)
    {
      return 0;
    }
  else if (rom2vram[rom_id] == NO_TILE)
    {
      if (next_free == 0)
        {
          console << "Out of Tilespace for: " << rom_id << "\n";
          return 0;
        }
      else 
        {    
          uint16_t vram_id = next_free;

          rom2vram[rom_id]  = vram_id;
          vram2rom[vram_id] = rom_id;
     
          next_free = freelist[vram_id];

          refcount[vram_id] = 1;

          upload_tile(rom_id, vram_id);

          return rom2vram[rom_id];
        }
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
  layers[layer_num].tilemap = tilemap_;
}

void
TileRenderer::done()
{
  for(uint8_t layer = 0; layer < 4; layer += 1)
    {
      if (layers[layer].tilemap)
        {
          copy_tilemap(layer);
        }
    }        

  if (0)
    {
      // places the set tilemaps into VRAM
      for(uint8_t layer = 0; layer < 4; layer += 1)
        {
          if (layers[layer].tilemap)
            {
              TileMap*  rom = layers[layer].tilemap;
              uint16_t* ram = (uint16_t*)MAP_BASE_ADR(28 + layer);

              for(uint8_t y = 0; y < 32; ++y)
                for(uint8_t x = 0; x < 32; ++x)
                  {
                    ram[y * 32 + x] = create_vram_tile(rom->get_data()[y * rom->get_width() + x]);
                  }
            }
        }
    }
}

void
TileRenderer::set_tilemap_offset(uint8_t layer_num, uint16_t x_offset, uint16_t y_offset)
{
  layers[layer_num].new_log_x = Math::mid(0, (int)x_offset, layers[layer_num].tilemap->get_width()  - 32);
  layers[layer_num].new_log_y = Math::mid(0, (int)y_offset, layers[layer_num].tilemap->get_height() - 32);

  prepare_layer(layer_num);
}

void
TileRenderer::copy_tilemap(uint8_t layer_num)
{
  uint16_t* ram = (uint16_t*)MAP_BASE_ADR(28 + layer_num);  
  Layer& layer = layers[layer_num];
  for(uint8_t x = 0; x < 32; ++x)
    for(uint8_t y = 0; y < 32; ++y)
      {
        delete_vram_tile(ram[y * 32 + x]);
        ram[y * 32 + x]= create_vram_tile(layer.tilemap->get_data()[y * layer.tilemap->get_width() + x]);
      }
}

void
TileRenderer::prepare_layer(uint8_t layer_num)
{
  Layer& layer = layers[layer_num];
  if (layer.log_x != layer.new_log_x || layer.log_y != layer.new_log_y)
    { // Layer has moved so we need to refill the tilemap
      int16_t diff_x = (layer.new_log_x/8) - (layer.log_x/8);
      if (diff_x == 0)
        { // We are still in the same tile segment, nothing to change
          layer.log_x = layer.new_log_x;
        }
      else if (abs(diff_x) >= 32)
        { // fullscreen refresh
          copy_tilemap(layer_num);
        }
      else if (diff_x > 0)
        {
          for(int8_t x = (layer.log_x/8)%32; x < (layer.log_x/8)%32; ++x)
            {
              for(uint8_t y = 0; y < 32; ++y)
                {
                  uint16_t* ram = (uint16_t*)MAP_BASE_ADR(28 + layer_num);
                  delete_vram_tile(ram[y * 32 + x]);
                  ram[y * 32 + x] = create_vram_tile(layer.tilemap->get_data()[y * layer.tilemap->get_width() + x]);
                }
            }
        }
      else if (diff_x < 0)
        {
        }
    }
}

void
TileRenderer::set_palette(const void* pal)
{
  for(uint16_t i = 0; i < 256; ++i)
    BG_COLORS[i] = ((u16*)pal)[i];
}

/* EOF */
