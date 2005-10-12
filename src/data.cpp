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

#include "sprite_data.hpp"
#include "data.hpp"

//                                  w, h, align, fps, frames, frames...
uint16_t   tux_sprite_rawdata[] = { 2, 2, 16,32,  6,     6,   0,0,  4,0,  8,0,  12,0,  0,4,  4,4  };
SpriteData tux_sprite_data(tux_sprite_rawdata);

/* EOF */
