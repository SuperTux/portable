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
#include "globals.hpp"
#include "math.hpp"

TileRenderer::TileRenderer()
{
  layers[0] = Layer(0);
  layers[1] = Layer(1);
  layers[2] = Layer(2);

  // This layer is reserved for the Console, see console.hpp for details
  layers[3] = Layer(3);
}

TileRenderer::~TileRenderer()
{
  
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
}

void
TileRenderer::set_tilemap_offset(uint8_t layer_num, int16_t x_offset, int16_t y_offset)
{
  layers[layer_num].new_x_offset = Math::mid(0, (int)x_offset, layers[layer_num].tilemap->get_width() *8 - 32*8);
  layers[layer_num].new_y_offset = Math::mid(0, (int)y_offset, layers[layer_num].tilemap->get_height()*8 - 20*8);

  layers[layer_num].x_offset = layers[layer_num].new_x_offset;
  layers[layer_num].y_offset = layers[layer_num].new_y_offset;

  bg_scroll scroll;
  scroll.x = layers[layer_num].new_x_offset;
  scroll.y = layers[layer_num].new_y_offset;
  BG_OFFSET[layer_num] = scroll;

  copy_tilemap(layer_num);
  // process_layer(layer_num);
}

void
TileRenderer::get_tilemap_offset(uint8_t layer_num, int16_t& x_offset, int16_t& y_offset)
{
  x_offset = layers[layer_num].x_offset;
  y_offset = layers[layer_num].y_offset;
}

void
TileRenderer::copy_tilemap(uint8_t layer_num)
{
  Layer& layer = layers[layer_num];
  /*
  for(uint8_t x = 0; x < 32; ++x)
    for(uint8_t y = 0; y < 32 && (layer.y_offset/8 + y < layer.tilemap->get_height()); ++y)
      {
        uint16_t& tile = layer.vram[y * 32 + (x)];
        tile_manager->delete_vram_tile(tile);
        tile = tile_manager->create_vram_tile(layer.tilemap->get_data()
                                              [((layer.y_offset/8) + y) * layer.tilemap->get_width() 
                                               + ((layer.x_offset/8) + x)]);
      }
  */

  uint16_t start_x = layer.x_offset/8;
  uint16_t start_y = layer.y_offset/8;
  for(uint8_t x = start_x; x < start_x + 32; ++x)
    for(uint8_t y = start_y; y < start_y + 32 && (y < layer.tilemap->get_height()); ++y)
      {
        uint16_t& tile = layer.vram[(y%32) * 32 + (x%32)];
        tile_manager->delete_vram_tile(tile);
        tile = tile_manager->create_vram_tile(layer.tilemap->get_data()
                                              [(y) * layer.tilemap->get_width() 
                                               + (x)]);
      }

}

void
TileRenderer::process_layer(uint8_t layer_num)
{
  Layer& layer = layers[layer_num];
  if (layer.x_offset != layer.new_x_offset || layer.y_offset != layer.new_y_offset)
    { // Layer has moved so we need to refill the tilemap
      int16_t diff_x = (layer.new_x_offset/8) - (layer.x_offset/8);
      if (diff_x == 0)
        { // We are still in the same tile segment, nothing to change
          layer.x_offset = layer.new_x_offset;
        }
      else if (abs(diff_x) >= 32)
        { // fullscreen refresh
          copy_tilemap(layer_num);
        }
      else if (diff_x > 0)
        {
          for(int8_t x = (layer.x_offset/8)%32; x < (layer.x_offset/8)%32; ++x)
            {
              for(uint8_t y = 0; y < 32; ++y)
                {
                  tile_manager->delete_vram_tile(layer.vram[y * 32 + x]);
                  layer.vram[y * 32 + x] = tile_manager->create_vram_tile(layer.tilemap->get_data()[y * layer.tilemap->get_width() + x]);
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
