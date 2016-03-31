#include "Benchmark.hpp"
#include "Configuration.hpp"
#include <string>
#include <unordered_map>
#include <utility>
#include <sstream>

struct Quix::Benchmark::Impl
{
  Impl( Configuration&& configuration );
  Configuration configuration;
};

Quix::Benchmark::Impl::Impl(
  Configuration&& other
  ) :
  configuration( std::move( other ) )
{
  return;
}

Quix::Benchmark::Benchmark(
  Configuration&& configuration
  )
  : pimpl( new Impl( std::move( configuration ) ) )
{
  return;
}

Quix::Benchmark::Benchmark(
  const Benchmark& other
  ) 
  : pimpl( new Impl( *other.pimpl ) )
{
  return;
}

Quix::Benchmark::Benchmark(
  Benchmark&& other
  ) : pimpl( other.pimpl )
{
  other.pimpl = nullptr;
}

Quix::Benchmark&
Quix::Benchmark::operator=(
  Benchmark other
  )
{
  std::swap( pimpl, other.pimpl );
  return *this;
}

Quix::Benchmark&
Quix::Benchmark::operator=(
  Benchmark&& other
  )
{
  std::swap( pimpl, other.pimpl );
  return *this;
}

Quix::Benchmark::~Benchmark()
{
  delete pimpl;
}

std::string
Quix::toString(
  const Benchmark& processor
  )
{ 
}

