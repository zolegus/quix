//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
#include <manager/client.hpp>
#include <stdexcept>
#include <utility>
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
struct manager::client::impl
{
};
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
manager::client::client()
  : pimpl( new impl )
{
  return;
}
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
manager::client::client(
  const manager::client& other
  ) 
  : pimpl( new impl( *other.pimpl ) )
{
  return;
}
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
manager::client::client(
  manager::client&& other
  ) : pimpl( other.pimpl )
{
  other.pimpl = nullptr;
}
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
manager::client&
manager::client::operator=(
  manager::client other
  )
{
  std::swap( pimpl, other.pimpl );
  return *this;
}
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
manager::client&
manager::client::operator=(
  manager::client&& other
  )
{
  std::swap( pimpl, other.pimpl );
  return *this;
}
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
manager::client::~client()
{
  delete pimpl;
}
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
std::string
toString(
  const manager::client& processor
  )
{ 
  throw std::runtime_error( "Unimplemented" );
  return ""; 
}
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//