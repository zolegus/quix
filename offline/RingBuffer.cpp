#include "RingBuffer.hpp"
#include "Configuration.hpp"
#include <string>
#include <unordered_map>
#include <utility>
#include <sstream>

struct Quix::RingBuffer::Impl
{
  Impl( Configuration&& configuration );
  Configuration configuration;
};

Quix::RingBuffer::Impl::Impl(
  Configuration&& other
  ) :
  configuration( std::move( other ) )
{
  return;
}

Quix::RingBuffer::RingBuffer(
  Configuration&& configuration
  )
  : pimpl( new Impl( std::move( configuration ) ) )
{
  return;
}

Quix::RingBuffer::RingBuffer(
  const RingBuffer& other
  ) 
  : pimpl( new Impl( *other.pimpl ) )
{
  return;
}

Quix::RingBuffer::RingBuffer(
  RingBuffer&& other
  ) : pimpl( other.pimpl )
{
  other.pimpl = nullptr;
}

Quix::RingBuffer&
Quix::RingBuffer::operator=(
  RingBuffer other
  )
{
  std::swap( pimpl, other.pimpl );
  return *this;
}

Quix::RingBuffer&
Quix::RingBuffer::operator=(
  RingBuffer&& other
  )
{
  std::swap( pimpl, other.pimpl );
  return *this;
}

Quix::RingBuffer::~RingBuffer()
{
  delete pimpl;
}

std::string
Quix::toString(
  const RingBuffer& processor
  )
{ 
}

