//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
#ifndef GUARD__BEHAVIOUR__FILE_READ__HPP
#define GUARD__BEHAVIOUR__FILE_READ__HPP
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
#include <behaviour/file_read.hh>
#include <types/buffer.hh>
#include <fstream>
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
  using event_type = E;
	using data_type = uint8_t;
  using buffer_type = types::buffer< char >;

  data_type *data_mem;
	std::ifstream file;

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
  return;
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
  auto buffer = reinterpret_cast< buffer_type* >( data_mem );
  file.getline( buffer->data, 65536 );
  buffer->size = file.gcount() - 1;
  event.buffer_mem = buffer;
  data_mem += ( ( sizeof( buffer->size ) + buffer->size + 63 ) & ~0x4F );
}
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
template< typename E >
bool
behaviour::file_read< E >::impl::post()
{
  return true;
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
  return; 
}
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
#endif // GUARD__BEHAVIOUR__FILE_READ__HPP
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
