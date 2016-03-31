#ifndef GUARD__PROCESSOR__HH
#define GUARD__PROCESSOR__HH

#include "Benchmark.h"
#include "Configuration.h"

class Quix::Benchmark
{
  struct Impl;
  Impl *pimpl;

public:

  Benchmark( Configuration&& );
  Benchmark( const Benchmark& );
  Benchmark( Benchmark&& );
  Benchmark& operator=( Benchmark );
  Benchmark& operator=( Benchmark&& );
  ~Benchmark();
};

#endif // GUARD__PROCESSOR__HH
