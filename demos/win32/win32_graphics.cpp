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

  //.... demo drawing here please!

  image_blit(hdc,0,0,_img,0,0,width,height, false);  
  graphics_release(g);
}


