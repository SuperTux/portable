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

#ifndef HEADER_SPRITE_RENDERER_HPP
#define HEADER_SPRITE_RENDERER_HPP

#include "sprite.hpp"

/** */
class SpriteRenderer
{
private:
  Sprite sprites[128];
  
  int next_free;
  int x_offset;
  int y_offset;

public:
  SpriteRenderer();
  ~SpriteRenderer();

  void set_palette(const void* pal);
  void update();

  Sprite* create(const SpriteData& data);

  /** Uploads a raw image of size 128x256 to the VRAM */
  void upload(uint16_t* data);

  void set_offset(int x, int y);
private:
  SpriteRenderer (const SpriteRenderer&);
  SpriteRenderer& operator= (const SpriteRenderer&);
};

#endif

/* EOF */
