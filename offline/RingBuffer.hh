#ifndef GUARD__PROCESSOR__HH
#define GUARD__PROCESSOR__HH

#include "RingBuffer.h"
#include "Configuration.h"

class Quix::RingBuffer
{
  struct Impl;
  Impl *pimpl;

public:

  RingBuffer( Configuration&& );
  RingBuffer( const RingBuffer& );
  RingBuffer( RingBuffer&& );
  RingBuffer& operator=( RingBuffer );
  RingBuffer& operator=( RingBuffer&& );
  ~RingBuffer();
};

#endif // GUARD__PROCESSOR__HH
