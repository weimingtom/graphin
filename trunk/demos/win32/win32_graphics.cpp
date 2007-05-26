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

  graphics_state_save(g);

  graphics_scale(g,0.5,0.5);
  graphics_translate(g,0,0);
    
  // first quadrant, basic drawing functions
    graphics_line(g, 0,0, width,height);

      graphics_line_color(g,graphics_rgbt(255,0,0,0));
    graphics_triangle(g, width/2, 10.5, 10.5, height - 10.5 , width - 10.5, height - 10.5);
      graphics_line_color(g,graphics_rgbt(0,255,0,0));
    graphics_rectangle(g, 20.5, 20.5, width - 20.5, height - 20.5);
      graphics_line_color(g,graphics_rgbt(0,0,255,0));
    graphics_rounded_rectangle(g, 30.5, 30.5, width - 30.5, height - 30.5, 10, 10);

      graphics_line_color(g,graphics_rgbt(0,127,127,0));
    graphics_ellipse(g, width/2, height/2, width/4, height/4);

    int r = min(width, height);

      graphics_line_color(g,graphics_rgbt(127,0,127,0));
    graphics_arc(g, width/2, height/2, r/8, r/8, 0,6);

      graphics_line_color(g,graphics_rgbt(127,127,0,0));
    graphics_star(g, width/2, height/2, r/5, r/8, 0,5);

  graphics_state_restore(g);

  // second quadrant, path operations

  graphics_state_save(g);

    graphics_translate(g,width/2,0);

      graphics_line_width(g,2);
      graphics_line_color(g, graphics_rgbt(0xAF,0x7F,0x2F)); // brown

      graphics_fill_linear_gradient(g, 30, 30, (150 + 70) / 2, 150, 
          graphics_rgbt(0xFF,0,0), // red
          graphics_rgbt(0xFF,0xFF,0), // yellow
          0.5); 

      graphics_open_path(g); // stealed from Mozilla <canvas> tests
      graphics_move_to(g, 30, 30, false);
      graphics_line_to(g, 150, 150, false);
      graphics_bezier_curve_to(g, 60, 70, 60, 70, 70, 150, false);
      graphics_line_to(g,30, 30, false);
      graphics_close_path(g);
      graphics_draw_path(g, FILL_AND_STROKE);

      graphics_fill_radial_gradient(g, 250, 80, 50, 
          graphics_rgbt(0xFF,0,0), // red
          graphics_rgbt(0xFF,0xFF,0)); // yellow

      graphics_rounded_rectangle(g, 200, 30, 200 + 100, 30 + 100, 10, 10);

  graphics_state_restore(g);

  image_blit(hdc,0,0,_img,0,0,width,height, false);  
  graphics_release(g);
}


