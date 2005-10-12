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

#ifndef HEADER_TILE_MAP_HPP
#define HEADER_TILE_MAP_HPP

#include "types.hpp"

class Tileset;

/** */
class TileMap
{
private:
  Tileset* tileset;

  /** Width of the TileMap */
  uint16_t  width;

  /** Height of the TileMap */
  uint16_t  height;

  /** Pointer to the tilemap in ROM */
  uint16_t* tilemap;

public:
  TileMap(Tileset* tileset, uint16_t* raw_data);
  ~TileMap();

  inline uint16_t* get_data()   const { return tilemap; }
  inline uint16_t  get_width()  const { return width; }
  inline uint16_t  get_height() const { return height; }

  uint8_t get_colmap(int x, int y) const;

private:
  TileMap (const TileMap&);
  TileMap& operator= (const TileMap&);
};

#endif

/* EOF */
