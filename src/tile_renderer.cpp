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
  Layer& layer = layers[layer_num];

  // Cut new offset to the limits of the layer
  // FIXME: Might be useful to make limits configurable per layer
  x_offset = Math::mid(0, (int)x_offset, layer.tilemap->get_width() *8 - 32*8);
  y_offset = Math::mid(0, (int)y_offset, layer.tilemap->get_height()*8 - 20*8);

  //copy_tilemap(layer_num);

  int x_tile = layer.x_offset/8;
  int y_tile = layer.y_offset/8;

  int new_x_tile = x_offset/8;
  int new_y_tile = y_offset/8;

  if (x_tile != new_x_tile || 
      y_tile != new_y_tile)
    { 
      int start_x = Math::min(x_tile, new_x_tile);
      int start_y = Math::min(y_tile, new_y_tile);

      int end_x = Math::max(x_tile, new_x_tile);
      int end_y = Math::max(y_tile, new_y_tile);

      int left = start_x;
      int top  = start_y;

      if (new_x_tile > x_tile)
        {
          start_x += 32;
          end_x   += 32;
        }

      if (new_y_tile > y_tile)
        {
          start_y += 32;
          end_y   += 32;
        }

      // Version refresh
      for(int y = start_y; y < end_y && (y < layer.tilemap->get_height()); ++y)
        for(int x = left; x < left+32; ++x)
          {
            uint16_t& tile = layer.vram[(y%32) * 32 + (x%32)];

            tile_manager->delete_vram_tile(tile);
            tile = tile_manager->create_vram_tile(layer.tilemap->get_data()
                                                  [y * layer.tilemap->get_width() + x]);
          }      

      // Horizontal refresh
      for(int y = top; y < top+32 && (y < layer.tilemap->get_height()); ++y)
        for(int x = start_x; x < end_x; ++x)
          {
            uint16_t& tile = layer.vram[(y%32) * 32 + (x%32)];

            tile_manager->delete_vram_tile(tile);
            tile = tile_manager->create_vram_tile(layer.tilemap->get_data()
                                                  [y * layer.tilemap->get_width() + x]);
          }      

      // FIXME: The regions of vertical and horizontal refresh may
      // overlap, could be optimized further

      // FIXME: Some off-by-one bug seems to be hiding here, diagonal scrolling causes slight trouble
    }

  layer.x_offset = x_offset;
  layer.y_offset = y_offset;
  
  bg_scroll scroll;
  scroll.x = layer.x_offset;
  scroll.y = layer.y_offset;
  BG_OFFSET[layer_num] = scroll;
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
TileRenderer::set_palette(const void* pal)
{
  for(uint16_t i = 0; i < 256; ++i)
    BG_COLORS[i] = ((u16*)pal)[i];
}

/* EOF */
