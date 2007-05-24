
#ifdef _WINDOWS
  #define WIN32_LEAN_AND_MEAN		// exclude unused stuff from Windows headers
  #include <windows.h>

  typedef HDC HPLATFORMGFX;
#endif



// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the GRAPHIN_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// GRAPHIN_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.
#ifdef GRAPHIN_EXPORTS
#define GRAPHIN_API extern "C" __declspec(dllexport)
#else
#define GRAPHIN_API extern "C" __declspec(dllimport)
#endif

#define GRAPHIN_CALL __stdcall 

struct graphics;
struct image;

typedef graphics* HGFX;
typedef image*    HIMG;

#ifndef BYTE
  typedef unsigned char   BYTE;
#endif 
typedef double POS;


enum GRAPHIN_RESULT
{
  GRAPHIN_OK,
  GRAPHIN_BAD_PARAM,
};

GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL 
        image_create( int width, int height, HIMG* poutImg );

GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL 
        image_release( HIMG himg );

GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL 
        image_get_info( HIMG himg,
             BYTE** data, 
             unsigned int* width,
             unsigned int* height,
             int* stride,
             unsigned int* pixel_format);

GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL 
        image_blit( HPLATFORMGFX dst, int dst_x, int dst_y, 
                            HIMG src, int src_x, int src_y, 
                            int width, int height, bool blend );

GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL 
        image_load( const BYTE* bytes, unsigned int num_bytes, HIMG* pout_img ); // load png/jpeg/etc. image from stream of bytes

GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL 
        graphics_create(HIMG img, HGFX* pout_gfx );

GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL 
        graphics_release(HGFX gfx);

// Draws line from x1,y1 to x2,y2 using current lineColor and lineGradient.
GRAPHIN_API GRAPHIN_RESULT 
        graphics_line ( POS x1, POS y1, POS x2, POS y2 );

// Draws triangle using current lineColor/lineGradient and fillColor/fillGradient.
GRAPHIN_API GRAPHIN_RESULT 
        graphics_triangle ( POS x1, POS y1, POS x2, POS y2, POS x3, POS y3 );

// Draws rectangle using current lineColor/lineGradient and fillColor/fillGradient with (optional) rounded corners.
GRAPHIN_API GRAPHIN_RESULT 
        graphics_rectangle ( POS x1, POS y1, POS x2, POS y2 );

// Draws rounded rectangle using current lineColor/lineGradient and fillColor/fillGradient with (optional) rounded corners.
GRAPHIN_API GRAPHIN_RESULT 
        graphics_rounded_rectangle ( POS x1, POS y1, POS x2, POS y2, POS rx, POS ry);

// Draws circle or ellipse using current lineColor/lineGradient and fillColor/fillGradient.
GRAPHIN_API GRAPHIN_RESULT 
        graphics_ellipse ( POS x, POS y, POS rx, POS ry );


