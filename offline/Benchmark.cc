#include "Benchmark.cpp"
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

struct RunBenchmark
{
  RunBenchmark()
  {
    StreamParsing();
  }

  void StreamParsing()
  {
  }
}
runBenchmark;
