#include "RingBuffer.cpp"
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

struct RunRingBuffer
{
  RunRingBuffer()
  {
    StreamParsing();
  }

  void StreamParsing()
  {
  }
}
runRingBuffer;
