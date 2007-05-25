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

