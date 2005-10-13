/*  $Id$
**   _____                 _____          
**  |   __|_ _ ___ ___ ___|_   _|_ _ _ _
**  |__   | | | . | -_|  _| | | | | |_'_| 
**  |_____|___|  _|___|_|   |_| |___|_,_| 
**            |_|                Portable
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

#ifndef HEADER_SPRITE_MANAGER_HPP
#define HEADER_SPRITE_MANAGER_HPP

#include "types.hpp"

/** Class for handling sprite allocation, at the moment unused, all
    code is in SpriteRenderer and will be refactored into this one
    later on
 */
class SpriteManager
{
private:
public:
  SpriteManager();
  ~SpriteManager();

private:
  SpriteManager (const SpriteManager&);
  SpriteManager& operator= (const SpriteManager&);
};

#endif

/* EOF */
