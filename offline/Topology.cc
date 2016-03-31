#include "Topology.cpp"
#include "Configuration.cpp"
#include "testing.h"

using namespace Quix;

struct CapricaSix
{
  CapricaSix()
  {
    StructorsStack();
    StructorsHeap();
    CopyConstructor();
    MoveConstructor();
    CopyAssignment();
    MoveAssignment();
  }

  void StructorsStack()
  {
  }

  void StructorsHeap()
  {
  }

  void CopyConstructor()
  {
  }

  void MoveConstructor()
  {
  }

  void CopyAssignment()
  {
  }

  void MoveAssignment()
  {
  }
}
CapricaSix;

struct RunTopology
{
  RunTopology()
  {
    StreamParsing();
  }

  void StreamParsing()
  {
  }
}
runTopology;
