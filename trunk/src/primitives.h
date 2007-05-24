#ifndef __stakan_h__
#define __stakan_h__

#include "assert.h"

  template <class T>
  class handle
  {
  public:
    handle ()               { _ptr = 0; }
    handle ( T* p )         { _ptr = 0; _set ( p ); }
    handle ( const handle<T>& p )     { _ptr = 0; _set ( p._ptr ); }

    ~handle ()              {  _set (0);  }

    handle<T>& operator= ( T *p )               { _set ( p ); return *this;  }
    handle<T>& operator=( const handle<T>& p )  { _set ( p._ptr );  return *this; }

    T* operator-> () const  { return _ptr;  }
    T* ptr () const         { return _ptr; }

    operator T* () const    { return _ptr; }
    
    bool is_null () const   { return _ptr == 0; }

    bool operator == ( T *p ) const  { return _ptr == p;  }

    private:
    T* _ptr;

    void  _set ( T *p )
    {
      if ( _ptr == p ) return;
      if ( _ptr ) _ptr->release();
      _ptr = p;
      if ( _ptr ) _ptr->add_ref();
    }
  };

  class resource
  {
    int _ref_cntr;
  public:
    resource ()             {  _ref_cntr = 0; }
    virtual ~resource ()    {  assert ( _ref_cntr == 0 ); }
    long release()
    { 
        assert(_ref_cntr > 0); 
        long t = --_ref_cntr;
        if(t == 0)
          delete this;
        return t;
    }
    void add_ref() { ++_ref_cntr; }
  };
 

#endif