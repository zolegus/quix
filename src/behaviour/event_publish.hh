//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
#ifndef GUARD__BEHAVIOUR__EVENT_PUBLISH__HH
#define GUARD__BEHAVIOUR__EVENT_PUBLISH__HH
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
#include <behaviour/event_publish.h>
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
template< typename E > 
class behaviour::event_publish
{
  struct impl;
  impl *pimpl;

public:

  using event_type = E;

  event_publish( void*, const unsigned int );
  event_publish( const event_publish& ) = delete;
  event_publish( event_publish&& ) = delete;
  event_publish& operator=( event_publish ) = delete;
  event_publish& operator=( event_publish&& ) = delete;
  ~event_publish();

  void  operator()( event_type& );
  bool post();
};
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
#endif // GUARD__BEHAVIOUR__EVENT_PUBLISH__HH
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//