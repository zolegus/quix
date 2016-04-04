//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
#ifndef GUARD__BEHAVIOUR__EVENT_CONSUME__HPP
#define GUARD__BEHAVIOUR__EVENT_CONSUME__HPP
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
#include <behaviour/event_consume.hh>
#include <stdexcept>
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
template< typename E >
struct behaviour::event_consume< E >::impl
{
  using event_type = E;

  unsigned int loops_mem{0};

  impl( const unsigned int );
  void operator()( event_type& );
  bool post();
};
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
template< typename E >
behaviour::event_consume< E >::impl::impl(
  const unsigned int loops_arg 
  )
  : loops_mem( loops_arg )
{
  return;
}
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
template< typename E >
void
behaviour::event_consume< E >::impl::operator()(
  event_type &event 
  )
{
  if( event != loops_mem ) throw std::runtime_error( "Loops count mismatch" );
  event = 0;
}
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
template< typename E >
bool
behaviour::event_consume< E >::impl::post()
{
  return --loops_mem;
}
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
template< typename E >
behaviour::event_consume< E >::event_consume(
  void*,
  const unsigned int loops_arg
  )
  : pimpl( new impl( loops_arg ) )
{
  return;
}
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
template< typename E >
behaviour::event_consume< E >::~event_consume()
{
  delete pimpl;
}
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
template< typename E >
void
behaviour::event_consume< E >::operator()(
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
behaviour::event_consume< E >::post()
{
  return pimpl->post();
}
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
template< typename E >
std::string
toString(
  const behaviour::event_consume< E >& event_consume_arg
  )
{ 
}
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
#endif // GUARD__BEHAVIOUR__EVENT_CONSUME__HPP
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//