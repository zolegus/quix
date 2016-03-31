//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
#ifndef GUARD__DUAL_TRANSPORT__HH
#define GUARD__DUAL_TRANSPORT__HH
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
#include <transport/dual.h>
#include <transport/slot.h>
#include <vector>
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
template< typename E > 
class transport::dual
{
  static_assert( std::is_pod< E >::value, "Event type must be POD" );

  struct impl;
  impl *pimpl;

public:

  using node_id_type = unsigned int;
  using follow_list_type = std::vector< unsigned int >;
  using event_type = E;

  dual( void*, const node_id_type&, const follow_list_type& );
  dual( const dual& ) = delete;
  dual( dual&& ) = delete;
  dual& operator=( dual ) = delete;
  dual& operator=( dual&& ) = delete;
  ~dual();

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
#endif // GUARD__DUAL_TRANSPORT__HH
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//

