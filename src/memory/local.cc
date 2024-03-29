//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
#include <memory/local.cpp>
#include <utility/testing.h>
#include <memory>
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
constexpr int mb = 1024576;
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
struct plumbing
{

  plumbing()
  {
    structors_stack();
    structors_heap();
  }

  void structors_stack()
  {
		memory::local local1( mb );
  }

  void structors_heap()
  {
    auto local1 = std::make_shared< memory::local >( mb );
  }
}
plumbing;
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
struct api
{
  api()
  {
    test();
  }

  void test()
  {
  }
}
api;
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
struct scenarios
{
  scenarios()
  {
    test();
  }

  void test()
  {
  }
}
scenarios;
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
