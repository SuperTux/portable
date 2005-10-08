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

#include <math.h>
#include "math.hpp"

Math math;

Math::Math()
{
  for(uint16_t i = 0; i < 256; ++i)
    sin_table[i] = (uint8_t)(::sin(i/8.0f) * 20);
}

Math::~Math()
{
}

uint8_t
Math::sin(uint8_t angle)
{
  return sin_table[angle];
}

uint8_t
Math::cos(uint8_t angle)
{
  return sin_table[angle];
}

/* EOF */
