// win32.cpp : Defines the entry point for the application.
//

#include "win32.h"
#include "resource.h"

#include "window.h"

bool GRAPHIN_CALL wproc(HWINDOW hw, unsigned message, void* params);

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
  application_init(hInstance, lpCmdLine);

  HWINDOW hw = 0;
  window_create( wproc, 0, WINDOW_TYPE_FRAME, &hw);
  window_show(hw, WINDOW_SHOW );

  int retval = 0; 
	// main message loop:
	while (application_do_event(&retval) == GRAPHIN_OK); 

	return retval;
}

extern void graphic_paint(WINDOW_ON_PAINT_PARAMS* p);

bool GRAPHIN_CALL wproc(HWINDOW hw, unsigned message, void* params)
{
  switch( message )
  {
    case WINDOW_ON_PAINT:
    {
      WINDOW_ON_PAINT_PARAMS *p = (WINDOW_ON_PAINT_PARAMS *)params;
      graphic_paint( p );
      return true;
    }
    case WINDOW_ON_CLOSE:
    {
      window_destroy(hw);
      application_request_exit(0);
      return true;
    }
  }
  return false;
}

//      graphic_paint(hdc, rt.right - rt.left, rt.bottom - rt.top);

