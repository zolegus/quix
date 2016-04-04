//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
#ifndef GUARD__TRANSPORT__LWD__HPP
#define GUARD__TRANSPORT__LWD__HPP
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
#include <transport/lwd.hh>
#include <types/cacheline.hpp>
#include <stdexcept>
#include <thread>
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
struct transport::lwd< E >::impl
{
  static constexpr int max_node_count{ 8 };
  static constexpr int max_follower_count{ max_node_count };
  static constexpr int pool_slot_count{ 256 };
  static constexpr int pool_mask{ pool_slot_count - 1 };

  using node_id_type = unsigned int;
  using follow_list_type = std::vector< unsigned int >;
  using event_type = E;

  struct data_type
	{
		types::cacheline< data_type* > base_address;

		using meta_type  = types::cacheline< uint64_t >[ max_node_count ];
		meta_type meta_mem;

	  using pool_type = types::cacheline< event_type >[ pool_slot_count ];
	  pool_type pool_mem;
	};

  typename data_type::meta_type &meta_mem;
  typename data_type::pool_type &pool_mem;
  const node_id_type node_id_mem;
  uint64_t *consumed_value_ptr[ max_follower_count ];

 	impl( data_type*, const node_id_type&, const follow_list_type& );
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
transport::lwd< E >::impl::impl(
  data_type* data_arg, 
  const node_id_type &node_id_arg, 
  const follow_list_type &follow_list_arg 
  )
	: meta_mem( data_arg->meta_mem )
  , pool_mem( data_arg->pool_mem )
  , node_id_mem( node_id_arg )
{
  if( data_arg->base_address.value == 0 ) 
    data_arg->base_address.value = data_arg;

  if( data_arg->base_address.value != data_arg ) 
    throw std::runtime_error( "Not all transport nodes using memory at same base address" );

  if( follow_list_arg.size() >= max_follower_count ) 
    throw std::runtime_error( "To many followers" );

  int i = 0;
  for( auto followed_node : follow_list_arg )
	{
    if( followed_node && followed_node > max_node_count || followed_node == node_id_mem )
      throw std::runtime_error( "Invalid follower id" );

    consumed_value_ptr[ i++ ] = &meta_mem[ followed_node ].value;
	}
  consumed_value_ptr[ i++ ] = 0;
}
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
template< typename E >
inline
bool
transport::lwd< E >::impl::aquire_test()
{
  register uint64_t next_slot = meta_mem[ node_id_mem ] + 1  & pool_mask ;
  uint64_t *volatile *walker = ( uint64_t *volatile * )consumed_value_ptr;

  do
	{
    if( next_slot == ( ( **walker ) & pool_mask ) ) return false;
	}
  while( *( ++walker ) );

  return true;
}
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
#include <thread>
#include <chrono>
template< typename E >
inline
E&
transport::lwd< E >::impl::aquire()
{
  register uint64_t next_slot = meta_mem[ node_id_mem ].value + 1 & pool_mask;
  uint64_t *volatile *walker = ( uint64_t *volatile * )consumed_value_ptr;

  do
	{
    while( next_slot == ( ( **walker ) & pool_mask ) )
  		std::this_thread::yield();
 	}
  while( *( ++walker ) );

  register uint64_t node_slot = meta_mem[ node_id_mem ].value & pool_mask;
  return pool_mem[ node_slot ].value;
}
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
template< typename E >
inline
void 
transport::lwd< E >::impl::commit()
{
  BARRIER;
  ++meta_mem[ node_id_mem ].value;
}
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
template< typename E >
inline
bool
transport::lwd< E >::impl::reaquire_test()
{
  register uint64_t node_slot = meta_mem[ node_id_mem ].value & pool_mask;
  uint64_t *volatile *walker = ( uint64_t *volatile * )consumed_value_ptr;

  do
	{
    if( node_slot == ( ( **walker ) & pool_mask ) ) return false;
  }
	while( *( ++walker ) );

 	return true;
}
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
template< typename E >
inline
E&
transport::lwd< E >::impl::reaquire()
{
  register uint64_t node_slot = meta_mem[ node_id_mem ].value & pool_mask;
  uint64_t *volatile  *walker = ( uint64_t *volatile * )consumed_value_ptr;

  do
	{
    while( node_slot == ( ( **walker ) & pool_mask ) )
  		std::this_thread::yield();
  }
	while( *( ++walker ) );

  return pool_mem[ node_slot ].value;
}
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
template< typename E >
inline
void 
transport::lwd< E >::impl::release()
{
  BARRIER;
  ++meta_mem[ node_id_mem ].value;
}
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
template< typename E >
inline
transport::lwd< E >::lwd( 
  void* data_arg, 
  const node_id_type &node_id_arg, 
  const follow_list_type &follow_list_arg 
  )
  : pimpl( 
      new impl( 
  			reinterpret_cast< typename impl::data_type* >( data_arg ), 
        node_id_arg, 
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
transport::lwd< E >::~lwd()
{
  delete pimpl;
}
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
template< typename E >
inline
bool
transport::lwd< E >::aquire_test()
{
  return pimpl->aquire_test();
}
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
template< typename E >
inline
E&
transport::lwd< E >::aquire()
{
  return pimpl->aquire();
}
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
template< typename E >
inline
void 
transport::lwd< E >::commit()
{
  return pimpl->commit();
}
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
template< typename E >
inline
bool
transport::lwd< E >::reaquire_test()
{
  return pimpl->reaquire_test();
}
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
template< typename E >
inline
E&
transport::lwd< E >::reaquire()
{
  return pimpl->reaquire();
}
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
template< typename E >
inline
void 
transport::lwd< E >::release()
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
  const transport::lwd< E >& processor
  )
{ 
  throw std::runtime_error( "Unimplemented" );
  return ""; 
}
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
#endif // GUARD__TRANSPORT__LWD__HPP
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
