//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
#ifndef GUARD__BEHAVIOUR__TCP_RECV__HPP
#define GUARD__BEHAVIOUR__TCP_RECV__HPP
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
#include <behaviour/tcp_recv.hh>
#include <types/event.hh>
#include <types/buffer.hh>
#include <network/shared_socket.hh>
#include <stdexcept>
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
template< typename E >
struct behaviour::tcp_recv< E >::impl
{
  static constexpr int pool_slot_count{ 256 };
  static constexpr int pool_mask{ pool_slot_count - 1 };

  using event_type = E;
  using buffer_type = typename event_type::buffer_type;

  struct data_type
	{
    alignas( 64 ) int pool_slot_cursor;
    alignas( 64 ) buffer_type buffer_pool[ pool_slot_count ];
	};

  data_type *data_mem;
	network::shared_socket receiver_mem;

  impl( data_type*, const std::string& ) ;
  void operator()( event_type& );
  bool post();
};
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
template< typename E >
behaviour::tcp_recv< E >::impl::impl(
  data_type *data_arg,
  const std::string &address_arg
  )
  : data_mem( data_arg )
	, receiver_mem( address_arg )
{
  return;
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
  event.buffer_mem = &data_mem->buffer_pool[ ++data_mem->pool_slot_cursor ];
  //event.buffer_mem->size = receiver_mem.recv( event.buffer_mem->data, event.buffer_mem->reserved );
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
  const std::string &address_arg
  )
  : pimpl( 
      new impl( 
        reinterpret_cast< typename impl::data_type* >( data_arg ), 
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
  throw std::runtime_error( "Unimplemented" );
  return ""; 
}
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
#endif // GUARD__BEHAVIOUR__TCP_RECV__HPP
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
