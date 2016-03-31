//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
#ifndef GUARD__TRANSPORT__MCD__HH
#define GUARD__TRANSPORT__MCD__HH
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
#include <transport/mcd.h>
#include <transport/slot.h>
#include <vector>
#include <utility>
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
template< typename E > 
class transport::mcd
{
  static_assert( std::is_pod< E >::value, "Event type must be POD" );

  struct impl;
  impl *pimpl;

public:

  using node_id_type = unsigned int;
  using injector_channel_type = unsigned int;
  using follow_list_type = std::vector< std::pair< unsigned int, unsigned int > >;
  using event_type = E;

  mcd( void*, const node_id_type&, const follow_list_type& );
  mcd( void*, const node_id_type&, const injector_channel_type&, const follow_list_type& );
  mcd( const mcd& ) = delete;
  mcd( mcd&& ) = delete;
  mcd& operator=( mcd ) = delete;
  mcd& operator=( mcd&& ) = delete;
  ~mcd();

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
#endif // GUARD__TRANSPORT__MCD__HH
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
