//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
#include <network/address_unix.hpp>
#include <utility>
#include <stdexcept>
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h> //sockaddr_in
#include <sys/un.h>
#include <arpa/inet.h>
#include <signal.h>
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
network::address_unix::address_unix(
  const std::string &address_arg
  )
{
  bzero( &sockaddr_un_mem, size() );
	sockaddr_un_mem.sun_family = AF_LOCAL;
	strcpy( sockaddr_un_mem.sun_path, address_arg.c_str() );
}
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
const struct sockaddr*
network::address_unix::addr()
{
  return ( struct sockaddr* )&sockaddr_un_mem;
}
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
std::size_t
network::address_unix::size()
{
  return sizeof( sockaddr_un_mem );
}
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
std::string
toString(
  const network::address_unix& address_unix_arg
  )
{
  throw std::runtime_error( "Unimplemented" );
  return ""; 
}
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
