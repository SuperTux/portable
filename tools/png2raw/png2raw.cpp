//       _________ __                 __                               
//      /   _____//  |_____________ _/  |______     ____  __ __  ______
//      \_____  \\   __\_  __ \__  \\   __\__  \   / ___\|  |  \/  ___/
//      /        \|  |  |  | \// __ \|  |  / __ \_/ /_/  >  |  /\___ |
//     /_______  /|__|  |__|  (____  /__| (____  /\___  /|____//____  >
//             \/                  \/          \//_____/            \/ 
//  ______________________                           ______________________
//			  T H E   W A R   B E G I N S
//   Utility for Stratagus - A free fantasy real time strategy game engine
//
//  Copyright (C) 2002 Ingo Ruhnke <grumbel@gmx.de>
//
//  This program is free software; you can redistribute it and/or
//  modify it under the terms of the GNU General Public License
//  as published by the Free Software Foundation; either version 2
//  of the License, or (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

/* To compile this programm:

    % g++ -o png2stratagus  png2stratagus.cpp -lpng
 */

/* This programm can be used to fix the palette of a indexed png file
   to be suitable for Stratagus. It works like this:

   1) You create a RGBA image in Gimp

   2) You convert it to indexed with 227 colors
   
   [Generate Optimal Palette # Colors 227] (MAX_COLORS - 1)
   
   3) You run png2stratagus on the image

   4) The final images will be written to out.png in the current
      directory
 */

#include <string>
#include <fstream>
#include <vector>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <png.h>
#include <assert.h>

class Color
{
public:
  unsigned int red;
  unsigned int green;
  unsigned int blue;

  Color ()
    : red (0), green (255), blue (0)
  {
  }

  Color (int r, int g, int b) 
    : red (r), green (g), blue (b)
  {    
  }
};

class Image
{
private:
  int m_width;
  int m_height;
  int m_transcol;
  std::vector<unsigned char> m_image;
  std::vector<Color> m_palette;
  
public:
  /** Load an image from a given png source */
  Image (const std::string& filename) 
  {
    FILE* fp;
    png_structp png_ptr;
    png_infop info_ptr;
    png_uint_32 pwidth, pheight;
    int bit_depth, color_type, interlace_type, compression_type, filter_type;
    int row_bytes;

    if ((fp = fopen(filename.c_str (), "rb")) == NULL)
      {
	perror (filename.c_str ());
	exit (EXIT_FAILURE);
      }

    png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING,
				     NULL, NULL, NULL);
    info_ptr = png_create_info_struct(png_ptr);
    png_init_io(png_ptr, fp);
    png_read_info(png_ptr, info_ptr);
    png_get_IHDR(png_ptr, info_ptr, &pwidth, &pheight,
		 &bit_depth, &color_type, &interlace_type,
		 &compression_type, &filter_type);
    row_bytes = png_get_rowbytes(png_ptr, info_ptr);

    // Create the 'data' array
    png_bytep row_pointers[pheight];
    for (unsigned int i = 0; i < pheight; i++)
      row_pointers[i] = new png_byte[row_bytes];

    png_read_image(png_ptr, row_pointers);
    
    if (color_type != PNG_COLOR_TYPE_PALETTE)
	{
	  std::cout << "Unsupported color type" << std::endl;
	  exit (EXIT_FAILURE);
	}

    int num_colors;
    int num_trans = 0;
    png_colorp lpalette;
    png_bytep trans;
    png_color_16p trans_values;

    // Read some more data
    png_get_PLTE(png_ptr, info_ptr, &lpalette, &num_colors);
    png_get_tRNS(png_ptr, info_ptr, &trans, &num_trans, &trans_values);

    // not sure what trans_values stand for

    //for (int i = 0; i < num_trans; ++i)
    //std::cout <<  "transcolors: " << trans_values[i] << std::endl;

    if (num_trans > 1)
      {
	std::cout << "Multiple transcolors not supported" << std::endl;
	exit (EXIT_FAILURE);
      }
    else if (num_trans == 1)
      m_transcol = trans[0];
    else
      m_transcol = -1;

    for (int i = 0; i < num_trans; i++)
      std::cout << "transcolor: " << int(trans[i]) << std::endl;
    
    m_width = pwidth;
    m_height = pheight;

    m_image.resize (m_width * m_height);

    // Convert the png into our internal data structure
    for (int y = 0; y < m_height; y++)
      {
	for (int i = 0; i < row_bytes; i++)
	  {
	    m_image[i + (m_width * y)] = row_pointers[y][i];
	  }
      }

    assert (num_colors <= 256);

    m_palette.resize (256);
    for (int i = 0; i < num_colors; ++i)
      {
	m_palette[i].red = lpalette[i].red;
	m_palette[i].green = lpalette[i].green;
	m_palette[i].blue = lpalette[i].blue;
      }

    png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp)NULL);
    fclose (fp);
  }
  
  ~Image () 
  {
  }

  void write_palette16bit(const std::string& filename)
  {
#define RGB5(r,g,b)     ((r)|((g)<<5)|((b)<<10))

    std::ofstream out(filename.c_str());
    for(std::vector<Color>::iterator i = m_palette.begin(); i != m_palette.end(); ++i)
      {
        unsigned short c = RGB5(i->red/8, i->green/8, i->blue/8);
        out.write(reinterpret_cast<char*>(&c), sizeof(unsigned short));
      }
  }

  void write_palette(const std::string& filename)
  {
    std::ofstream out(filename.c_str());
    out.write(reinterpret_cast<char*>(&*m_palette.begin()), m_palette.size()); 
  }

  void write_image(const std::string& filename)
  {
    if (0)
      {
        // Simple dump of the image data
        std::ofstream out(filename.c_str());
        out.write(reinterpret_cast<char*>(&*m_image.begin()), m_image.size());
      }
    else
      {
        std::cout << "Writing tileset" << std::endl;
        std::ofstream out(filename.c_str());
        unsigned char* ptr = &*m_image.begin();
        int num_tiles = (get_width() * get_height()) / 64;
        int pitch = get_width();
        int tile_width = get_width()/8;
        for(int i = 0; i < num_tiles; ++i)
          {
            for(int y = 0; y < 8; ++y)
              {
                out.write(reinterpret_cast<char*>(ptr + 64*(i/tile_width) + 8*(i%tile_width) + y*pitch), 8);
              }
          }
      }
  }

  void set_color (int i, Color c) {
    m_palette[i] = c;
  }

  int get_transcolor () {
    return m_transcol;
  }
  
  int num_colors () {
    return m_palette.size ();
  }

  int get_width () {
    return m_width;
  }
  
  int get_height () {
    return m_height;
  }
};

int main (int argc, char* argv[])
{
  if (argc != 3)
    {
      printf ("Usage: %s INFILE OUTFILE\n", argv[0]);
      exit (EXIT_FAILURE);
    }
  else
    {
      Image image (argv[1]);
      std::cout << image.get_width() << "x" << image.get_height() << std::endl;
      std::cout << "writing " << std::string(argv[2]) + "_pal.raw" << std::endl;
      std::cout << "writing " << std::string(argv[2]) + "_img.raw" << std::endl;
      image.write_palette16bit(std::string(argv[2]) + "_pal.raw");
      image.write_image(std::string(argv[2]) + "_img.raw");
    }
}

// EOF //
