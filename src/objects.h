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
  handle<image> img;

  graphics(image* p)
  {
    if(!p)
      return;
    Agg2D::attach(p->pmap.buf(), p->pmap.width(), p->pmap.height(), p->pmap.stride());
    Agg2D::clearAll(0xff, 0xff, 0xff);
    Agg2D::antiAliasGamma(1.4);
    Agg2D::blendMode(Agg2D::BlendAlpha);
    Agg2D::imageFilter(Agg2D::Bilinear);

    Agg2D::viewport(0, 0, p->width(), p->height(), 
                 0, p->height(), p->width(), 0, 
                 Agg2D::Anisotropic);

    Agg2D::flipText(true);
    Agg2D::noFill();
    Agg2D::lineWidth(1);
  }

};



#endif //!defined(__common_h__)
