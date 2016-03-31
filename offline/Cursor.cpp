#include "Cursor.hpp"
#include "Configuration.hpp"
#include <string>
#include <unordered_map>
#include <utility>
#include <sstream>

struct Quix::Cursor::Impl
{
  Impl( Configuration&& configuration );
  Configuration configuration;
};

Quix::Cursor::Impl::Impl(
  Configuration&& other
  ) :
  configuration( std::move( other ) )
{
  return;
}

Quix::Cursor::Cursor(
  Configuration&& configuration
  )
  : pimpl( new Impl( std::move( configuration ) ) )
{
  return;
}

Quix::Cursor::Cursor(
  const Cursor& other
  ) 
  : pimpl( new Impl( *other.pimpl ) )
{
  return;
}

Quix::Cursor::Cursor(
  Cursor&& other
  ) : pimpl( other.pimpl )
{
  other.pimpl = nullptr;
}

Quix::Cursor&
Quix::Cursor::operator=(
  Cursor other
  )
{
  std::swap( pimpl, other.pimpl );
  return *this;
}

Quix::Cursor&
Quix::Cursor::operator=(
  Cursor&& other
  )
{
  std::swap( pimpl, other.pimpl );
  return *this;
}

Quix::Cursor::~Cursor()
{
  delete pimpl;
}

std::string
Quix::toString(
  const Cursor& processor
  )
{ 
}

