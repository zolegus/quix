//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
#ifndef GUARD__BEHAVIOUR__TCP_SEND__HPP
#define GUARD__BEHAVIOUR__TCP_SEND__HPP
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
#include <behaviour/tcp_send.hh>
#include <network/shared_socket.hh>
#include <types/buffer.hh>
#include <stdexcept>
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

	network::shared_socket sender_mem;

  impl( const std::string& );
  void operator()( event_type &event );
  bool post();
};
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
template< typename E >
behaviour::tcp_send< E >::impl::impl(
  const std::string &address_arg
  )
	: sender_mem( address_arg )
{
  return;
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
	//sender_mem.send( event.buffer_mem->data, event.buffer_mem->size );
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
  throw std::runtime_error( "Unimplemented" );
  return ""; 
}
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
#endif // GUARD__BEHAVIOUR__TCP_SEND__HPP
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
