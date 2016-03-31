#include "Topology.hpp"
#include "Configuration.hpp"
#include <string>
#include <unordered_map>
#include <utility>
#include <sstream>

struct Quix::Topology::Impl
{
  Impl( Configuration&& configuration );
  Configuration configuration;
};

Quix::Topology::Impl::Impl(
  Configuration&& other
  ) :
  configuration( std::move( other ) )
{
  return;
}

Quix::Topology::Topology(
  Configuration&& configuration
  )
  : pimpl( new Impl( std::move( configuration ) ) )
{
  return;
}

Quix::Topology::Topology(
  const Topology& other
  ) 
  : pimpl( new Impl( *other.pimpl ) )
{
  return;
}

Quix::Topology::Topology(
  Topology&& other
  ) : pimpl( other.pimpl )
{
  other.pimpl = nullptr;
}

Quix::Topology&
Quix::Topology::operator=(
  Topology other
  )
{
  std::swap( pimpl, other.pimpl );
  return *this;
}

Quix::Topology&
Quix::Topology::operator=(
  Topology&& other
  )
{
  std::swap( pimpl, other.pimpl );
  return *this;
}

Quix::Topology::~Topology()
{
  delete pimpl;
}

std::string
Quix::toString(
  const Topology& processor
  )
{ 
}

