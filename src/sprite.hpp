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

#ifndef HEADER_SPRITE_HPP
#define HEADER_SPRITE_HPP

#include "sprite_data.hpp"
#include "types.hpp"

/** */
class Sprite
{
private:
  bool enabled;
  SpriteData data;
  int x;
  int y;
  bool world_co;
  int frame;
  bool hflip;
  bool vflip;
public:
  Sprite();
  Sprite(const SpriteData& data_);

  void set_pos(int x, int y);
  void set_x(int x_) { x = x_; }
  void set_y(int y_) { y = y_; }
  int  get_x() const { return x; }
  int  get_y() const { return y; }

  bool is_enabled() const { return enabled; }

  const SpriteData& get_data() const { return data; }

  /** If true use world coordinates, else use screen coordinates */
  bool use_world_co() const { return world_co; }

  int get_frame() const { return frame; }

  void set_hflip(bool t) { hflip = t; }
  void set_vflip(bool t) { vflip = t; }

  bool get_hflip() const { return hflip; }
  bool get_vflip() const { return vflip; }

  void update();
};

#endif

/* EOF */
