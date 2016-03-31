#ifndef GUARD__PROCESSOR__HH
#define GUARD__PROCESSOR__HH

#include "Cursor.h"
#include "Configuration.h"

class Quix::Cursor
{
  struct Impl;
  Impl *pimpl;

public:

  Cursor( Configuration&& );
  Cursor( const Cursor& );
  Cursor( Cursor&& );
  Cursor& operator=( Cursor );
  Cursor& operator=( Cursor&& );
  ~Cursor();
};

#endif // GUARD__PROCESSOR__HH
