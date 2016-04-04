//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
#ifndef GUARD__TRANSPORT__MCD__HPP
#define GUARD__TRANSPORT__MCD__HPP
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
#include <transport/mcd.hh>
#include <types/cacheline.hpp>
#include <stdexcept>
#include <thread>
#include <array>
#include <cstring>
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
// OK if used in a multi process system where no process follows another on its own physical core
//#define BARRIER  
//
// OK if used in a multi threaded system where no thread follows another in its own physical core
#define BARRIER asm volatile("": : :"memory")
//
// Needed when nodes follow other nodes that share physical cores
//#define BARRIER __sync_synchronize() // probably stronger than needed
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
template< typename E >
struct transport::mcd< E >::impl
{
  static constexpr int max_node_count{ 8 };
  static constexpr int max_follower_count{ max_node_count };
  static constexpr int max_channel_count{ max_node_count };
  static constexpr int pool_slot_count{ 256 };
  static constexpr int pool_mask{ pool_slot_count - 1 };

  using event_type = E;

  struct data_type
	{
		types::cacheline< data_type* > base_address;

		using meta_type  = types::cacheline< uint64_t >[ max_node_count ][ max_channel_count ];
		meta_type meta_mem;

	  using pool_type = types::cacheline< event_type >[ max_channel_count ][ pool_slot_count ];
	  pool_type pool_mem;
	};

  typename data_type::meta_type &meta_mem;
  typename data_type::pool_type &pool_mem;
  const node_id_type node_id_mem;
  injector_channel_type injector_channel_mem{ 0 };
  unsigned int channel_mem{ 0 };
  unsigned int walked_mem{ 0 };
  alignas( 64 ) uint64_t *followed_ptr_mem[ max_channel_count ][ max_follower_count ];
  alignas( 64 ) uint64_t cursor_mem[ max_channel_count ];
  alignas( 64 ) uint64_t *sequence_ptr_mem[ max_channel_count ];
  alignas( 64 ) int external_channel_mem[ max_channel_count ];
  alignas( 64 ) volatile uint64_t **internal_ptr_mem[ max_channel_count ];
  alignas( 64 ) volatile uint64_t **walker_ptr_mem[ max_channel_count ];

 	impl( data_type*, const node_id_type&, const injector_channel_type&, const follow_list_type& );
  bool aquire_test();
  event_type& aquire();
  void commit();
  bool reaquire_test();
  event_type& reaquire();
  void release();
};
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
template< typename E >
inline
transport::mcd< E >::impl::impl(
  data_type* data_arg, 
  const node_id_type &node_id_arg, 
  const injector_channel_type &injector_channel_arg, 
  const follow_list_type &follow_list_arg 
  )
	: meta_mem( data_arg->meta_mem )
  , pool_mem( data_arg->pool_mem )
  , node_id_mem( node_id_arg )
  , injector_channel_mem( injector_channel_arg )
{

  if( data_arg->base_address.value == 0 ) 
    data_arg->base_address.value = data_arg;

  if( data_arg->base_address.value != data_arg ) 
    throw std::runtime_error( "Shared data not at same base address" );

  if( follow_list_arg.size() > max_follower_count ) 
    throw std::logic_error( "To many followers" );

	std::array< int, max_channel_count > idx;
  idx.fill( 0 );  
	std::memset( followed_ptr_mem, 0, max_channel_count * max_follower_count * sizeof( uint64_t* ) );
  for( auto followed : follow_list_arg )
	{
    auto fnode = followed.first;
    if( fnode && fnode > max_node_count ) 
      throw std::runtime_error( "Invalid follower id" );
    if( fnode == node_id_mem ) 
      throw std::runtime_error( "Node cannot follow itself" );

    auto fchannel = followed.second;
    if( fchannel && fchannel > max_channel_count ) 
      throw std::runtime_error( "Invalid channel id" );

    followed_ptr_mem[ fchannel ][ idx[ fchannel ] ] = &meta_mem[ fnode ][ fchannel ].value;
    ++idx[ fchannel ];
	}

  std::memset( cursor_mem, 0, max_channel_count * sizeof( cursor_mem[ 0 ] ) );
  std::memset( sequence_ptr_mem, 0, max_channel_count * sizeof( sequence_ptr_mem[ 0 ] ) );
  std::memset( external_channel_mem, 0, max_channel_count * sizeof( external_channel_mem[ 0 ] ) );
  std::memset( internal_ptr_mem, 0, max_channel_count * sizeof( internal_ptr_mem[ 0 ] ) );
  std::memset( walker_ptr_mem, 0, max_channel_count * sizeof( walker_ptr_mem[ 0 ] ) );
  for( int c = 0; c < max_channel_count ; ++c )
	{
    if( followed_ptr_mem[ c ][ 0 ] )
		{
      if( injector_channel_mem == c ) injector_channel_mem = walked_mem;
      sequence_ptr_mem[ walked_mem ] = &meta_mem[ node_id_mem ][ c ].value;
      cursor_mem[ walked_mem ] = ( meta_mem[ node_id_mem ][ c ].value & pool_mask );
      internal_ptr_mem[ walked_mem ] = const_cast< volatile uint64_t** >( followed_ptr_mem[ c ] );
			external_channel_mem[ walked_mem ] = c;
      ++walked_mem;
		}
	}
}
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
template< typename E >
inline bool
transport::mcd< E >::impl::aquire_test()
{
	std::memcpy( walker_ptr_mem, internal_ptr_mem, sizeof( walker_ptr_mem[0] ) * walked_mem );
  for( int channel = 0 ; channel < walked_mem ;  )
  {
    if( ( cursor_mem[ channel ] + 1 & pool_mask ) == ( **walker_ptr_mem[ channel ] & pool_mask ) ) ++channel;
    else if( *++walker_ptr_mem[ channel ] == 0 ) return true;
  }

  return false;
}
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
template< typename E >
inline
E&
transport::mcd< E >::impl::aquire()
{
	std::memcpy( walker_ptr_mem, internal_ptr_mem, sizeof( walker_ptr_mem[0] ) * walked_mem );
  do
  {
    do
    {
      if( ++channel_mem == walked_mem ) channel_mem = 0;
    }
    while( ( ( cursor_mem[ channel_mem ] + 1 ) & pool_mask ) == ( **walker_ptr_mem[ channel_mem ] & pool_mask ) );
  }
  while( *++walker_ptr_mem[ channel_mem ] );

  return pool_mem[ external_channel_mem[ injector_channel_mem ] ][ cursor_mem[ injector_channel_mem ] ].value;
}
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
template< typename E >
inline
void 
transport::mcd< E >::impl::commit()
{
  BARRIER;
  cursor_mem[ channel_mem ] = ( ++( *sequence_ptr_mem[ channel_mem ] ) & pool_mask );
}
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
template< typename E >
inline
bool
transport::mcd< E >::impl::reaquire_test()
{
	std::memcpy( walker_ptr_mem, internal_ptr_mem, sizeof( walker_ptr_mem[0] ) * walked_mem );
  for( int channel = 0 ; channel < walked_mem ;  )
  {
    if( cursor_mem[ channel ] == ( **walker_ptr_mem[ channel ] & pool_mask ) ) ++channel;
    else if( *++walker_ptr_mem[ channel ] == 0 ) return true;
  }

  return false;
}
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
template< typename E >
inline
E&
transport::mcd< E >::impl::reaquire()
{
	std::memcpy( walker_ptr_mem, internal_ptr_mem, sizeof( walker_ptr_mem[0] ) * walked_mem );
  do
  {
    do
    {
      if( ++channel_mem == walked_mem ) channel_mem = 0;
    }
    while( cursor_mem[ channel_mem ] == ( **walker_ptr_mem[ channel_mem ] & pool_mask ) );
  }
  while( *++walker_ptr_mem[ channel_mem ] );

  return pool_mem[ external_channel_mem[ channel_mem ] ][ cursor_mem[ channel_mem ] ].value;
}
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
template< typename E >
inline
void
transport::mcd< E >::impl::release()
{
  BARRIER;
  cursor_mem[ channel_mem ] = ( ++( *sequence_ptr_mem[ channel_mem ] ) & pool_mask );
}
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
template< typename E >
inline
transport::mcd< E >::mcd( 
  void* data_arg, 
  const node_id_type &node_id_arg, 
  const follow_list_type &follow_list_arg 
  )
  : pimpl( 
      new impl( 
  			reinterpret_cast< typename impl::data_type* >( data_arg ), 
        node_id_arg, 
        injector_channel_type( -1 ), 
        follow_list_arg 
        )
      )
{
	return;
}
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
template< typename E >
inline
transport::mcd< E >::mcd( 
  void* data_arg, 
  const node_id_type &node_id_arg, 
  const injector_channel_type &injector_channel_arg, 
  const follow_list_type &follow_list_arg 
  )
  : pimpl( 
      new impl( 
  			reinterpret_cast< typename impl::data_type* >( data_arg ), 
        node_id_arg, 
        injector_channel_arg, 
        follow_list_arg 
        )
      )
{
	return;
}
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
template< typename E >
inline
transport::mcd< E >::~mcd()
{
  delete pimpl;
}
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
template< typename E >
inline
bool
transport::mcd< E >::aquire_test()
{
  return pimpl->aquire_test();
}
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
template< typename E >
inline
E&
transport::mcd< E >::aquire()
{
  return pimpl->aquire();
}
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
template< typename E >
inline
void 
transport::mcd< E >::commit()
{
  return pimpl->commit();
}
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
template< typename E >
inline
bool
transport::mcd< E >::reaquire_test()
{
  return pimpl->reaquire_test();
}
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
template< typename E >
inline
E&
transport::mcd< E >::reaquire()
{
  return pimpl->reaquire();
}
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
template< typename E >
inline
void 
transport::mcd< E >::release()
{
  return pimpl->release();
}
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
template< typename E >
inline
std::string
toString(
  const transport::mcd< E >& processor
  )
{ 
  throw std::runtime_error( "Unimplemented" );
  return ""; 
}
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
#endif // GUARD__TRANSPORT__MCD__HPP
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
