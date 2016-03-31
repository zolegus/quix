#include "Entry.hpp"
#include "Configuration.hpp"
#include <string>
#include <unordered_map>
#include <utility>
#include <sstream>

struct Quix::Entry::Impl
{
  Impl( Configuration&& configuration );
  Configuration configuration;
};

Quix::Entry::Impl::Impl(
  Configuration&& other
  ) :
  configuration( std::move( other ) )
{
  return;
}

Quix::Entry::Entry(
  Configuration&& configuration
  )
  : pimpl( new Impl( std::move( configuration ) ) )
{
  return;
}

Quix::Entry::Entry(
  const Entry& other
  ) 
  : pimpl( new Impl( *other.pimpl ) )
{
  return;
}

Quix::Entry::Entry(
  Entry&& other
  ) : pimpl( other.pimpl )
{
  other.pimpl = nullptr;
}

Quix::Entry&
Quix::Entry::operator=(
  Entry other
  )
{
  std::swap( pimpl, other.pimpl );
  return *this;
}

Quix::Entry&
Quix::Entry::operator=(
  Entry&& other
  )
{
  std::swap( pimpl, other.pimpl );
  return *this;
}

Quix::Entry::~Entry()
{
  delete pimpl;
}

std::string
Quix::toString(
  const Entry& processor
  )
{ 
}

