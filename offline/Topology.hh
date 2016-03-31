#ifndef GUARD__PROCESSOR__HH
#define GUARD__PROCESSOR__HH

#include "Topology.h"
#include "Configuration.h"

class Quix::Topology
{
  struct Impl;
  Impl *pimpl;

public:

  Topology( Configuration&& );
  Topology( const Topology& );
  Topology( Topology&& );
  Topology& operator=( Topology );
  Topology& operator=( Topology&& );
  ~Topology();
};

#endif // GUARD__PROCESSOR__HH
