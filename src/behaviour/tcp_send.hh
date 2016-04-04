//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
#ifndef GUARD__BEHAVIOUR__TCP_SEND__HH
#define GUARD__BEHAVIOUR__TCP_SEND__HH
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
#include <behaviour/tcp_send.h>
#include <string>
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
template< typename E > 
class behaviour::tcp_send
{
  struct impl;
  impl *pimpl;

public:

  using event_type = E;

  tcp_send(  void*, const std::string&, int );
  tcp_send( const tcp_send& ) = delete;
  tcp_send( tcp_send&& ) = delete;
  tcp_send& operator=( tcp_send ) = delete;
  tcp_send& operator=( tcp_send&& ) = delete;
  ~tcp_send();

  void operator()( event_type& );
  bool post();
};
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
#endif // GUARD__BEHAVIOUR__TCP_SEND__HH
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//