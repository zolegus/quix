//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
#ifndef GUARD__NETWORK__SOCKET_LISTENING_HPP
#define GUARD__NETWORK__SOCKET_LISTENING_HPP
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
#include <network/socket_listening.hh>
#include <utility>
#include <stdexcept>
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
#include <unistd.h> // write/close
#include <errno.h> //errno
#include <string.h> //strerror
#include <sys/socket.h>
#include <netinet/in.h> //sockaddr_in
#include <sys/un.h>
#include <arpa/inet.h>
#include <signal.h>
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
template< typename A > 
struct network::socket_listening< A >::impl
{
  using address_type = A;

  int fd_mem{ 0 };

  impl( const std::string& );
  ~impl();

  int accept();
};
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
template< typename A > 
network::socket_listening< A >::impl::impl(
  const std::string &address_arg
  )
{
  unlink( address_arg.c_str() );
	address_type address( address_arg );

  if( ( fd_mem = socket( address.addr()->sa_family, SOCK_STREAM, 0 ) ) < 0 )
  {
		std::string what( "Failed create listen socket for port: " );
    what += "[" + address_arg + "] ";
    throw std::runtime_error( ( what + strerror( errno ) ).c_str() );
  }

  if( bind( fd_mem, address.addr(), address.size() ) < 0 )
  {
    close( fd_mem );
		std::string what( "Failed create listen socket for port: " );
    what += "[" + address_arg + "] ";
    throw std::runtime_error( ( what + strerror( errno ) ).c_str() );
  }

  if( listen( fd_mem, 1024 ) < 0 )
  {
    close( fd_mem );
		std::string what( "Failed to bind listen socket: " );
    what += "[" + address_arg + "] ";
    throw std::runtime_error( ( what + strerror( errno ) ).c_str() );
  }
}
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
template< typename A > 
network::socket_listening< A >::impl::~impl(
  )
{
  close( fd_mem );
}
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
template< typename A > 
int
network::socket_listening< A >::impl::accept()
{
  int fd_new;

  if( ( fd_new = ::accept( fd_mem, ( struct sockaddr* )0, 0 ) ) < 0 )
  {
		std::string what( "Failed to accept incoming connection on port: " );
    throw std::runtime_error( ( what + strerror( errno ) ).c_str() );
  }

 return fd_new;
}
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
template< typename A > 
network::socket_listening< A >::socket_listening(
  const std::string &address_arg
  )
  : pimpl( 
      new impl( 
        address_arg 
        )
      )
{
  return;
}
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
template< typename A > 
network::socket_listening< A >::~socket_listening()
{
  delete pimpl;
}
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
template< typename A > 
int
network::socket_listening< A >::accept()
{
  return pimpl->accept();
}
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
template< typename A > 
std::string
toString(
  const network::socket_listening< A >& socket_listening_arg
  )
{
  throw std::runtime_error( "Unimplemented" );
  return ""; 
}
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
#endif // GUARD__NETWORK__SOCKET_LISTENING_HPP
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
