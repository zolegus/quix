//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
#ifndef GUARD__TRANSPORT__UNI__HH
#define GUARD__TRANSPORT__UNI__HH
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
#include <transport/uni.h>
#include <transport/slot.h>
#include <vector>
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
template< typename E > 
class transport::uni
{
  static_assert( std::is_pod< E >::value, "Event type must be POD" );

  struct impl;
  impl *pimpl;

public:

  using node_id_type = unsigned int;
  using follow_list_type = std::vector< unsigned int >;
  using event_type = E;

  uni( void*, const node_id_type&, const follow_list_type& );
  uni( const uni& ) = delete;
  uni( uni&& ) = delete;
  uni& operator=( uni ) = delete;
  uni& operator=( uni&& ) = delete;
  ~uni();

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
#endif // GUARD__TRANSPORT__UNI__HH
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//

