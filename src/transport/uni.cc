//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
#include <transport/uni.cpp>
#include <utility/testing.h>
#include <memory>
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
struct plumbing
{
  typedef struct
	{
		char id;
    short key;
		int data;
	}
	event_type;

  using transport_type = transport::uni< event_type >;
  char data[ 65536 ];

  plumbing()
  {
    structors_stack();
    structors_heap();
  }

  void structors_stack()
  {
    transport_type uni( (void*)data, 1, {2} );
  }

  void structors_heap()
  {
    using follow_list_type = transport_type::follow_list_type;
    auto uni1 = std::make_shared< transport_type >( (void*)data, 1, follow_list_type( { 2 } ) );
  }
}
plumbing;
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
struct api
{
  using transport_type = transport::uni< uint64_t >;
  char data[ 65536 ];

  api()
  {
    aquire_test();
    aquire();
    commit();
    reaquire_test();
    reaquire();
    release();
 		basic_three_stage_sequence();
  }

  void aquire_test()
  {
    return;
  }

  void aquire()
  {
    return;
  }

  void commit()
  {
    return;
  }

  void reaquire_test()
  {
    return;
  }

  void reaquire()
  {
    return;
  }

  void release()
  {
    return;
  }

  void basic_three_stage_sequence()
	{
    transport_type uni( (void*)data, 1, {2} );
	}
}
api;
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
struct scenarios
{
  using transport_type = transport::uni< uint64_t >;
  char data[ 1024 * 1024 * 1024 ];

  scenarios()
  {
 		basic_three_stage_sequence();
  }

  void basic_three_stage_sequence()
	{
    transport_type injector( (void*)data, 1, {0} );
    transport_type processor( (void*)data, 2, {1} );
    transport_type terminator( (void*)data, 3, {2} );

    uint64_t &value1 = injector.aquire();
    value1 = 10;
    injector.commit();

    uint64_t &value2 = processor.reaquire();
    PASSES( value1 == value2 );
		value2 = 20;
    processor.commit();

    uint64_t &value3 = terminator.reaquire();
    PASSES( value2 == value3 );
		terminator.release();
	}
}
scenarios;
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
