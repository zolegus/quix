//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
#ifndef GUARD__NETWORK__SOCKET_CONNECTED__H
#define GUARD__NETWORK__SOCKET_CONNECTED__H
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
#include <string>
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
namespace network
{
  template< typename A > class socket_connected;
}
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
template< typename A > std::string toString( const network::socket_connected< A >& );
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
#endif // GUARD__NETWORK__SOCKET_CONNECTED__H
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//