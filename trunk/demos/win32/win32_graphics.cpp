#include "win32.h"

static HIMG _img = 0;
static int  _width = 0;
static int  _height = 0;

HGFX gfx(int width, int height)
{
  HGFX _gfx = 0;

  if( !_img || _width != width || _height != height )
  {
    if( _img )
      image_release(_img);

    _width = width;
    _height = height;

    image_create( _width, _height, &_img);
  }

  if( _img)
    graphics_create( _img, &_gfx);

  return _gfx;
}

void graphic_paint(HDC hdc, int width, int height)
{
  HGFX g = gfx(width,height);

  graphics_line(g, 0,0, width,height);

    graphics_line_color(g,graphics_rgbt(255,0,0,0));
  graphics_triangle(g, width/2, 10.5, 10.5, height - 10.5 , width - 10.5, height - 10.5);
    graphics_line_color(g,graphics_rgbt(0,255,0,0));
  graphics_rectangle(g, 20.5, 20.5, width - 20.5, height - 20.5);
    graphics_line_color(g,graphics_rgbt(0,0,255,0));
  graphics_rounded_rectangle(g, 30.5, 30.5, width - 30.5, height - 30.5, 10, 10);

    graphics_line_color(g,graphics_rgbt(0,127,127,0));
  graphics_ellipse(g, width/2, height/2, width/4, height/4);
  
  image_blit(hdc,0,0,_img,0,0,width,height, false);  
  graphics_release(g);
}


