//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
#include <behaviour/client_manager.hpp>
#include <stdexcept>
#include <utility>
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
struct manager::client_manager::impl
{
};
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
manager::client_manager::client_manager()
  : pimpl( new impl )
{
  return;
}
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
manager::client_manager::client_manager(
  const manager::client_manager& other
  ) 
  : pimpl( new impl( *other.pimpl ) )
{
  return;
}
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
manager::client_manager::client_manager(
  manager::client_manager&& other
  ) : pimpl( other.pimpl )
{
  other.pimpl = nullptr;
}
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
manager::client_manager&
manager::client_manager::operator=(
  manager::client_manager other
  )
{
  std::swap( pimpl, other.pimpl );
  return *this;
}
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
manager::client_manager&
manager::client_manager::operator=(
  manager::client_manager&& other
  )
{
  std::swap( pimpl, other.pimpl );
  return *this;
}
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
manager::client_manager::~client_manager()
{
  delete pimpl;
}
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
std::string
toString(
  const manager::client_manager& processor
  )
{ 
  throw std::runtime_error( "Unimplemented" );
  return ""; 
}
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
