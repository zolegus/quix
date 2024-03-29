//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
#ifndef GUARD__TRANSPORT__SLOT__HH
#define GUARD__TRANSPORT__SLOT__HH
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
#include <transport/slot.h>
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
template< typename T, bool C = true >
class transport::slot
{
  using transport_type = T;
  using event_type = typename T::event_type;

  transport_type &transport_mem;
  event_type &value_mem;

public:

  using commit_slot = slot< transport_type, true >;
  using release_slot = slot< transport_type, false >;

  slot( transport_type &transport_arg, event_type &value_arg );
  slot( const slot& ) = delete;
  slot( slot&& ) = default;
  slot& operator=( slot ) = delete;
  slot& operator=( slot&& ) = default;
  ~slot();

  event_type &operator*();
  event_type *operator->();

};
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
#endif // GUARD__TRANSPORT__SLOT__HH
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
