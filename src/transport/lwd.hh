//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
#ifndef GUARD__TRANSPORT__LWD__HH
#define GUARD__TRANSPORT__LWD__HH
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
#include <transport/lwd.h>
#include <transport/slot.h>
#include <vector>
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
template< typename E > 
class transport::lwd
{
  static_assert( std::is_pod< E >::value, "Event type must be POD" );

  struct impl;
  impl *pimpl;

public:

  using node_id_type = unsigned int;
  using follow_list_type = std::vector< unsigned int >;
  using event_type = E;

  lwd( void*, const node_id_type&, const follow_list_type& );
  lwd( const lwd& ) = delete;
  lwd( lwd&& ) = delete;
  lwd& operator=( lwd ) = delete;
  lwd& operator=( lwd&& ) = delete;
  ~lwd();

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
#endif // GUARD__TRANSPORT__LWD__HH
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//

