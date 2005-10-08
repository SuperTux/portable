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

  BGCTRL[0] = BG_PRIORITY(0) | CHAR_BASE(0) | BG_MOSAIC | BG_256_COLOR | SCREEN_BASE(31) | BG_SIZE_0;

  // console layer
  BGCTRL[3] = BG_PRIORITY(0) | CHAR_BASE(0) | BG_MOSAIC | BG_256_COLOR | SCREEN_BASE(28) | BG_SIZE_0;

  // mosaic works: *((u16*)0x400004C) = (3 << 0) | (3 << 4);
  uint16_t tilemap_data[32*32];
  for(uint16_t i = 0; i < 32*32; ++i)
    tilemap_data[i] = i;

  Tileset tileset(tileset_img_raw);
  TileMap tilemap(32, 32, tilemap_data);
  TileRenderer tile_renderer;
  
  tile_renderer.set_tileset(&tileset);
  tile_renderer.set_tilemap(0, &tilemap);
  tile_renderer.set_palette(tileset_pal_raw);
  tile_renderer.done();
  
  console.print("Hello World1234567891234567891234");
  console.putchar(':');
  console.print(1024);
  console.print(10245);
  console.print("\nHello World");

  bg_scroll scroll, scroll2;
  scroll.x  = 0;
  scroll.y  = 0;
  scroll2.x = 0;
  scroll2.y = 0;
  while (1)
    {
      u16 keys = KeysHeld();
      if (keys & KEY_UP)
        {
          scroll.y  += 1;
          scroll2.y += 2;
        }
      else if (keys & KEY_DOWN)
        {
          scroll.y -= 1;
          scroll2.y -= 2;
        }
      
      if (keys & KEY_LEFT)
        {
          scroll.x += 1;
          scroll2.x += 2;
        }
      else if (keys & KEY_RIGHT)
        {
          scroll.x -= 1;
          scroll2.x -= 2;
        }

      //BG_OFFSET[3] = scroll2;
      BG_OFFSET[3] = scroll;
          
      console.print(scroll.x);
      console.putchar('x');
      console.print(scroll.y);
      console.putchar('\n');

      VBlankIntrWait();
    }
}

/* EOF */
