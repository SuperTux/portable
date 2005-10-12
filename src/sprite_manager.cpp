/*  $Id$
**   __      __ __             ___        __   __ __   __
**  /  \    /  \__| ____    __| _/_______/  |_|__|  | |  |   ____
**  \   \/\/   /  |/    \  / __ |/  ___/\   __\  |  | |  | _/ __ \
**   \        /|  |   |  \/ /_/ |\___ \  |  | |  |  |_|  |_\  ___/
**    \__/\  / |__|___|  /\____ /____  > |__| |__|____/____/\___  >
**         \/          \/      \/    \/                         \/
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

#include "gba_sprites.h"
#include "gba_video.h"
#include "sprite_manager.hpp"

SpriteManager::SpriteManager()
{
}

SpriteManager::~SpriteManager()
{
}

void
SpriteManager::upload(uint16_t* data)
{
  for(int i = 0; i < 32 * 512; ++i)
    {
      ((uint16_t*)OBJ_BASE_ADR)[i] = data[i];
    }

  OAM[0].attr0 = OBJ_Y(80)  | OBJ_SHAPE(0) | OBJ_256_COLOR;
  OAM[0].attr1 = OBJ_X(120) | OBJ_SIZE(2);
  OAM[0].attr2 = OBJ_CHAR(8) | OBJ_PALETTE(0) | OBJ_PRIORITY(0);
}

/* EOF */
