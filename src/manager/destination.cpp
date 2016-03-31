//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
#include <manager/destination.hpp>
#include <utility>
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
struct manager::destination::impl
{
};
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
manager::destination::destination()
  : pimpl( new impl )
{
  return;
}
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
manager::destination::destination(
  const manager::destination& other
  ) 
  : pimpl( new impl( *other.pimpl ) )
{
  return;
}
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
manager::destination::destination(
  manager::destination&& other
  ) : pimpl( other.pimpl )
{
  other.pimpl = nullptr;
}
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
manager::destination&
manager::destination::operator=(
  manager::destination other
  )
{
  std::swap( pimpl, other.pimpl );
  return *this;
}
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
manager::destination&
manager::destination::operator=(
  manager::destination&& other
  )
{
  std::swap( pimpl, other.pimpl );
  return *this;
}
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
manager::destination::~destination()
{
  delete pimpl;
}
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
std::string
toString(
  const manager::destination& processor
  )
{ 
}
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
