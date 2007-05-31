#ifndef __graphinius_h__
#define __graphinius_h__

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
typedef double POS;         // position 
typedef double DIM;         // dimention
typedef double ANGLE;       // angle (radians)
typedef unsigned int COLOR; // guess what?

enum GRAPHIN_RESULT
{
  GRAPHIN_PANIC = -1, // e.g. not enough memory
  GRAPHIN_OK = 0,
  GRAPHIN_BAD_PARAM = 1,  // bad parameter
  GRAPHIN_FAILURE = 2,    // operation failed, e.g. restore() without save()
};

// image primitives

GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL  
        image_create( unsigned int width, unsigned int height, HIMG* poutImg );

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

// SECTION: graphics primitives and drawing operations

// create color value
GRAPHIN_API COLOR GRAPHIN_CALL 
        graphics_rgbt(unsigned int red, unsigned int green, unsigned int blue, unsigned int transparency = 0);
// Note: transparent color (no-color value) is rgba(?, ?, ?, 0xff);

GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL 
        graphics_create(HIMG img, HGFX* pout_gfx );

GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL 
        graphics_release(HGFX gfx);

// Set line color
GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL
        graphics_line_color ( HGFX hgfx, COLOR c);

// Draws line from x1,y1 to x2,y2 using current lineColor and lineGradient.
GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL
        graphics_line ( HGFX hgfx, POS x1, POS y1, POS x2, POS y2 );

// Draws triangle using current lineColor/lineGradient and fillColor/fillGradient.
GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL
        graphics_triangle ( HGFX hgfx, POS x1, POS y1, POS x2, POS y2, POS x3, POS y3 );

// Draws rectangle using current lineColor/lineGradient and fillColor/fillGradient with (optional) rounded corners.
GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL
        graphics_rectangle ( HGFX hgfx, POS x1, POS y1, POS x2, POS y2 );

// Draws rounded rectangle using current lineColor/lineGradient and fillColor/fillGradient with (optional) rounded corners.
GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL
        graphics_rounded_rectangle ( HGFX hgfx, POS x1, POS y1, POS x2, POS y2, POS rx, POS ry);

// Draws circle or ellipse using current lineColor/lineGradient and fillColor/fillGradient.
GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL
        graphics_ellipse ( HGFX hgfx, POS x, POS y, POS rx, POS ry );

// Draws closed arc using current lineColor/lineGradient and fillColor/fillGradient.
GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL
        graphics_arc ( HGFX hgfx, POS x, POS y, POS rx, POS ry, ANGLE start, ANGLE sweep );

// Draws star.
GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL
        graphics_star ( HGFX hgfx, POS x, POS y, POS r1, POS r2, ANGLE start, unsigned rays );

// Closed polygon.
GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL
        graphics_polygon ( HGFX hgfx, POS* xy, unsigned int num_points );

// Polyline.
GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL
        graphics_polyline ( HGFX hgfx, POS* xy, unsigned int num_points );

// SECTION: Path operations

GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL
        graphics_open_path ( HGFX hgfx );

GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL
        graphics_move_to ( HGFX hgfx, POS x, POS y, bool relative );

GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL
        graphics_line_to ( HGFX hgfx, POS x, POS y, bool relative );

GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL
        graphics_hline_to ( HGFX hgfx, POS x, bool relative );

GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL
        graphics_vline_to ( HGFX hgfx, POS y, bool relative );

GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL
        graphics_arc_to ( HGFX hgfx, POS x, POS y, ANGLE angle, POS rx, POS ry, bool is_large_arc, bool sweep_flag, bool relative );

GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL
        graphics_ellipse_to ( HGFX hgfx, POS x, POS y, POS rx, POS ry, bool clockwise );

GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL
        graphics_quadratic_curve_to ( HGFX hgfx, POS xc, POS yc, POS x, POS y, bool relative );

GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL
        graphics_bezier_curve_to ( HGFX hgfx, POS xc1, POS yc1, POS xc2, POS yc2, POS x, POS y, bool relative );

GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL
        graphics_close_path ( HGFX hgfx );

enum DRAW_PATH_MODE
{
    FILL_ONLY, 
    STROKE_ONLY,
    FILL_AND_STROKE,
    FILL_BY_LINE_COLOR
};

GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL
        graphics_draw_path ( HGFX hgfx, DRAW_PATH_MODE dpm );

// end of path opearations

// SECTION: affine tranformations:

GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL
        graphics_rotate ( HGFX hgfx, ANGLE radians, POS* cx = 0, POS* cy = 0 );

GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL
        graphics_translate ( HGFX hgfx, POS cx, POS cy );

GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL
        graphics_scale ( HGFX hgfx, double x, double y );

GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL
        graphics_skew ( HGFX hgfx, double dx, double dy );

// all above in one shot
GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL
        graphics_transform ( HGFX hgfx, POS m11, POS m12, POS m21, POS m22, POS dx, POS dy );

// end of affine tranformations.

// SECTION: state save/restore

GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL
        graphics_state_save ( HGFX hgfx );

GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL
        graphics_state_restore ( HGFX hgfx );

// end of state save/restore

// SECTION: drawing attributes 

// set line width for subsequent drawings.
GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL
      graphics_line_width ( HGFX hgfx, DIM width );  	

inline void 
      graphics_no_line ( HGFX hgfx ) { graphics_line_width(hgfx,0.0); }

// color for solid lines/strokes
GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL
      graphics_line_color ( HGFX hgfx, COLOR color );

// color for solid fills
GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL
      graphics_fill_color ( HGFX hgfx, COLOR color );

inline void 
      graphics_no_fill ( HGFX hgfx ) { graphics_fill_color(hgfx, graphics_rgbt(0,0,0,0xFF)); }

// setup parameters of linear gradient of lines.
GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL
      graphics_line_linear_gradient( HGFX hgfx, POS x1, POS y1, POS x2, POS y2, COLOR color1, COLOR color2, double profile = 1.0 );

// setup parameters of linear gradient of fills.
GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL
      graphics_fill_linear_gradient( HGFX hgfx, POS x1, POS y1, POS x2, POS y2, COLOR color1, COLOR color2, double profile = 1.0 );

// setup parameters of line gradient radial fills.
GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL
      graphics_line_radial_gradient( HGFX hgfx, POS x, POS y, DIM r, COLOR color1, COLOR color2, double profile = 1.0 );  	

// setup parameters of gradient radial fills.
GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL
      graphics_fill_radial_gradient( HGFX hgfx, POS x, POS y, DIM r, COLOR color1, COLOR color2, double profile = 1.0 );

GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL
      graphics_fill_mode( HGFX hgfx, bool even_odd /* false - fill_non_zero */ );

// SECTION: text 

// define font attributes for all subsequent text operations.
GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL
      graphics_font( HGFX hgfx, const char* name, DIM size, bool bold = false, bool italic = false, ANGLE angle = 0);

// draw text at x,y with text alignment
GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL
      graphics_text( HGFX hgfx, POS x, POS y, const wchar_t* text, unsigned int text_length);

// calculates width of the text string
GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL
      graphics_text_width( HGFX hgfx, const wchar_t* text, unsigned int text_length, DIM* out_width);

// returns height and ascent of the font.
GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL
      graphics_font_metrics( HGFX hgfx, DIM* out_height, DIM* out_ascent );

enum TEXT_ALIGNMENT
{
  ALIGN_TOP,
  ALIGN_BOTTOM,
  ALIGN_CENTER,
  ALIGN_BASELINE,
  ALIGN_RIGHT = ALIGN_TOP,
  ALIGN_LEFT = ALIGN_BOTTOM
  
};

// calculates width of the text string
GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL
      graphics_text_alignment( HGFX hgfx, TEXT_ALIGNMENT x, TEXT_ALIGNMENT y);

// SECTION: image rendering

// draws img onto the graphics surface with current transformation applied (scale, rotation). 
GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL
      graphics_draw_image ( HGFX hgfx, HIMG himg, POS x, POS y, 
                            DIM* w = 0, DIM* h = 0, unsigned* ix = 0, unsigned* iy = 0, unsigned* iw = 0, unsigned* ih = 0 );

// blits image bits onto underlying pixel buffer. no affine transformations.  
GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL
      graphics_blit_image ( HGFX hgfx, HIMG himg, POS x, POS y, unsigned* ix = 0, unsigned* iy = 0, unsigned* iw = 0, unsigned* ih = 0 );

// blends image bits with bits of the buffer. no affine transformations.  
GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL
      graphics_blend_image ( HGFX hgfx, HIMG himg, POS x, POS y, unsigned opacity = 0xFF, unsigned* ix = 0, unsigned* iy = 0, unsigned* iw = 0, unsigned* ih = 0 );

// SECTION: coordinate space

GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL
      graphics_world_to_screen ( HGFX hgfx, POS* inout_x, POS* inout_y);

inline GRAPHIN_RESULT
      graphics_world_to_screen ( HGFX hgfx, POS* length)
{
   return graphics_world_to_screen ( hgfx, length, 0);
}

GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL
      graphics_screen_to_world ( HGFX hgfx, POS* inout_x, POS* inout_y);

inline GRAPHIN_RESULT
      graphics_screen_to_world ( HGFX hgfx, POS* length)
{
   return graphics_screen_to_world (hgfx, length, 0);
}

// SECTION: clipping

GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL
      graphics_set_clip_box ( HGFX hgfx, POS x1, POS y1, POS x2, POS y2);

GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL
      graphics_get_clip_box ( HGFX hgfx, POS* x1, POS* y1, POS* x2, POS* y2);

GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL
      graphics_is_in_clip_box ( HGFX hgfx, POS x, POS y);

GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL
      graphics_rect_is_visible ( HGFX hgfx, POS x1, POS y1, POS x2, POS y2, bool* yes);

#endif