//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
#ifndef GUARD__TRANSPORT__SLOT__HPP
#define GUARD__TRANSPORT__SLOT__HPP
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
#include <transport/slot.hh>
#include <stdexcept>
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
template< typename T, bool C >
transport::slot< T, C >::slot( 
	transport_type &transport_arg,
  event_type &value_arg
  ) 
	: transport_mem( transport_arg )
  , value_mem( value_arg )
{
  return;
}
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
template< typename T, bool C >
transport::slot< T, C >::~slot()
{
  if( C ) 
    transport_mem.commit();
	else 
    transport_mem.release();
}
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
template< typename T, bool C >
auto
transport::slot< T, C >::operator*() -> event_type&
{
  return value_mem;
}
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
template< typename T, bool C >
auto
transport::slot< T, C >::operator->() -> event_type*
{
  return &value_mem;
}
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
template< typename T >
auto
transport::make_aquire_commit_slot( T &transport_arg )
{
  auto& value = transport_arg.aquire();
  return typename slot< T >::commit_slot( transport_arg, value );
}
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
template< typename T >
auto
transport::make_reaquire_commit_slot( T &transport_arg )
{
  auto& value = transport_arg.reaquire();
  return typename slot< T >::commit_slot( transport_arg, value );
}
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
template< typename T >
auto
transport::make_reaquire_release_slot( T &transport_arg )
{
  auto& value = transport_arg.reaquire();
  return typename slot< T >::release_slot( transport_arg, value );
}
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
template< typename T, bool C >
std::string
toString(
	const transport::slot< T, C >&
  )
{ 
  throw std::runtime_error( "Unimplemented" );
  return ""; 
}
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
#endif // GUARD__TRANSPORT__SLOT__HPP
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
