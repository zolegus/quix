//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
#ifndef GUARD__BEHAVIOUR__TCP_RECV__HPP
#define GUARD__BEHAVIOUR__TCP_RECV__HPP
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
#include <behaviour/tcp_recv.hh>
#include <types/buffer.hh>
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
#include <stdio.h> // printf
#include <stdlib.h> //exit
#include <unistd.h> // read/close
#include <errno.h> //errno
#include <string.h> //strerror
#include <sys/socket.h>
#include <netinet/in.h> //sockaddr_in
#include <arpa/inet.h>
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
template< typename E >
struct behaviour::tcp_recv< E >::impl
{
  using event_type = E;
	using data_type = uint8_t;
  using buffer_type = types::buffer< char >;

  data_type *data_mem;

  unsigned int idx;
  int fd;

  impl( data_type*, const std::string&, int ) ;
  void operator()( event_type& );
  bool post();
};
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
template< typename E >
behaviour::tcp_recv< E >::impl::impl(
  data_type* data_arg,
  const std::string& hostname_arg,
  int port_arg
  )
  : data_mem( data_arg )
{
#if 0
  int socket_port = get_argument_as_int( 'p' );
  buffer_pool_ptr = raw;
  idx = 0;

  // Open socket for receiving
  struct sockaddr_in servaddr;
  bzero(&servaddr,sizeof(servaddr));
	servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  servaddr.sin_port = htons(socket_port);

  int listen_fd;
  if( ( listen_fd = socket(AF_INET, SOCK_STREAM, 0 ) ) < 0 )
  {
    printf( "Failed create listen socket for port %d: %d %s\n", 
      socket_port, errno, strerror( errno ) );
    exit( 1 );
  }

  if( bind( listen_fd, ( struct sockaddr* )&servaddr, sizeof( servaddr ) ) < 0 )
  {
    printf( "Failed to bind listen socket for  %s: %d %s\n", 
      socket_port, errno, strerror( errno ) );
    exit( 1 );
  }

  if( listen( listen_fd, TCP_LISTENQ ) < 0 )
  {
    printf( "Failed start listening on port %d: %d %s\n", 
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
behaviour::tcp_recv< E >::impl::operator()(
  event_type &event 
  )
{
  auto buffer = reinterpret_cast< buffer_type* >( data_mem );
  //buffer->size = read( fd, buffer->data, RAW_MSG_SIZE_MAX );
  buffer->data[ buffer->size ] = 0;
  event.buffer_mem = buffer;
  data_mem += ( ( sizeof( buffer->size ) + buffer->size + 63 ) & ~0x4F );
}
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
template< typename E >
bool
behaviour::tcp_recv< E >::impl::post()
{
  return true;
}
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
template< typename E >
behaviour::tcp_recv< E >::tcp_recv( 
  void* data_arg, 
  const std::string &hostname_arg, 
  int port_arg 
  )
  : pimpl( 
      new impl( 
        reinterpret_cast< typename impl::data_type* >( data_arg ), 
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
behaviour::tcp_recv< E >::~tcp_recv()
{
  delete pimpl;
}
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
template< typename E >
void
behaviour::tcp_recv< E >::operator()( 
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
behaviour::tcp_recv< E >::post()
{
  return pimpl->post();
}
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
template< typename E >
std::string
toString(
  const behaviour::tcp_recv< E >& tcp_recv_arg
  )
{ 
}
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
#endif // GUARD__BEHAVIOUR__TCP_RECV__HPP
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
