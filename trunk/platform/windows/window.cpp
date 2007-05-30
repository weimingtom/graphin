#include "window.h"  
#include "objects.h"
#include "resource.h"

#include <windows.h>

#ifndef WM_IDLE
#define WM_IDLE WM_APP
#endif

  const wchar_t* WINDOW_CLASS_NAME = L"Graphinius";

  struct window
  {
    unsigned          signature;
    window_function*  pf;
    void*             tag;
    HWND              hwnd;
    handle<image>     surface;
    
    window(): signature(0xAFED), pf(0), tag(0), hwnd(0) {}

    bool notify( unsigned cmd, void *params ) {  return pf( tag, cmd, params ); }
    bool is_valid() { return signature == 0xAFED; }
  };
  
  static LRESULT CALLBACK wnd_proc( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

  static LRESULT CALLBACK wnd_proc_primordial( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
  {
    if(msg == WM_NCCREATE)
    {
      CREATESTRUCT* lpcs = (CREATESTRUCT*)lParam;
      window* pw = (window*)lpcs->lpCreateParams;
      pw->hwnd = hwnd;
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
      case WM_CLOSE:
        pw->notify(WINDOW_ON_CLOSE, 0);
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


GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL 
        window_create(window_function* pf, void* tag, WINDOW_TYPE type, HWINDOW* out_hwnd)
{
    unsigned wflags = 0;
    unsigned wflagsex = 0;
    switch( type )
    {
    case WINDOW_TYPE_POPUP:  // no border, no caption
      wflags = WS_POPUP;
      break;
    case WINDOW_TYPE_FRAME:  // caption and resizeable frame
      wflags = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_THICKFRAME | WS_MAXIMIZEBOX;
      wflagsex = WS_EX_OVERLAPPEDWINDOW; 
      break;
    case WINDOW_TYPE_TOOL:   // narrow caption and resizeable frame
      wflags = WS_OVERLAPPED | WS_CAPTION | WS_MINIMIZEBOX;
      wflagsex = WS_EX_TOOLWINDOW;
      break;
    case WINDOW_TYPE_DIALOG: // caption and not resizeable frame
      wflags = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;
      wflagsex = WS_EX_DLGMODALFRAME;
      break;
    }
    window* pw = new window();
    pw->pf = pf;
    pw->surface = new image(1,1);
    pw->tag = tag;
    HWND hwnd = ::CreateWindowExW(wflagsex, WINDOW_CLASS_NAME, L"", wflags, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, 0, 0, 0, pw);
    if( !hwnd )
    {
      delete pw;
      return GRAPHIN_FAILURE;
    }
    *out_hwnd = pw;
    return GRAPHIN_OK;
}

GRAPHIN_API bool GRAPHIN_CALL 
        window_is_valid(HWINDOW hw)
{
  return hw && hw->is_valid() && ::IsWindow(hw->hwnd);
}

GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL 
        window_destroy(HWINDOW hw)
{
  if( window_is_valid(hw) )
  {
    ::DestroyWindow(hw->hwnd);
    return GRAPHIN_OK;
  }
  return GRAPHIN_FAILURE;
}

GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL 
        window_move(HWINDOW hw, int x, int y, int w, int h)
{
  if( window_is_valid(hw) )
  {
    ::MoveWindow(hw->hwnd, x, y, w, h, TRUE);
    return GRAPHIN_OK;
  }
  return GRAPHIN_FAILURE;
}

GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL 
        window_show(HWINDOW hw, SHOW_CMD cmd)
{
  if( window_is_valid(hw) )
  {
    ::PostMessage(hw->hwnd, WM_CLOSE, 0, 0);
    return GRAPHIN_OK;
  }
  return GRAPHIN_FAILURE;
}

GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL 
        window_request_close(HWINDOW hw)
{
  if( window_is_valid(hw) )
  {
    ::PostMessage(hw->hwnd, WM_CLOSE, 0, 0);
    return GRAPHIN_OK;
  }
  return GRAPHIN_FAILURE;
}

GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL 
        window_request_idle(HWINDOW hw)
{
  if( window_is_valid(hw) )
  {
    ::PostMessage(hw->hwnd, WM_IDLE, 0, 0);
    return GRAPHIN_OK;
  }
  return GRAPHIN_FAILURE;
}

GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL 
        window_set_timer(HWINDOW hw, unsigned ms, unsigned long id)
{
  if( window_is_valid(hw) )
  {
    if(ms)
      ::SetTimer(hw->hwnd, id, ms, 0);
    else
      ::KillTimer(hw->hwnd, id);
    return GRAPHIN_OK;
  }
  return GRAPHIN_FAILURE;
}

GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL 
        window_set_caption(HWINDOW hw, const wchar_t* text)
{
  if( window_is_valid(hw) )
  {
    ::SetWindowTextW(hw->hwnd,text);
    return GRAPHIN_OK;
  }
  return GRAPHIN_FAILURE;
}

GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL 
        window_get_caption(HWINDOW hw, wchar_t* buf, unsigned buf_size)
{
  if( window_is_valid(hw) )
  {
   ::GetWindowTextW(hw->hwnd,buf,buf_size);
    return GRAPHIN_OK;
  }
  return GRAPHIN_FAILURE;
}

bool application_init(void* hinstance)
{
  static bool done = false;
  if( done ) return true;

  int class_flags = 
#ifndef UNDER_CE
    CS_OWNDC | 
#endif
    CS_VREDRAW | CS_HREDRAW;

  WNDCLASSW wc;
  wc.lpszClassName = WINDOW_CLASS_NAME;
  wc.lpfnWndProc = &wnd_proc_primordial;
  wc.style = class_flags;
  wc.hInstance = (HINSTANCE)hinstance;
  wc.hIcon = LoadIcon(0, IDI_APPLICATION);
  wc.hCursor = LoadCursor(0, IDC_ARROW);
  wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
  wc.lpszMenuName = NULL;
  wc.cbClsExtra = 0;
  wc.cbWndExtra = 0;
  if(::RegisterClassW(&wc))
  {
      done = true;
      return true;
  }
  return false;
}


