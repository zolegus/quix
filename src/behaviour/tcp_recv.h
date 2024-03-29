//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
#ifndef GUARD__BEHAVIOUR__TCP_RECV__H
#define GUARD__BEHAVIOUR__TCP_RECV__H
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
#include <string>
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
namespace behaviour
{
  template< typename E > class tcp_recv;
}
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
template< typename E > std::string toString( const behaviour::tcp_recv< E >& );
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
#endif // GUARD__BEHAVIOUR__TCP_RECV__H
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
