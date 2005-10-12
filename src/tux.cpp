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

#include "gba_input.h"
#include "gba_video.h"
#include "globals.hpp"
#include "sprite_renderer.hpp"
#include "data.hpp"
#include "tile_renderer.hpp"
#include "tux.hpp"
#include "console.hpp"

Tux::Tux()
  : sprite(sprite_renderer->create(tux_sprite_data))

{
}

Tux::~Tux()
{
}
  
void
Tux::update()
{
  sprite->update();

  u16 keys = KeysHeld();
  if (keys & KEY_UP)
    {
      scroll_y -= 4;
      sprite->set_y(sprite->get_y() - 1);
    }
  else if (keys & KEY_DOWN)
    {
      scroll_y += 4;
      sprite->set_y(sprite->get_y() + 1);
    }
      
  if (keys & KEY_LEFT)
    {
      scroll_x -= 4;
      sprite->set_x(sprite->get_x() - 1);
      sprite->set_hflip(true);
    }
  else if (keys & KEY_RIGHT)
    {
      scroll_x += 4;
      sprite->set_x(sprite->get_x() + 1);
      sprite->set_hflip(false);
    }

  tile_renderer->set_tilemap_offset(1, scroll_x, scroll_y);
  tile_renderer->get_tilemap_offset(1, scroll_x, scroll_y);

  tile_renderer->set_tilemap_offset(2, scroll_x/2, scroll_y/2);
  tile_renderer->set_tilemap_offset(3, scroll_x/4, scroll_y/4);
}

/* EOF */
