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

#ifndef HEADER_CONSOLE_HPP
#define HEADER_CONSOLE_HPP

/** */
class Console
{
private:
  uint16_t* screen;
  uint8_t x_pos;
  uint8_t y_pos;
  uint8_t width;
  uint8_t height;

public:
  Console();
  ~Console();
  
  void print(const char* buf);
  void print(uint16_t num);

  void putchar(char c);
  void write(const char* buf, uint16_t len);
  
  void moveto(uint8_t x, uint8_t y);
  void clear();
  
  void scroll();

  inline uint16_t char2tile(char c) { return 800 - 32 + c; }
private:
  Console (const Console&);
  Console& operator= (const Console&);
};

extern Console console;

#endif

/* EOF */
