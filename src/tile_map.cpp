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

#include "tileset.hpp"
#include "tile_map.hpp"

TileMap::TileMap(Tileset* tileset_, uint16_t* raw_data)
  : tileset(tileset_), 
    width(raw_data[0]), height(raw_data[1]), tilemap(raw_data + 2)
{
}

TileMap::~TileMap()
{
}

uint8_t
TileMap::get_colmap(int x, int y) const
{
  return tileset->get_colmap(tilemap[y*width + x]);
}

/* EOF */
