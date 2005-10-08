/*  $Id$
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

#ifndef HEADER_LAYER_HPP
#define HEADER_LAYER_HPP

#include "types.hpp"

class Tileset;
class TileMap;

/** */
class Layer
{
private:
  /** Which hardware layer are we representing? */
  uint8_t  number;
  Tileset* tileset;
  TileMap* tilemap;

public:
  Layer(uint8_t number_);
  ~Layer();

  bool is_enabled();
  void enable(bool t);
  
  void set_tileset(Tileset* tileset);
  void set_tilemap(TileMap* tilemap);

  void set_mosaic(bool t);
  
  //void set_blend();
};

#endif

/* EOF */
