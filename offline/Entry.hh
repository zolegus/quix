#ifndef GUARD__PROCESSOR__HH
#define GUARD__PROCESSOR__HH

#include "Entry.h"
#include "Configuration.h"

class Quix::Entry
{
  struct Impl;
  Impl *pimpl;

public:

  Entry( Configuration&& );
  Entry( const Entry& );
  Entry( Entry&& );
  Entry& operator=( Entry );
  Entry& operator=( Entry&& );
  ~Entry();
};

#endif // GUARD__PROCESSOR__HH
