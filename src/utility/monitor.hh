//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
#ifndef GUARD__UTILITY__MONITOR__HH
#define GUARD__UTILITY__MONITOR__HH
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
#include <utility/monitor.h>
#include <utility/event.h>
#include <utility/configuration.h>
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
class utility::monitor
{
  struct impl;
  impl *pimpl;

public:

  monitor( utility::configuration&& );
  monitor( const monitor& );
  monitor( monitor&& );
  monitor& operator=( monitor );
  monitor& operator=( monitor&& );
  ~monitor();

	//  void observe( const node::node& );
  const event& monitored_event() const;
};
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
#endif // GUARD__UTILITY__MONITOR__H
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
