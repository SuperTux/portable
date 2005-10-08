#ifndef HEADER_VIDEO_HPP
#define HEADER_VIDEO_HPP

#include "types.hpp"
#include "sprite.hpp"
#include "layer.hpp"

class Video
{
private:
  Sprite sprites[128];

  Layer layer0;
  Layer layer1;
  Layer layer2;
  Layer layer3;

public:
  Video();
  ~Video();
  
  void set_palette(const void* pal);
  void set_tileset(u8 num, uint16_t* img, uint16_t count);
  void set_tilemap(const void* map);
  void set_sprite(const Sprite& sprite);

  Layer* get_layer(uint8_t num);
};

extern Video video;

#endif

/* EOF */
