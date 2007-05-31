#include "win32.h"
#include "mm_file.h"
#include <direct.h>

#include "window.h"



void graphic_paint(WINDOW_ON_PAINT_PARAMS* p)
{
  HGFX g = 0;
  
  unsigned int width;
  unsigned int height;

  image_get_info(p->surface,0,&width,&height,0,0);

  graphics_create( p->surface, &g );

  // next line is not working in the way I expect.
  //graphics_set_clip_box ( g, p->clip_x, p->clip_y, p->clip_x + p->clip_w, p->clip_y + p->clip_h);
  //

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


  // Third quadrant, text output

  graphics_state_save(g);

    graphics_translate(g,0,height/2);
    graphics_scale(g,0.5,0.5);

      graphics_line(g,0,height/2, width, height/2);

      graphics_line_width(g,3);
      graphics_line_color(g, graphics_rgbt(0xAF,0x7F,0x2F)); // brown

      graphics_fill_radial_gradient(g, width/2, height/2, r / 2, 
          graphics_rgbt(0xFF,0xFF,0), // yellow
          graphics_rgbt(0,0x8F,0)); // green


      graphics_text_alignment(g, ALIGN_CENTER, ALIGN_BASELINE);
      graphics_font(g, "Century Gothic", r / 5);

      graphics_text(g, width/2, height/2, L"graphinius", 10);
     

  graphics_state_restore(g);


  // Fourth quadrant, image output

  graphics_state_save(g);

    graphics_translate(g,width/2,height/2);
    graphics_scale(g,0.5,0.5);

    static HIMG img = 0;

    if(!img)
    {
      tool::mm_file mf;
      if( mf.open("../petrov_vodkin_A_Candle_and_a_Decanter.jpg") )
        image_load( (BYTE*) mf.data(), mf.size(), &img ); // load png/jpeg/etc. image from stream of bytes
    }

    if(img)
    {
      DIM w = width - 20;
      DIM h = height - 20;
      graphics_draw_image ( g, img, 10, 10, &w, &h );
    }

  graphics_state_restore(g);
  graphics_release(g);
}


