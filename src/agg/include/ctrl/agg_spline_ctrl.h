//----------------------------------------------------------------------------
// Anti-Grain Geometry - Version 2.4
// Copyright (C) 2002-2005 Maxim Shemanarev (http://www.antigrain.com)
//
// Permission to copy, use, modify, sell and distribute this software 
// is granted provided this copyright notice appears in all copies. 
// This software is provided "as is" without express or implied
// warranty, and with no claim as to its suitability for any purpose.
//
//----------------------------------------------------------------------------
// Contact: mcseem@antigrain.com
//          mcseemagg@yahoo.com
//          http://www.antigrain.com
//----------------------------------------------------------------------------
//
// classes spline_ctrl_impl, spline_ctrl
//
//----------------------------------------------------------------------------

#ifndef AGG_SPLINE_CTRL_INCLUDED
#define AGG_SPLINE_CTRL_INCLUDED

#include "agg_basics.h"
#include "agg_ellipse.h"
#include "agg_bspline.h"
#include "agg_conv_stroke.h"
#include "agg_path_storage.h"
#include "agg_trans_affine.h"
#include "agg_color_rgba.h"
#include "agg_ctrl.h"

namespace agg
{

    //------------------------------------------------------------------------
    // Class that can be used to create an interactive control to set up 
    // gamma arrays.
    //------------------------------------------------------------------------
    class spline_ctrl_impl : public ctrl
    {
    public:
        spline_ctrl_impl(real x1, real y1, real x2, real y2, 
                         unsigned num_pnt, bool flip_y=false);

        // Set other parameters
        void border_width(real t, real extra=0.0f);
        void curve_width(real t) { m_curve_width = t; }
        void point_size(real s)  { m_point_size = s; }

        // Event handlers. Just call them if the respective events
        // in your system occure. The functions return true if redrawing
        // is required.
        virtual bool in_rect(real x, real y) const;
        virtual bool on_mouse_button_down(real x, real y);
        virtual bool on_mouse_button_up(real x, real y);
        virtual bool on_mouse_move(real x, real y, bool button_flag);
        virtual bool on_arrow_keys(bool left, bool right, bool down, bool up);

        void active_point(int i);

        const real* spline()  const { return m_spline_values;  }
        const int8u*  spline8() const { return m_spline_values8; }
        real value(real x) const;
        void   value(unsigned idx, real y);
        void   point(unsigned idx, real x, real y);
        void   x(unsigned idx, real x) { m_xp[idx] = x; }
        void   y(unsigned idx, real y) { m_yp[idx] = y; }
        real x(unsigned idx) const { return m_xp[idx]; }
        real y(unsigned idx) const { return m_yp[idx]; }
        void  update_spline();

        // Vertex soutce interface
        unsigned num_paths() { return 5; }
        void     rewind(unsigned path_id);
        unsigned vertex(real* x, real* y);

    private:
        void calc_spline_box();
        void calc_curve();
        real calc_xp(unsigned idx);
        real calc_yp(unsigned idx);
        void set_xp(unsigned idx, real val);
        void set_yp(unsigned idx, real val);

        unsigned m_num_pnt;
        real   m_xp[32];
        real   m_yp[32];
        bspline  m_spline;
        real   m_spline_values[256];
        int8u    m_spline_values8[256];
        real   m_border_width;
        real   m_border_extra;
        real   m_curve_width;
        real   m_point_size;
        real   m_xs1;
        real   m_ys1;
        real   m_xs2;
        real   m_ys2;
        path_storage              m_curve_pnt;
        conv_stroke<path_storage> m_curve_poly;
        ellipse                   m_ellipse;
        unsigned m_idx;
        unsigned m_vertex;
        real   m_vx[32];
        real   m_vy[32];
        int      m_active_pnt;
        int      m_move_pnt;
        real   m_pdx;
        real   m_pdy;
        const trans_affine* m_mtx;
    };


    template<class ColorT> class spline_ctrl : public spline_ctrl_impl
    {
    public:
        spline_ctrl(real x1, real y1, real x2, real y2, 
                    unsigned num_pnt, bool flip_y=false) :
            spline_ctrl_impl(x1, y1, x2, y2, num_pnt, flip_y),
            m_background_color(rgba(1.0f, 1.0f, 0.9f)),
            m_border_color(rgba(0.0f, 0.0f, 0.0f)),
            m_curve_color(rgba(0.0f, 0.0f, 0.0f)),
            m_inactive_pnt_color(rgba(0.0f, 0.0f, 0.0f)),
            m_active_pnt_color(rgba(1.0f, 0.0f, 0.0f))
        {
            m_colors[0] = &m_background_color;
            m_colors[1] = &m_border_color;
            m_colors[2] = &m_curve_color;
            m_colors[3] = &m_inactive_pnt_color;
            m_colors[4] = &m_active_pnt_color;
        }

        // Set colors
        void background_color(const ColorT& c)   { m_background_color = c; }
        void border_color(const ColorT& c)       { m_border_color = c; }
        void curve_color(const ColorT& c)        { m_curve_color = c; }
        void inactive_pnt_color(const ColorT& c) { m_inactive_pnt_color = c; }
        void active_pnt_color(const ColorT& c)   { m_active_pnt_color = c; }
        const ColorT& color(unsigned i) const { return *m_colors[i]; } 

    private:
        spline_ctrl(const spline_ctrl<ColorT>&);
        const spline_ctrl<ColorT>& operator = (const spline_ctrl<ColorT>&);

        ColorT  m_background_color;
        ColorT  m_border_color;
        ColorT  m_curve_color;
        ColorT  m_inactive_pnt_color;
        ColorT  m_active_pnt_color;
        ColorT* m_colors[5];
    };




}


#endif
