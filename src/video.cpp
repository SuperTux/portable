#include "gba_types.h"
#include "gba_video.h"
#include "video.hpp"

Video video;

Video::Video()
  : layer0(0), layer1(1), layer2(2), layer3(3)
{
}

Video::~Video()
{
}
  
void
Video::set_palette(const void* pal)
{
  u16 i;
  for(i = 0; i < 256; ++i)
    BG_COLORS[i] = ((u16*)pal)[i];
}

void
Video::set_tileset(u8 num, uint16_t* img, uint16_t count)
{
  u16 i;
  for(i = 0; i < count; ++i)
    {
      u8 x, y;
      for(y = 0; y < 8; ++y)
        for(x = 0; x < 4; ++x)
          ((u16*)CHAR_BASE_ADR(num))[(i * (8*4) + y*4 + x)] =
            img[(y + ((i*4)/128)*8) * 128 + x + ((i*4)%128)];
    }
}

Layer*
Video::get_layer(uint8_t num)
{
  switch(num)
    {
    case 0: 
      return &layer0;
      
    case 1:
      return &layer1;

    case 2:
      return &layer2;

    case 3:
      return &layer3;
      
    default:
      return &layer0;
    }
}

/* EOF */
