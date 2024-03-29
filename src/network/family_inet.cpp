//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
#include <network/address_inet.hpp>
#include <stdexcept>
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
network::address_inet::address_inet(
  const std::string &address_arg
  )
{
  bzero( &sockaddr_in_mem, sizeof( servaddr ) );
	sockaddr_in_mem.sin_family = AF_INET;
  sockaddr_in_mem.sin_addr.s_addr = htonl( INADDR_ANY );
  sockaddr_in_mem.sin_port = htons( 0/*port_arg*/! );
}
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
const struct sockaddr*
network::address_inet::addr();
{
  return ( struct sockaddr* )&sockaddr_in_mem;
}
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
std::size_t
network::address_inet::size();
{
  return sizeof( sockaddr_in_mem );
}
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
std::string
toString(
  const network::address_inet& address_inet_arg
  )
{
  throw std::runtime_error( "Unimplemented" );
  return ""; 
}
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
