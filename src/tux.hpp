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

#ifndef HEADER_TUX_HPP
#define HEADER_TUX_HPP

class Sprite;

/** */
class Tux
{
private:
  Sprite* sprite;

  int x_pos;
  int y_pos;

  int16_t scroll_x;
  int16_t scroll_y;

public:
  Tux();
  ~Tux();
  
  void update();

private:
  Tux (const Tux&);
  Tux& operator= (const Tux&);
};

#endif

/* EOF */
