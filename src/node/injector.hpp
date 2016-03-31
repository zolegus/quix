//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
#ifndef GUARD__NODE__INJECTOR__HPP
#define GUARD__NODE__INJECTOR__HPP
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
#include <node/injector.hh>
#include <utility/configuration.hh>
#include <transport/slot.hpp> // TODO: because of auto deduction - refactor out
#include <iostream>
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
template< typename T, typename B >
struct node::injector< T, B >::impl
{
  using transport_type = T;
  using behaviour_type = B;

  transport_type &transport_mem;
  behaviour_type &behaviour_mem;

  impl( transport_type&, behaviour_type& );
  void operator()();
};
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
template< typename T, typename B >
node::injector< T, B >::impl::impl(
  transport_type &transport_arg,
  behaviour_type &behaviour_arg 
  )
  : transport_mem( transport_arg )
  , behaviour_mem( behaviour_arg )
{
  return;
}
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
template< typename T, typename B >
void
node::injector< T, B >::impl::operator()()
{
  std::cout << "Entering node loop" << std::endl;
  do
	{
    auto slot = transport::make_aquire_commit_slot( transport_mem );
    behaviour_mem( *slot );
	}
  while( behaviour_mem.post() );
}
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
template< typename T, typename B >
node::injector< T, B >::injector(
  transport_type &transport_arg,
  behaviour_type &behaviour_arg 
  )
  : pimpl( new impl( transport_arg, behaviour_arg ) )
{
  return;
}
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
template< typename T, typename B >
node::injector< T, B >::~injector()
{
  delete pimpl;
}
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
template< typename T, typename B >
void node::injector< T, B >::operator()()
{
  return pimpl->operator()();
}
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
template< typename T, typename B >
std::string
toString(
const node::injector< T, B >& injector
  )
{ 
}
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
#endif // GUARD__NODE__INJECTOR__HPP
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
