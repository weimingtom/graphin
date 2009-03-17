#ifndef __graphinius_window_hpp__
#define __graphinius_window_hpp__

#include "window.h"

namespace graphin
{
  namespace app
  {
    bool init(void* hinstance, const char* command_line);
    bool do_event(int& retval);
    void request_exit(int retval);
    int  run();
  }

  template< class IMPL>
  class window // top level window frame
  {
    HWINDOW hwnd;
    static bool GRAPHIN_CALL wproc(HWINDOW hw, unsigned message, void* params);

    window(): hwnd(0) {}
  public:
  
    typedef WINDOW_ON_MOUSE_PARAMS MOUSE_EVT;
    typedef WINDOW_ON_KEY_PARAMS   KEY_EVT;
    typedef WINDOW_ON_PAINT_PARAMS PAINT_EVT;
    typedef WINDOW_ON_SIZE_PARAMS  SIZE_EVT;
    typedef WINDOW_ON_MOVE_PARAMS  MOVE_EVT;
    typedef WINDOW_ON_TIMER_PARAMS TIMER_EVT;
    typedef WINDOW_STATE           STATE;
  
    window(WINDOW_TYPE type);
    ~window() { destroy(); }

    bool is_valid();
    
    void destroy();
    
    void move(int x, int y, int w, int h);
    STATE get_state();
    void set_state(STATE st);
    
    void request_close();
    void request_idle();
    
    void set_timer(unsigned ms, unsigned long timer_id);
    void set_caption(const wchar_t* text);
    void get_caption(wchar_t* buffer, unsigned buffer_length);
    
    void invalidate(int x, int y, int w, int h);
    void update();
    
    // overridables
    void on_create() {}
    void on_destroy() {}
    void on_activate(bool active) {}
    void on_size(SIZE_EVT& evt) {}
    void on_move(MOVE_EVT& evt) {}
    void on_mouse(MOUSE_EVT& evt) {}
    void on_key(KEY_EVT& evt) {}
    void on_timer(TIMER_EVT& evt) {}
    void on_idle() {}
    void on_close() { destroy(); }
    void on_paint(PAINT_EVT& evt) {}
  };
  
  template <class IMPL>
    inline window<IMPL>::window(WINDOW_TYPE type)
    {
      window_create(&wproc, this, type, &hwnd);
    }
 
  template <class IMPL>
    inline bool window<IMPL>::is_valid()
    {
      return GRAPHIN_OK == window_is_valid( hwnd );
    }

  template <class IMPL>
    inline void window<IMPL>::destroy()
    { 
      if(is_valid())
      {
        window_destroy(hwnd);
        hwnd = 0;
      }
    }

  template <class IMPL>
    inline void window<IMPL>::move(int x, int y, int w, int h)
    {
      window_move( hwnd,x, y, w, h );
    }
 
  template <class IMPL>
    inline void window<IMPL>::request_close()
    {
      window_request_close(hwnd);
    }

  template <class IMPL>
    inline void window<IMPL>::request_idle()
    {
      window_request_idle(hwnd);
    }
  
  template <class IMPL>
    inline void window<IMPL>::set_state(window<IMPL>::STATE st)
    {
      window_set_state(hwnd, st);
    }

  template <class IMPL>
    inline window<IMPL>::STATE window<IMPL>::get_state()
    {
      WINDOW_STATE st;
      window_get_state(hwnd, &st);
      return st;
    }
  
  template <class IMPL>
    inline void window<IMPL>::set_timer(unsigned ms, unsigned long id)
    {
      window_set_timer(hwnd,ms,id);
    }

  template <class IMPL>
    inline void window<IMPL>::set_caption(const wchar_t* text)
    {
      window_set_caption(hwnd, text);
    }
  
  template <class IMPL>
    inline void window<IMPL>::get_caption(wchar_t* buffer, unsigned buffer_length)
    {
      window_get_caption(hwnd, buffer, buffer_length);
    }
  
  template <class IMPL>
    inline void window<IMPL>::invalidate(int x, int y, int w, int h)
    {
      window_invalidate(hwnd, x, y, w, h);
    }
  
  template <class IMPL>
    inline void window<IMPL>::update()
    {
      window_update(hwnd);
    }

  inline bool app::init(void* hinstance, const char* command_line)
  {
    return GRAPHIN_OK == application_init(hinstance, command_line);
  }
  
  inline bool app::do_event(int& retval)
  {
    return GRAPHIN_OK == application_do_event(&retval);
  }

  inline void app::request_exit(int retval)
  {
    application_request_exit(retval);
  }
  
  inline int app::run()
  {
    int retval = 0;
    while( do_event(retval) );
    return retval;
  }

  template <class IMPL>
    inline bool GRAPHIN_CALL window<IMPL>::wproc(HWINDOW hw, unsigned message, void* params)
    {
      void* pw; window_get_tag(hw,&pw);
      IMPL* pimpl = static_cast< IMPL* >( pw );
    
      switch( message )
      {
        case WINDOW_ON_CREATE:
        {
          pimpl->on_create();
          return true;
        }
        case WINDOW_ON_DESTROY:
        {
          pimpl->on_destroy();
          static_cast<window<IMPL>*>(pw)->hwnd = 0;
          return true;
        }
        case WINDOW_ON_ACTIVATE:
        {
          WINDOW_ON_ACTIVATE_PARAMS* p = (WINDOW_ON_ACTIVATE_PARAMS*)params;
          pimpl->on_activate(p->manner != 0);
          return true;
        }
        case WINDOW_ON_SIZE:
        {
          WINDOW_ON_SIZE_PARAMS* p = (WINDOW_ON_SIZE_PARAMS*)params;
          pimpl->on_size(*p);
          return true;
        }
        case WINDOW_ON_MOVE:
        {
          WINDOW_ON_MOVE_PARAMS* p = (WINDOW_ON_MOVE_PARAMS*)params;
          pimpl->on_move(*p);
          return true;
        }
        case WINDOW_ON_TIMER:
        {
          WINDOW_ON_TIMER_PARAMS* p = (WINDOW_ON_TIMER_PARAMS*)params;
          pimpl->on_timer(*p);
          return true;
        }
        case WINDOW_ON_IDLE:
        {
          pimpl->on_idle();
          return true;
        }
        case WINDOW_ON_PAINT:
        {
          WINDOW_ON_PAINT_PARAMS *prm = (WINDOW_ON_PAINT_PARAMS *)params;
          pimpl->on_paint(*prm);
          return true;
        }
        case WINDOW_ON_MOUSE:
        {
          WINDOW_ON_MOUSE_PARAMS *prm = (WINDOW_ON_MOUSE_PARAMS *)params;
          pimpl->on_mouse(*prm);
          return true;
        }
        case WINDOW_ON_KEY:
        {
          WINDOW_ON_KEY_PARAMS *prm = (WINDOW_ON_KEY_PARAMS *)params;
          pimpl->on_key(*prm);
          return true;
        }
        case WINDOW_ON_CLOSE:
        {
          pimpl->on_close();
          return true;
        }
      }
      return false;
    }
 
}

#endif
