#include "Entry.cpp"
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

struct RunEntry
{
  RunEntry()
  {
    StreamParsing();
  }

  void StreamParsing()
  {
  }
}
runEntry;
