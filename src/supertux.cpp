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

#include "gba_video.h"
#include "gba_input.h"
#include "gba_interrupt.h"
#include "fade.h"
#include <math.h>

#include "video.hpp"
#include "math.hpp"

#include <stdlib.h>

// Data for use in the game
#include "../build/tileset_img_raw.h"
#include "../build/tileset_pal_raw.h"

#include "tileset.hpp"
#include "tile_map.hpp"
#include "console.hpp"
#include "tile_renderer.hpp"

u16 PaletteBuffer[256];

unsigned int frame;
u8 line_count; 

void VblankInterrupt()
{
  line_count = 0;
  frame += 1;
  ScanKeys();
}

int main(void)
{
  // Set up the interrupt handlers
  InitInterrupt();

  SetInterrupt( Int_Vblank, VblankInterrupt);

  // Enable Vblank Interrupt to allow VblankIntrWait
  EnableInterrupt(Int_Vblank);

  // Allow Interrupts
  REG_IME = 1;

  SetMode( MODE_0 | BG0_ON | BG3_ON );		// screen mode & background to display

  // console layer
  BGCTRL[0] = BG_PRIORITY(0) | CHAR_BASE(0) | BG_MOSAIC | BG_256_COLOR | SCREEN_BASE(31) | BG_SIZE_0;

  BGCTRL[3] = BG_PRIORITY(0) | CHAR_BASE(0) | BG_MOSAIC | BG_256_COLOR | SCREEN_BASE(28) | BG_SIZE_0;

  // mosaic works: *((u16*)0x400004C) = (3 << 0) | (3 << 4);
  uint16_t tilemap_data[32*32];
  for(uint16_t i = 0; i < 32*32; ++i)
    tilemap_data[i] = i;

  Tileset tileset(tileset_img_raw);
  TileMap tilemap(32, 32, tilemap_data);
  TileRenderer tile_renderer;
  
  tile_renderer.set_tileset(&tileset);
  tile_renderer.set_tilemap(3, &tilemap);
  tile_renderer.set_palette(tileset_pal_raw);
  tile_renderer.done();

  if (0)
    {
      console.print("SupeTux Portable Version 0.0.0\n");
      console.print("==============================\n");
      console.print("\nThis program is free software;"
                    "you can redistribute it and/or"
                    "modify it under the terms of\n"
                    "the GNU General Public License"
                    "as published by the Free\n"
                    "Software Foundation; either\n"
                    "version 2 of the License, or\n"
                    "(at your option) any later\n"
                    "version. Version 0.0.0\n");
    }

  bg_scroll scroll;
  scroll.x  = 0;
  scroll.y  = 0;

  while (1)
    {
      u16 keys = KeysHeld();
      if (keys & KEY_UP)
        {
          scroll.y  += 1;
        }
      else if (keys & KEY_DOWN)
        {
          scroll.y -= 1;
        }
      
      if (keys & KEY_LEFT)
        {
          scroll.x += 1;
        }
      else if (keys & KEY_RIGHT)
        {
          scroll.x -= 1;
        }

      console.moveto(0,0);
      console << scroll.x << "x" << scroll.y << "   \n";

      tile_renderer.set_tilemap_offset(3, scroll.x, scroll.y);

      VBlankIntrWait();
    }
}

/* EOF */
