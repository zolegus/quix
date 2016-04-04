//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
#ifndef GUARD__TRANSPORT__DUAL__HPP
#define GUARD__TRANSPORT__DUAL__HPP
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
#include <transport/dual.hh>
#include <types/cacheline.hpp>
#include <stdexcept>
//
////////////////////////////////////////////////////////////////////////////////
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
////////////////////////////////////////////////////////////////////////////////
//
template< typename E >
struct transport::dual< E >::impl
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

		using meta_type  = types::cacheline< event_type* >[ max_node_count ][2];
		meta_type meta_mem;

	  using pool_type = types::cacheline< event_type >[ pool_slot_count ];
	  pool_type pool_mem;
  };

  typename data_type::meta_type &meta_mem;
  typename data_type::pool_type &pool_mem;
  const node_id_type node_id_mem;
  unsigned int pool_index{ 0 };
  unsigned int tt{ 0 };
  event_type *pending_value_ptr{ nullptr };
  event_type *volatile *consumed_value_ptr[ 2 ];

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
transport::dual< E >::impl::impl(
  data_type* data_arg, 
  const node_id_type& node_id_arg, 
  const follow_list_type& follow_list_arg 
  )
	: meta_mem( data_arg->meta_mem )
  , pool_mem( data_arg->pool_mem )
  , node_id_mem( node_id_arg )
{
  if( data_arg->base_address.value == 0 ) 
    data_arg->base_address.value = data_arg;

  if( data_arg->base_address.value != data_arg ) 
    throw std::runtime_error( "Not all transport nodes using memory at same base address" );

  if( follow_list_arg.size() > max_follower_count ) 
    throw std::logic_error( "To many followers" );

  int f = *follow_list_arg.begin();
  consumed_value_ptr[0] = &meta_mem[ f ][ 0 ].value;
  consumed_value_ptr[1] = &meta_mem[ f ][ 1 ].value;
}
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
template< typename E >
inline
bool
transport::dual< E >::impl::aquire_test()
{
  if( pending_value_ptr ) 
    throw std::logic_error( "aquire_test called out of sequence" );

  return meta_mem[ node_id_mem ][ tt ].value ? false : true;
}
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
template< typename E >
inline
E&
transport::dual< E >::impl::aquire()
{
  if( pending_value_ptr ) 
    throw std::logic_error( "aquire called out of sequence" );

  while( aquire_test() == false );
  pending_value_ptr = &pool_mem[ ++pool_index & pool_mask ].value;
  return *pending_value_ptr;
}
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
template< typename E >
inline
void 
transport::dual< E >::impl::commit()
{
  if( !pending_value_ptr ) throw std::logic_error( "commit called out of sequence" );

  while( meta_mem[ node_id_mem ][ tt ].value );
  BARRIER;
  meta_mem[ node_id_mem ][ tt ].value = pending_value_ptr;
  pending_value_ptr = nullptr;
  tt = !tt;
}
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
template< typename E >
inline
bool
transport::dual< E >::impl::reaquire_test()
{
  if( pending_value_ptr ) 
    throw std::logic_error( "reaquire_test called out of sequence" );

  return *consumed_value_ptr[ tt ] ? true : false;
}
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
template< typename E >
inline
E&
transport::dual< E >::impl::reaquire()
{
  if( pending_value_ptr ) 
    throw std::logic_error( "reaquire called out of sequence" );

  while( reaquire_test() == false );
  pending_value_ptr = *consumed_value_ptr[ tt ];
  *consumed_value_ptr[ tt ] = 0;
  return *pending_value_ptr;
}
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
template< typename E >
inline
void 
transport::dual< E >::impl::release()
{
  if( !pending_value_ptr ) 
   throw std::logic_error( "release called out of sequence" );

  pending_value_ptr = nullptr;
  tt = !tt;
}
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
template< typename E >
inline
transport::dual< E >::dual( 
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
transport::dual< E >::~dual()
{
  delete pimpl;
}
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
template< typename E >
inline
bool 
transport::dual< E >::aquire_test()
{
  return pimpl->aquire_test();
}
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
template< typename E >
inline
E& 
transport::dual< E >::aquire()
{
  return pimpl->aquire();
}
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
template< typename E >
inline
void 
transport::dual< E >::commit()
{
  return pimpl->commit();
}
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
template< typename E >
inline
bool 
transport::dual< E >::reaquire_test()
{
  return pimpl->reaquire_test();
}
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
template< typename E >
inline
E& 
transport::dual< E >::reaquire()
{
  return pimpl->reaquire();
}
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
template< typename E >
inline
void 
transport::dual< E >::release()
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
  const transport::dual< E >& processor
  )
{ 
  throw std::runtime_error( "Unimplemented" );
  return ""; 
}
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
#endif // GUARD__TRANSPORT__DUAL__HPP
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
