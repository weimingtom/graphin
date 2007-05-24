// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(__objects_h__)
#define __objects_h__

#ifdef _WINDOWS

  #include "platform/win32/agg_win32_bmp.h"

#else

  #error "platform is not supported yet!"

#endif 

#include "agg2d.h"
#include "primitives.h"

struct image:
  public resource,
  public Agg2D::Image
{
    agg::pixel_map pmap;
    
    image(unsigned int width, unsigned int height)
    {
      pmap.create(width, height, agg::org_color32);
      attach(pmap.buf(),width,height,pmap.stride());
    }
};

struct graphics: 
  public resource,
  public Agg2D
{
};



#endif //!defined(__common_h__)
