//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
#ifndef GUARD__BEHAVIOUR__EVENT_REPUBLISH__HPP
#define GUARD__BEHAVIOUR__EVENT_REPUBLISH__HPP
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
#include <behaviour/event_republish.hh>
#include <stdexcept>
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
template< typename E >
struct behaviour::event_republish< E >::impl
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
behaviour::event_republish< E >::impl::impl(

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
behaviour::event_republish< E >::impl::operator()(
  event_type &event 
  )
{
  if( event != loops_mem ) throw std::runtime_error( "Loops count mismatch" );
}
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
template< typename E >
bool
behaviour::event_republish< E >::impl::post()
{
  return --loops_mem;
}
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
template< typename E >
behaviour::event_republish< E >::event_republish(
  void*,
  const unsigned int loops
  )
  : pimpl( new impl( loops ) )
{
  return;
}
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
template< typename E >
behaviour::event_republish< E >::~event_republish()
{
  delete pimpl;
}
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
template< typename E >
void
behaviour::event_republish< E >::operator()( 
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
behaviour::event_republish< E >::post()
{
  return pimpl->post();
}
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
template< typename E >
std::string
toString(
  const behaviour::event_republish< E >& event_republish_arg
  )
{ 
}
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
#endif // GUARD__BEHAVIOUR__EVENT_REPUBLISH__HPP
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
