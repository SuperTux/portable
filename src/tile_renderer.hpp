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

class Tileset;
class TileMap;

/** Management class for uploading tiles from ROM to VRAM and handling the remapping */
class TileRenderer
{
private:
  static const uint16_t NO_TILE     = 0xFFFF;
  static const uint16_t MAX_TILE_ID = 1024;
  static const uint16_t MAX_VRAM_ID = 896 - 96; // we use 96 tiles for letters

  uint16_t rom2vram[MAX_TILE_ID];
  uint16_t vram2rom[MAX_VRAM_ID];

  /** This array holds the refcount for VRAM tiles as well as the list
      of freelist for tile entries in VRAM, since a tile entry is
      either used or free, which can be checked via vram2rom those two
      don't conflict */
  uint16_t  refcount[MAX_VRAM_ID];

  /** pointer to refcount, see refcount */
  uint16_t* freelist;
 
  uint16_t  next_free;

  Tileset* tileset;

  struct Layer {
    TileMap* tilemap;
    uint16_t log_x;
    uint16_t log_y;
    uint16_t new_log_x;
    uint16_t new_log_y;
    uint8_t x_offset;
    uint8_t y_offset;

    Layer() 
      : tilemap(0), 
        log_x(0), log_y(0),
        new_log_x(0), new_log_y(0),
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

  /** Set the offset for the given layer */
  void set_tilemap_offset(uint8_t layer_num, uint16_t x_offset, uint16_t y_offset);

  void set_palette(const void* pal);

  void prepare_layer(uint8_t layer_num);

  void copy_tilemap(uint8_t layer_num);

  void done();

private:
  /** Allocate a tile in VRAM and copy the ROM tile over to VRAM */
  uint16_t create_vram_tile(uint16_t rom_id);
  
  /** Decrements the refcount and deletes \a tile from VRAM if it is
      no longer used */
  void delete_vram_tile(uint16_t vram_id);

  /** Uploads a tile at location \a rom_id to location \a vram_id */
  void upload_tile(uint16_t rom_id, uint16_t vram_id);
};

#endif

/* EOF */
