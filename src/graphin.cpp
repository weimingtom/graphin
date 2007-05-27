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
        graphics_open_path ( HGFX hgfx )
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

GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL
        graphics_ellipse_to ( HGFX hgfx, POS x, POS y, POS rx, POS ry, bool clockwise )
{
  if(!hgfx)
    return GRAPHIN_BAD_PARAM;
  hgfx->addEllipse(x,y,rx,ry, clockwise? Agg2D::CW: Agg2D::CCW);
  return GRAPHIN_OK;
}

GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL
        graphics_quadratic_curve_to ( HGFX hgfx, POS xc, POS yc, POS x, POS y, bool rel )
{
  if(!hgfx)
    return GRAPHIN_BAD_PARAM;
  if(rel)
    hgfx->quadricCurveRel(xc, yc, x, y);
  else
    hgfx->quadricCurveTo(xc, yc, x, y);
  return GRAPHIN_OK;
}

GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL
        graphics_bezier_curve_to ( HGFX hgfx, POS xc1, POS yc1, POS xc2, POS yc2, POS x, POS y, bool rel )
{
  if(!hgfx)
    return GRAPHIN_BAD_PARAM;
  if(rel)
    hgfx->cubicCurveRel(xc1, yc1, xc2, yc2, x, y);
  else
    hgfx->cubicCurveTo(xc1, yc1, xc2, yc2, x, y);
  return GRAPHIN_OK;
}

GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL
        graphics_close_path ( HGFX hgfx )
{
  if(!hgfx)
    return GRAPHIN_BAD_PARAM;
  hgfx->closePolygon();
  return GRAPHIN_OK;
}

GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL
        graphics_draw_path ( HGFX hgfx, DRAW_PATH_MODE dpm )
{
  if(!hgfx)
    return GRAPHIN_BAD_PARAM;
  hgfx->drawPath(Agg2D::DrawPathFlag(dpm));
  return GRAPHIN_OK;
}

GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL
        graphics_rotate ( HGFX hgfx, ANGLE radians, POS* cx, POS* cy )
{
  if(!hgfx)
    return GRAPHIN_BAD_PARAM;
  if(cx && cy)
    hgfx->rotate(radians, *cx, *cy);
  else
    hgfx->rotate(radians);
  return GRAPHIN_OK;
}

GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL
        graphics_translate ( HGFX hgfx, POS cx, POS cy )
{
  if(!hgfx)
    return GRAPHIN_BAD_PARAM;
  hgfx->translate(cx, cy);
  return GRAPHIN_OK;
}

GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL
        graphics_scale ( HGFX hgfx, double x, double y )
{
  if(!hgfx)
    return GRAPHIN_BAD_PARAM;
  hgfx->scale(x, y);
  return GRAPHIN_OK;
}

GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL
        graphics_skew ( HGFX hgfx, double sx, double sy )
{
  if(!hgfx)
    return GRAPHIN_BAD_PARAM;
  hgfx->skew(sx, sy);
  return GRAPHIN_OK;
}

GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL
        graphics_transform ( HGFX hgfx, POS m11, POS m12, POS m21, POS m22, POS dx, POS dy )
{
  if(!hgfx)
    return GRAPHIN_BAD_PARAM;

  Agg2D::Affine m(m11, m12, m21, m22, dx, dy);
  hgfx->matrix(m); 
  return GRAPHIN_OK;
}

GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL
        graphics_state_save ( HGFX hgfx )
{
  if(!hgfx)
    return GRAPHIN_BAD_PARAM;
  hgfx->save_state();
  return GRAPHIN_OK;
}

GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL
        graphics_state_restore ( HGFX hgfx )
{
  if(!hgfx)
    return GRAPHIN_BAD_PARAM;
  return hgfx->restore_state()? 
    GRAPHIN_OK: 
    GRAPHIN_FAILURE;
}

GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL
      graphics_line_width ( HGFX hgfx, DIM width )
{
  if(!hgfx)
    return GRAPHIN_BAD_PARAM;
  hgfx->lineWidth(width);
  if( width == 0.0 )
    hgfx->noLine();
  return GRAPHIN_OK;
}

GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL
      graphics_line_color ( HGFX hgfx, COLOR color )
{
  if(!hgfx)
    return GRAPHIN_BAD_PARAM;
  //if( t(c) == 0xFF000000 )
  //  hgfx->noLine();
  //else 
  //  hgfx->lineColor(AGG_COLOR(c));
  hgfx->lineColor(AGG_COLOR(color));
  return GRAPHIN_OK;
}

GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL
      graphics_fill_color ( HGFX hgfx, COLOR color )
{
  if(!hgfx)
    return GRAPHIN_BAD_PARAM;
  if( t(color) == 0xFF )
    hgfx->noFill();
  else 
    hgfx->fillColor(AGG_COLOR(color));
  return GRAPHIN_OK;
}

GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL
      graphics_line_linear_gradient( HGFX hgfx,  	
       POS x1, POS y1, POS x2, POS y2, COLOR color1, COLOR color2, POS profile )
{
  if(!hgfx)
    return GRAPHIN_BAD_PARAM;

  hgfx->lineLinearGradient(x1, y1, x2, y2, AGG_COLOR(color1), AGG_COLOR(color2), profile);

  return GRAPHIN_OK;
}

GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL
      graphics_fill_linear_gradient( HGFX hgfx,  	
       POS x1, POS y1, POS x2, POS y2, COLOR color1, COLOR color2, POS profile )
{
  if(!hgfx)
    return GRAPHIN_BAD_PARAM;

  hgfx->fillLinearGradient(x1, y1, x2, y2, AGG_COLOR(color1), AGG_COLOR(color2), profile);

  return GRAPHIN_OK;
}

// setup parameters of line gradient radial fills.
GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL
      graphics_line_radial_gradient( HGFX hgfx, POS x, POS y, DIM r, COLOR color1, COLOR color2, double profile )
{
  if(!hgfx)
    return GRAPHIN_BAD_PARAM;

  hgfx->lineRadialGradient(x, y, r, AGG_COLOR(color1), AGG_COLOR(color2), profile);

  return GRAPHIN_OK;
}

// setup parameters of gradient radial fills.
GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL
      graphics_fill_radial_gradient( HGFX hgfx, POS x, POS y, DIM r, COLOR color1, COLOR color2, double profile )
{
  if(!hgfx)
    return GRAPHIN_BAD_PARAM;

  hgfx->fillRadialGradient(x, y, r, AGG_COLOR(color1), AGG_COLOR(color2), profile);

  return GRAPHIN_OK;
}

GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL
      graphics_fill_mode( HGFX hgfx, bool even_odd /* false - fill_non_zero */ )
{
  if(!hgfx)
    return GRAPHIN_BAD_PARAM;

  hgfx->fillEvenOdd( even_odd );

  return GRAPHIN_OK;
}

GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL
      graphics_font( HGFX hgfx, const char* name, DIM size, bool bold, bool italic, ANGLE angle)
{
  if(!hgfx)
    return GRAPHIN_BAD_PARAM;
  hgfx->font( name, size, bold, italic, Agg2D::VectorFontCache, angle); 
  return GRAPHIN_OK;

}

GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL
      graphics_text( HGFX hgfx, POS x, POS y, const wchar_t* text, unsigned int text_length)
{
  if(!hgfx)
    return GRAPHIN_BAD_PARAM;
  hgfx->text( x, y, text, text_length ); 
  return GRAPHIN_OK;
}

GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL
      graphics_text_width( HGFX hgfx, const wchar_t* text, unsigned int text_length, DIM* out_width)
{
  if(!hgfx || !out_width)
    return GRAPHIN_BAD_PARAM;
  *out_width = hgfx->textWidth( text, text_length ); 
  return GRAPHIN_OK;
}

GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL
      graphics_text_alignment( HGFX hgfx, TEXT_ALIGNMENT x, TEXT_ALIGNMENT y)
{
  if(!hgfx)
    return GRAPHIN_BAD_PARAM;
  hgfx->textAlignment( Agg2D::TextAlignment(x), Agg2D::TextAlignment(y) ); 
  return GRAPHIN_OK;
}

// returns height and ascent of the font.
GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL
      graphics_font_metrics( HGFX hgfx, DIM* out_height, DIM* out_ascent )
{
  if(!hgfx)
    return GRAPHIN_BAD_PARAM;
  if(out_height)
    *out_height = hgfx->fontHeight();
  if(out_ascent)
    *out_ascent = hgfx->fontAscent();
  return GRAPHIN_OK;
}

#include "imageio.h"

bool image_ctor(void* pctorPrm, unsigned int width, unsigned int height, BYTE** rowPtrs)
{
  image* pim = new image(width, height);
  if(!pim) return 0;

  int   stride = pim->pmap.stride();
  BYTE* ptr    = pim->pmap.buf();

  for(unsigned n = 0; n < height; ++n, ptr += stride)
    rowPtrs[n] = ptr;

  handle<image>* him = (handle<image>*)pctorPrm;
  *him = pim;
  return true;
}

GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL 
        image_load( const BYTE* bytes, unsigned int num_bytes, HIMG* pout_img ) // load png/jpeg/etc. image from stream of bytes
{
  handle<image> img;
  if( !DecodeImage(&image_ctor, &img, const_cast<BYTE*>(bytes), num_bytes) )
    return GRAPHIN_FAILURE;

  *pout_img = img.detach();

  return GRAPHIN_OK;
}

GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL
      graphics_draw_image ( HGFX hgfx, HIMG himg, POS x, POS y, 
                            DIM* w, DIM* h, unsigned* ix, unsigned* iy, unsigned* iw, unsigned* ih )
{
  if(!hgfx || !himg)
    return GRAPHIN_BAD_PARAM;

  double unit = 0;

  if(!w && !h)
    unit = hgfx->screenToWorld(1.0);

  unsigned src_x = ix? *ix : 0;
  unsigned src_y = iy? *iy : 0;
  unsigned src_width = iw? *iw : himg->pmap.width();
  unsigned src_height = ih? *ih : himg->pmap.height();
  
  DIM dst_width = w? *w : unit * src_width;  
  DIM dst_height = h? *h : unit * src_height;

  // void transformImage(const Image& img,    
  //                        int imgX1,    int imgY1,    int imgX2,    int imgY2,
  //                     double dstX1, double dstY1, double dstX2, double dstY2);

  hgfx->transformImage(*himg, src_x, src_y, src_x + src_width, src_y + src_height, 
                              x,y, x + dst_width, y + dst_height);
  return GRAPHIN_OK;

}
