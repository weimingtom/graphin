#ifndef __graphin_window_h__
#define __graphin_window_h__

#include "graphin.h"

typedef bool GRAPHIN_CALL window_function(void* tag, unsigned message, void* params);
 struct window;
typedef window* HWINDOW;

enum WINDOW_MESSAGE
{
  WINDOW_ON_CREATE,
  WINDOW_ON_DESTROY,
  WINDOW_ON_ACTIVATE,
  WINDOW_ON_SIZE,
  WINDOW_ON_MOVE,
  WINDOW_ON_MOUSE,
  WINDOW_ON_KEY,
  WINDOW_ON_TIMER,
  WINDOW_ON_IDLE,
  WINDOW_ON_CLOSE,
  WINDOW_ON_PAINT,
};

struct WINDOW_ON_PAINT_PARAMS     { HIMG surface;  int  clip_x;  int  clip_y;  int  clip_w;  int  clip_h; };
struct WINDOW_ON_SIZE_PARAMS      { int width;  int height; };
struct WINDOW_ON_MOVE_PARAMS      { int x; int y; };
struct WINDOW_ON_ACTIVATE_PARAMS  { int manner; };
struct WINDOW_ON_TIMER_PARAMS     { unsigned long id; };

GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL 
        window_create(window_function* pf, void* tag, unsigned style, HWINDOW* out_hwnd);

GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL 
        window_is_valid(HWINDOW hwnd);

GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL 
        window_destroy(HWINDOW hwnd);

GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL 
        window_move(HWINDOW hwnd, int x, int y, int w, int h);

enum SHOW_CMD
{
  WINDOW_SHOW,
  WINDOW_COLLAPSE, // minimize
  WINDOW_EXPAND,   // maximize
};

GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL 
        window_show(HWINDOW hwnd, SHOW_CMD cmd);

GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL 
        window_request_close(HWINDOW hwnd);

GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL 
        window_request_idle(HWINDOW hwnd);

GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL 
        window_set_timer(HWINDOW hwnd, unsigned ms, unsigned long id);

GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL 
        window_set_caption(HWINDOW hwnd, const wchar_t* text);

GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL 
        window_get_caption(HWINDOW hwnd, wchar_t* buf, unsigned buf_size);

#endif