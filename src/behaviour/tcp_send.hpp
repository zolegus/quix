//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
#ifndef GUARD__BEHAVIOUR__TCP_SEND__HPP
#define GUARD__BEHAVIOUR__TCP_SEND__HPP
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
#include <behaviour/tcp_send.hh>
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
#include <stdio.h> // printf
#include <stdlib.h> //exit
#include <unistd.h> // write/close
#include <errno.h> //errno
#include <string.h> //strerror
#include <sys/socket.h>
#include <netinet/in.h> //sockaddr_in
#include <arpa/inet.h>
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
template< typename E >
struct behaviour::tcp_send< E >::impl
{
  using event_type = E;

  int fd;

  impl( const std::string&, int );
  void operator()( event_type &event );
  bool post();
};
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
template< typename E >
behaviour::tcp_send< E >::impl::impl(
  const std::string &hostname_arg,
  int port_arg
  )
{
#if 0
  int socket_port = get_argument_as_int( 'p' );

  // Open socket for sending
  struct sockaddr_in servaddr;
  bzero(&servaddr,sizeof(servaddr));
	servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  servaddr.sin_port = htons(socket_port);

  int listen_fd;
  if( ( listen_fd = socket(AF_INET, SOCK_STREAM, 0 ) ) < 0 )
  {
    printf( "Failed create listen socket for port %p: %d %s\n", 
      socket_port, errno, strerror( errno ) );
    exit( 1 );
  }

  if( bind( listen_fd, ( struct sockaddr* )&servaddr, sizeof( servaddr ) ) < 0 )
  {
    printf( "Failed create listen socket for port %d: %d %s\n", 
      socket_port, errno, strerror( errno ) );
    exit( 1 );
  }

  if( listen( listen_fd, TCP_LISTENQ ) < 0 )
  {
    printf( "Failed to bind listen socket for  %s: %d %s\n", 
      socket_port, errno, strerror( errno ) );
    exit( 1 );
  }

  if( ( fd = accept( listen_fd, (struct sockaddr*)0, 0 ) ) < 0 )
  {
    printf( "Failed to accept incoming connection on port %d: %d %s\n", 
      socket_port, errno, strerror( errno ) );
    exit( 1 );
  }

  close( listen_fd );
#endif
}
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
template< typename E >
void
behaviour::tcp_send< E >::impl::operator()( 
  event_type &event 
  )
{
  //auto buffer = event->buffer;
  //write( fd, buffer->data, buffer->size );
}
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
template< typename E >
bool
behaviour::tcp_send< E >::impl::post()
{
  return true;
}
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
template< typename E >
behaviour::tcp_send< E >::tcp_send(
  void*,
  const std::string &hostname_arg,
  int port_arg
  )
  : pimpl( 
      new impl( 
        hostname_arg, 
        port_arg 
        )
      )
{
  return;
}
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
template< typename E >
behaviour::tcp_send< E >::~tcp_send()
{
  delete pimpl;
}
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
template< typename E >
void
behaviour::tcp_send< E >::operator()( 
  event_type &event 
  )
{
  return pimpl->operator()( event );
}
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
template< typename E >
bool 
behaviour::tcp_send< E >::post()
{
  return pimpl->post();
}
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
template< typename E >
std::string
toString(
  const behaviour::tcp_send< E >& tcp_send_arg
  )
{ 
}
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
#endif // GUARD__BEHAVIOUR__TCP_SEND__HPP
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
