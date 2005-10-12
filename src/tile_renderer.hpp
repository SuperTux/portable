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

#ifndef HEADER_TILE_RENDERER_HPP
#define HEADER_TILE_RENDERER_HPP

#include "types.hpp"
#include "tile_manager.hpp"

class Tileset;
class TileMap;

/** The TileRenderer handles the mapping from a abitary large Tilemap
    in ROM to the 32x32 large tilemap in VRAM */
class TileRenderer
{
private:
  struct Layer {
    uint16_t* vram;
    TileMap*  tilemap;
    uint16_t x_offset;
    uint16_t y_offset;

    Layer() {}

    Layer(uint8_t num)
      : vram((uint16_t*)MAP_BASE_ADR(28 + num)),
        tilemap(0), 
        x_offset(0), y_offset(0)
    {}
  };

  Layer layers[4];

public:
  TileRenderer();
  ~TileRenderer();

  /** Use tileset \a tileset, must be 1024 tiles long (if needed that
      size can be increased) */
  void set_tileset(Tileset* tileset);

  /** Use the tilemap \a map for layer \a layer num, use tilemap == 0
      to disable the layer */
  void set_tilemap(uint8_t layer_num, TileMap* tilemap);

  /** Set the offset for the given layer (this copies stuff to VRAM,
      so use only once per loop) */
  void set_tilemap_offset(uint8_t layer_num, int16_t x_offset, int16_t y_offset);

  void get_tilemap_offset(uint8_t layer_num, int16_t& x_offset, int16_t& y_offset);

  void set_palette(const void* pal);

  /** Copies the complete visible part of the tilemap over into VRAM */
  void copy_tilemap(uint8_t layer_num);

  void done();

private:
};

#endif

/* EOF */
