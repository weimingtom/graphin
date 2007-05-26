// graphin.cpp : Defines the entry point for the DLL application.
//

#include "primitives.h"
#include "objects.h"
#include "graphin.h"

#undef GRAPHIN_API
#define GRAPHIN_API

// this is an example of an exported function.
GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL image_create( unsigned int width, unsigned int height, HIMG* pout_img )
{
  if( !pout_img )
    return GRAPHIN_BAD_PARAM;
  image* p = new image(width, height);
  if( p )
  {
    p->add_ref();
    *pout_img = p;
    return GRAPHIN_OK;
  }
  return GRAPHIN_PANIC;
}

GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL image_release( HIMG himg )
{
  if(himg) himg->release();
  return GRAPHIN_OK;
}

GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL image_get_info(
   HIMG himg,
   BYTE** data, 
   unsigned int* width,
   unsigned int* height,
   int* stride,
   unsigned int* pixel_format)
{
  if( !himg ) return GRAPHIN_BAD_PARAM;
  if( data ) *data = himg->pmap.buf();
  if( width ) *width = himg->pmap.width();
  if( height ) *height = himg->pmap.height();
  if( stride ) *stride = himg->pmap.stride(); 
  if( pixel_format ) *pixel_format = 0; // TODODODODO
  return GRAPHIN_OK;
}

GRAPHIN_API GRAPHIN_RESULT  GRAPHIN_CALL image_blit( HPLATFORMGFX dst, int dst_x, int dst_y, 
    HIMG src, int src_x, int src_y, 
    int width, int height, bool blend )
{
  if( !src || !dst) return GRAPHIN_BAD_PARAM;
  RECT dstr = { dst_x, dst_y, dst_x + width, dst_y + height };
  RECT srcr = { src_x, src_y, src_x + width, src_y + height };
  if( blend )
    src->pmap.blend(dst, &dstr, &srcr);
  else 
    src->pmap.draw(dst, &dstr, &srcr);
  return GRAPHIN_OK;
}

GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL graphics_create(HIMG img, HGFX* pout_gfx )
{
  if( !pout_gfx )
    return GRAPHIN_BAD_PARAM;
  graphics* p = new graphics(img);
  if(p) 
  {
    p->add_ref();
    *pout_gfx = p;
    return GRAPHIN_OK;
  }
  return GRAPHIN_PANIC;
}

GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL graphics_release( HGFX hgfx )
{
  if(hgfx) hgfx->release();
  return GRAPHIN_OK;
}

// Draws line from x1,y1 to x2,y2 using current lineColor and lineGradient.
GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL
        graphics_line ( HGFX hgfx, POS x1, POS y1, POS x2, POS y2 )
{
  if(!hgfx)
    return GRAPHIN_BAD_PARAM;
  hgfx->line(x1,y1,x2,y2);
  return GRAPHIN_OK;
}

GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL
        graphics_triangle ( HGFX hgfx, POS x1, POS y1, POS x2, POS y2, POS x3, POS y3 )
{
  if(!hgfx)
    return GRAPHIN_BAD_PARAM;
  hgfx->triangle(x1,y1,x2,y2,x3,y3);
  return GRAPHIN_OK;
}

GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL
        graphics_rectangle ( HGFX hgfx, POS x1, POS y1, POS x2, POS y2 )
{
  if(!hgfx)
    return GRAPHIN_BAD_PARAM;
  hgfx->rectangle(x1,y1,x2,y2);
  return GRAPHIN_OK;
}

GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL
        graphics_rounded_rectangle ( HGFX hgfx, POS x1, POS y1, POS x2, POS y2, POS rx, POS ry)
{
  if(!hgfx)
    return GRAPHIN_BAD_PARAM;
  hgfx->roundedRect(x1,y1,x2,y2,rx,ry); 
  return GRAPHIN_OK;
}

inline COLOR rgbt( unsigned r, unsigned g, unsigned b, unsigned a ) { return ((a & 0xff) << 24) | ((b & 0xff) << 16) | ((g & 0xff) << 8) | (r & 0xff); }
inline unsigned r( COLOR c ) { return c & 0xff; }
inline unsigned g( COLOR c ) { return (c >> 8)  & 0xff; }
inline unsigned b( COLOR c ) { return (c >> 16) & 0xff; }
inline unsigned a( COLOR c ) { return (c >> 24) & 0xff; } // alpha
inline unsigned t( COLOR c ) { return 255 - a(c); }       // transparency

inline Agg2D::Color AGG_COLOR(COLOR c)
{
  return Agg2D::Color( r(c), g(c), b(c), a(c) );
}

GRAPHIN_API COLOR GRAPHIN_CALL
        graphics_rgbt(unsigned int red, unsigned int green, unsigned int blue, unsigned int transparency)
{
  return rgbt(red,green,blue,255-transparency);
}

GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL
        graphics_line_color ( HGFX hgfx, COLOR c)
{
  if(!hgfx)
    return GRAPHIN_BAD_PARAM;
  if( t(c) == 0xFF000000 )
    hgfx->noLine();
  else 
    hgfx->lineColor(AGG_COLOR(c));
  return GRAPHIN_OK;
}

// Draws circle or ellipse using current lineColor/lineGradient and fillColor/fillGradient.
GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL
        graphics_ellipse ( HGFX hgfx, POS x, POS y, POS rx, POS ry )
{
  if(!hgfx)
    return GRAPHIN_BAD_PARAM;
  hgfx->ellipse(x,y,rx,ry); 
  return GRAPHIN_OK;
}

GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL
        graphics_arc ( HGFX hgfx, POS x, POS y, POS rx, POS ry, ANGLE start, ANGLE sweep )
{
  if(!hgfx)
    return GRAPHIN_BAD_PARAM;
  hgfx->arc(x, y, rx, ry, start, sweep);
  return GRAPHIN_OK;
}


// Draws star.
GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL
        graphics_star ( HGFX hgfx, POS x, POS y, POS r1, POS r2, ANGLE start, unsigned rays )
{
  if(!hgfx)
    return GRAPHIN_BAD_PARAM;
  hgfx->star(x, y, r1, r2, start, rays);
  return GRAPHIN_OK;
}

GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL
        graphics_polygon ( HGFX hgfx, POS* xy, unsigned int num_points )
{
  if(!hgfx)
    return GRAPHIN_BAD_PARAM;
  hgfx->polygon(xy, num_points);
  return GRAPHIN_OK;
}

GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL
        graphics_polyline ( HGFX hgfx, POS* xy, unsigned int num_points )
{
  if(!hgfx)
    return GRAPHIN_BAD_PARAM;
  hgfx->polyline(xy, num_points);
  return GRAPHIN_OK;
}

GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL
        graphics_begin_path ( HGFX hgfx )
{
  if(!hgfx)
    return GRAPHIN_BAD_PARAM;
  hgfx->resetPath();
  return GRAPHIN_OK;
}

GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL
        graphics_move_to ( HGFX hgfx, POS x, POS y, bool rel )
{
  if(!hgfx)
    return GRAPHIN_BAD_PARAM;
  if( rel )
    hgfx->moveRel(x, y);
  else
    hgfx->moveTo(x, y);
  return GRAPHIN_OK;
}

GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL
        graphics_line_to ( HGFX hgfx, POS x, POS y, bool rel )
{
  if(!hgfx)
    return GRAPHIN_BAD_PARAM;
  if( rel )
    hgfx->lineRel(x, y);
  else
    hgfx->lineTo(x, y);
  return GRAPHIN_OK;
}

GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL
        graphics_hline_to ( HGFX hgfx, POS x, bool rel )
{
  if(!hgfx)
    return GRAPHIN_BAD_PARAM;
  if( rel )
    hgfx->horLineRel(x);
  else
    hgfx->horLineTo(x);
  return GRAPHIN_OK;
}

GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL
        graphics_vline_to ( HGFX hgfx, POS y, bool rel )
{
  if(!hgfx)
    return GRAPHIN_BAD_PARAM;
  if( rel )
    hgfx->verLineRel(y);
  else
    hgfx->verLineTo(y);
  return GRAPHIN_OK;
}

GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL
        graphics_arc_to ( HGFX hgfx, POS x, POS y, ANGLE angle, POS rx, POS ry, bool is_large_arc, bool sweep_flag, bool rel )
{
  if(!hgfx)
    return GRAPHIN_BAD_PARAM;
  if( rel )
    hgfx->arcRel(rx, ry, angle, is_large_arc, sweep_flag, x, y);
  else
    hgfx->arcTo(rx, ry, angle, is_large_arc, sweep_flag, x, y);
  return GRAPHIN_OK;
}
