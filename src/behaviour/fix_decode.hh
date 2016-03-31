//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
#ifndef GUARD__BEHAVIOUR__FIX_DECODE__HH
#define GUARD__BEHAVIOUR__FIX_DECODE__HH
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
#include <behaviour/fix_decode.h>
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
template< typename E > 
class behaviour::fix_decode
{
  struct impl;
  impl *pimpl;

public:

  using event_type = E;

  fix_decode( void* );
  fix_decode( const fix_decode& ) = delete;
  fix_decode( fix_decode&& ) = delete;
  fix_decode& operator=( fix_decode ) = delete;
  fix_decode& operator=( fix_decode&& ) = delete;
  ~fix_decode();

  void operator()( event_type& );
  bool post();
};
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
#endif // GUARD__BEHAVIOUR__FIX_DECODE__HH
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//

