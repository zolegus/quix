//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
#include <network/address_inet.hpp>
#include <stdexcept>
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
#include <sys/socket.h>
#include <netinet/in.h> //sockaddr_in
#include <sys/un.h>
#include <arpa/inet.h>
#include <signal.h>
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
network::address_inet::address_inet(
  const std::string &address_arg
  )
{
	auto sep = address_arg.find( ":" );
  if( sep == std::string::npos ) throw std::runtime_error( "Bad address arg in configuration file" );
	uint32_t host = htonl( INADDR_ANY );
	if( sep ) host = inet_addr( address_arg.substr( 0, sep ).c_str() );
	uint16_t port = atoi( address_arg.substr( sep + 1 ).c_str() );
  if( port < 10000 ) throw std::runtime_error( "port to low" );

  bzero( &sockaddr_in_mem, size() );
	sockaddr_in_mem.sin_family = AF_INET;
  sockaddr_in_mem.sin_addr.s_addr = host;
  sockaddr_in_mem.sin_port = htons( port );
}
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
const struct sockaddr*
network::address_inet::addr()
{
  return ( struct sockaddr* )&sockaddr_in_mem;
}
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
std::size_t
network::address_inet::size()
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
