//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
#include <node/processor.cpp>
#include <utility/testing.h>
#include <memory/local.cpp>
#include <transport/uni.cpp>
#include <behaviour/event_republish.cpp>
#include <memory>
#include <cstring>
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
    using transport_type = transport::uni< uint64_t >;
		memory::local transport_memory( 1048576 );
    std::memset( transport_memory.data(), 0, transport_memory.size() );
		transport_type transport( transport_memory.data(), 1, { 2 } );

    using behaviour_type = behaviour::event_republish< uint64_t >;
		memory::local behaviour_memory( 1048576 );
		behaviour_type behaviour( behaviour_memory.data(), 1 );

    using processor_type = node::processor< transport_type, behaviour_type >;
    processor_type injector( transport, behaviour );
  }

  void structors_heap()
  {
    using transport_type = transport::uni< uint64_t >;
		memory::local transport_memory( 1048576 );
    std::memset( transport_memory.data(), 0, transport_memory.size() );
		transport_type transport( transport_memory.data(), 1, { 2 } );

    using behaviour_type = behaviour::event_republish< uint64_t >;
		memory::local behaviour_memory( 1048576 );
		behaviour_type behaviour( behaviour_memory.data(), 1 );

    using processor_type = node::processor< transport_type, behaviour_type >;
    auto processor = std::make_shared< processor_type >( transport, behaviour );
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
