//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
#ifndef GUARD__MEMORY__LOCAL__HH
#define GUARD__MEMORY__LOCAL__HH
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
#include <memory/local.h>
#include <cstddef>
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
class memory::local
{
  struct impl;
  impl *pimpl;

public:

  local( const std::size_t );
  local( const local& );
  local( local&& );
  local& operator=( local );
  local& operator=( local&& );
  ~local();

  void *data( const std::size_t = 0 );
	std::size_t size( const std::size_t = 0 );
};
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
#endif // GUARD__MEMORY__LOCAL__HH
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
