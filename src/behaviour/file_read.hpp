//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
#ifndef GUARD__BEHAVIOUR__FILE_READ__HPP
#define GUARD__BEHAVIOUR__FILE_READ__HPP
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
#include <behaviour/file_read.hh>
#include <types/event.hh>
#include <types/buffer.hh>
#include <fstream>
#include <stdexcept>
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
#define POOL_COUNT 256
#define POOL_MASK  ( POOL_COUNT - 1 )
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
template< typename E >
struct behaviour::file_read< E >::impl
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
	std::ifstream file;
  bool again_mem;

  impl( data_type*, const std::string& );
  void operator()( event_type &event );
  bool post();
};
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
template< typename E >
behaviour::file_read< E >::impl::impl(
	data_type* data_arg,
  const std::string& filename_arg
  )
  : data_mem( data_arg )
	, file( filename_arg )
{
  file.exceptions( std::ifstream::badbit | std::ifstream::failbit );
}
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
template< typename E >
void
behaviour::file_read< E >::impl::operator()(
  event_type &event 
  )
{
  event.buffer_mem = &data_mem->buffer_pool[ ++data_mem->pool_slot_cursor ];
  file.getline( event.buffer_mem->data, event.buffer_mem->reserved );
  event.buffer_mem->size = file.gcount() - 1;
  if( event.buffer_mem->size ) again_mem == false;
}
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
template< typename E >
bool
behaviour::file_read< E >::impl::post()
{
  return again_mem;
}
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
template< typename E >
behaviour::file_read< E >::file_read(
	void* data_arg,
  const std::string& filename_arg
  )
  : pimpl( 
      new impl( 
        reinterpret_cast< typename impl::data_type* >( data_arg ), 
        filename_arg
        ) 
      )
{
  return;
}
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
template< typename E >
behaviour::file_read< E >::~file_read()
{
  delete pimpl;
}
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
template< typename E >
void
behaviour::file_read< E >::operator()( 
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
behaviour::file_read< E >::post()
{
  return pimpl->post();
}
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
template< typename E >
std::string
toString(
  const behaviour::file_read< E >& file_read_arg
  )
{
  throw std::runtime_error( "Unimplemented" );
  return ""; 
}
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
#endif // GUARD__BEHAVIOUR__FILE_READ__HPP
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
