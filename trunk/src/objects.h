// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(__objects_h__)
#define __objects_h__

#ifdef _WINDOWS
  #include "platform/win32/agg_win32_bmp.h"
#else  
  
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
  struct state: Agg2D::State
  {
    state* next;
    state( state* n = 0): next(n) {}
    ~state() { delete next; }
  };

  handle<image> img;
  state*        saved_states;

  graphics(image* p): saved_states(0)
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

  ~graphics()
  {
    delete saved_states;
  }

  void save_state()
  {
    saved_states = new state(saved_states); 
    saveStateTo(*saved_states);
  }
  bool restore_state()
  {
    if(!saved_states) 
      return false;
    restoreStateFrom(*saved_states);
    saved_states = saved_states->next;
    return true;
  }
};


#endif //!defined(__common_h__)
