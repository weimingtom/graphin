#include "window.h"  
#include "objects.h"

#include <windows.h>

#ifndef WM_IDLE
#define WM_IDLE WM_APP
#endif

  struct window
  {
    window_function*  pf;
    void*             tag;
    handle<image>     surface;
    bool notify( unsigned cmd, void *params )
    {
      return pf( tag, cmd, params );
    }
  };
  
  static LRESULT CALLBACK wnd_proc( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

  static LRESULT CALLBACK wnd_proc_primordial( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
  {
    if(msg == WM_NCCREATE)
    {
      CREATESTRUCT* lpcs = (CREATESTRUCT*)lParam;
      window* pw = (window*)lpcs->lpCreateParams;
      SetWindowLongPtr(hwnd, GWLP_USERDATA, LONG_PTR(pw));
      SetWindowLongPtr(hwnd, GWLP_WNDPROC,  LONG_PTR(&wnd_proc));
    }
    return DefWindowProc(hwnd, msg, wParam, lParam);
  }

  void wpaint(window* pw, HWND hwnd, HDC hdc, RECT ri)
  {
    WINDOW_ON_PAINT_PARAMS p;
		RECT rt; GetClientRect(hwnd, &rt);
    if( rt.right - rt.left != int(pw->surface->pmap.width()) ||
        rt.bottom - rt.top != int(pw->surface->pmap.height()) )
    {
      pw->surface = new image(rt.right - rt.left, rt.bottom - rt.top);
      p.clip_x = rt.left;
      p.clip_y = rt.top;
      p.clip_w = rt.right - rt.left;
      p.clip_h = rt.bottom - rt.top;
    }
    else
    {
      p.clip_x = ri.left;
      p.clip_y = ri.top;
      p.clip_w = ri.right - ri.left;
      p.clip_h = ri.bottom - ri.top;
    }
    p.surface = pw->surface; 
    pw->notify(WINDOW_ON_PAINT, &p);
    image_blit(hdc,p.clip_x,p.clip_y,pw->surface.ptr(),p.clip_x,p.clip_y,p.clip_w,p.clip_h, false);  
  }

  static LRESULT CALLBACK wnd_proc( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
  {
    window* pw = (window*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
    if(!pw)
      return DefWindowProc(hwnd, msg, wParam, lParam);

    switch(msg)
    {
      case WM_CREATE: 
        pw->notify(WINDOW_ON_CREATE, 0); 
        break;
      case WM_DESTROY: 
        pw->notify(WINDOW_ON_DESTROY, 0); 
        SetWindowLongPtr(hwnd, GWLP_USERDATA, 0);
        delete pw;
        break;
      case WM_SIZE: 
        if(::IsWindow(hwnd))
        {
          RECT rc; GetClientRect(hwnd, &rc);
          WINDOW_ON_SIZE_PARAMS p;
          p.width = rc.right - rc.left;
          p.height = rc.bottom - rc.top;
          if(pw->notify(WINDOW_ON_SIZE, &p))
          {
            //todo: MoveWindow here as client changed requested dimensions 
          }
        }
        break;
      case WM_MOVE: 
        if(::IsWindow(hwnd))
        {
          RECT rc; GetClientRect(hwnd, &rc);
          ::MapWindowPoints(hwnd,HWND_DESKTOP,(LPPOINT)&rc, 1);
          WINDOW_ON_MOVE_PARAMS p;
          p.x = rc.left;
          p.y = rc.top;
          if(pw->notify(WINDOW_ON_SIZE, &p))
          {
            //todo: MoveWindow here as client changed requested position 
          }
        } break;
      case WM_ACTIVATE:
        {
          WINDOW_ON_ACTIVATE_PARAMS p;
          p.manner = LOWORD(wParam) != WA_INACTIVE;
          pw->notify(WINDOW_ON_ACTIVATE, &p);
        } break;
      case WM_IDLE:
        pw->notify(WINDOW_ON_IDLE, 0);
        break;
      case WM_TIMER:
        {
          WINDOW_ON_TIMER_PARAMS p;
          p.id = wParam;
          if(!pw->notify(WINDOW_ON_TIMER, &p))
            ::KillTimer(hwnd, wParam);
          return 0;
        } 
      case WM_ERASEBKGND:
        return TRUE;

      case WM_PAINT:
        {
          PAINTSTRUCT ps;
          HDC hdc = BeginPaint(hwnd, &ps);
          wpaint(pw, hwnd, hdc, ps.rcPaint);
          EndPaint(hwnd, &ps);
          return 0;
        } 
      case WM_PRINTCLIENT:
        {
          HDC hdc = (HDC) wParam;
          RECT ri; ::GetClipBox(hdc,&ri);
          wpaint(pw, hwnd, hdc, ri);
          return 0;
        } 

    }
    return DefWindowProc(hwnd, msg, wParam, lParam);
  }

/*

template <class BASE>
  struct window_impl
{
  HWND _hwnd;
  window_impl(): _hwnd(0) {}
  virtual ~window_impl() { assert(_hwnd == 0); }

  void close()                       
  { 
    if(_hwnd)
     ::PostMessage(_hwnd, WM_CLOSE, 0, 0);
  }
  bool destroy()                     
  { 
    if(_hwnd)
      ::DestroyWindow(_hwnd);
  }
  bool show()               
  { 
    if(!_hwnd)
      return false;
    ::ShowWindow(_hwnd, SW_SHOW); 
    return true;
  }
  bool expand()
  { 
    if(!_hwnd)
      return false;
    ::ShowWindow(_hwnd, SW_MAXIMIZE); 
    return true;
  }
  bool collapse()               
  { 
    if(!_hwnd)
      return false;
    ::ShowWindow(_hwnd, SW_MINIMIZE); 
    return true;
  }
  void set_caption(const wchar_t* text) 
  { 
    if(!_hwnd)
      return;
    SetWindowTextW(_hwnd,text);
  }
  void request_idle()                   
  { 
    ::PostMessage(_hwnd, WM_IDLE, 0,0 ); 
  }
  void set_timer(unsigned ms, unsigned id) 
  { 
    if(ms)
      ::SetTimer(_hwnd,id,ms,0);
    else
      ::KillTimer(_hwnd,id);
  }

};
*/