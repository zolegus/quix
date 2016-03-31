//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
#include <utility/daemon.cpp>
//#include <node/node.cpp>
#include <utility/configuration.cpp>
#include <memory/mapped.cpp>
#include <utility/testing.h>
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
struct plumbing
{
  plumbing()
  {
    structors_stack();
    structors_heap();
    copy_constructor();
    move_constructor();
    copy_assignment();
    move_assignment();
  }

  void structors_stack()
  {
  }

  void structors_heap()
  {
  }

  void copy_constructor()
  {
  }

  void move_constructor()
  {
  }

  void copy_assignment()
  {
  }

  void move_assignment()
  {
  }
}
plumbing;
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
struct Rundaemon
{
  Rundaemon()
  {
    StreamParsing();
  }

  void StreamParsing()
  {
  }
}
rundaemon;
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
