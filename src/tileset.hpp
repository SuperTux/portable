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

#ifndef HEADER_TILESET_HPP
#define HEADER_TILESET_HPP

#include "types.hpp"

/** Handles a tileset in ROM */
class Tileset
{
private:
  const uint16_t* data;
  const uint8_t*  colmap;
public:
  Tileset(const void* data, const void* colmap);
  ~Tileset();

  /** Returns a pointer to the start of tile \a id */
  const uint16_t* get_tile(uint16_t id) const;

  uint8_t get_colmap(uint16_t id) const;
};

#endif

/* EOF */
