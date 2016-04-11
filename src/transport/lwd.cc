//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
#include <transport/lwd.cpp>
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

  using transport_type = transport::lwd< event_type >;
  char data[ 65536 ];

  plumbing()
  {
    structors_stack();
    structors_heap();
  }

  void structors_stack()
  {
    transport_type lwd( (void*)data, 1, {2} );
  }

  void structors_heap()
  {
    using follow_list_type = transport_type::follow_list_type;
    auto lwd1 = std::make_shared< transport_type >( (void*)data, 2, follow_list_type( { 1 } ) );
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
    aquire_test();
    aquire();
    commit();
    reaquire_test();
    reaquire();
    release();
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
}
api;
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
struct scenarios
{
  using transport_type = transport::lwd< uint64_t >;
  char data[ 1024 * 1024 * 1024 ];

  scenarios()
  {
 		basic_three_stage_sequence();
		basic_three_stage_sequence_with_batching();
  }

  void basic_three_stage_sequence()
	{
    transport_type injector( (void*)data, 1, {3} );
    transport_type processor( (void*)data, 2, {1} );
    transport_type terminator( (void*)data, 3, {2} );

    uint64_t &value1 = injector.aquire();
    value1 = 10;
    injector.commit();

    uint64_t &value2 = processor.reaquire();
    CHECK( value1 == value2 );
		value2 = 20;
    processor.commit();

    uint64_t &value3 = terminator.reaquire();
    CHECK( value2 == value3 );
		terminator.release();
	}

  void basic_three_stage_sequence_with_batching()
	{
    transport_type injector( (void*)data, 1, {3} );
    transport_type processor( (void*)data, 2, {1} );
    transport_type terminator( (void*)data, 3, {2} );

    uint64_t &value11 = injector.aquire();
    value11 = 11;

    uint64_t &value12 = injector.aquire();
    value12 = 12;

    uint64_t &value13 = injector.aquire();
    value13 = 13;

    injector.commit();
    injector.commit();
    injector.commit();
    CATCH( processor.commit(), std::runtime_error, "commit called on event that was not aquired" );
			
    uint64_t &value21 = processor.reaquire();
    CHECK( value11 == value21 );
		value21 = 21;

    uint64_t &value22 = processor.reaquire();
    CHECK( value12 == value22 );
		value22 = 22;

    uint64_t &value23 = processor.reaquire();
    CHECK( value13 == value23 );
		value23 = 23;

    processor.commit();
    processor.commit();
    processor.commit();
    CATCH( processor.commit(), std::runtime_error, "commit called on event that was not aquired" );
			
    uint64_t &value31 = terminator.reaquire();
    CHECK( value21 == value31 );

    uint64_t &value32 = terminator.reaquire();
    CHECK( value22 == value32 );

    uint64_t &value33 = terminator.reaquire();
    CHECK( value23 == value33 );

		terminator.release();
		terminator.release();
		terminator.release();
    CATCH( terminator.release(), std::runtime_error, "release called on event that was not aquired" );
	}
}
scenarios;
//
///////////////////////////////////////////////////////////////////////////////////////////////////
