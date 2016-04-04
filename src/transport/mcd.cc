//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
#include <transport/mcd.cpp>
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

  using transport_type = transport::mcd< event_type >;
  char meta[ 65536 ];

  plumbing()
  {
    structors_stack();
    structors_heap();
  }

  void structors_stack()
  {
		transport_type mcd1( (void*)meta, 1, 1, { { 2, 1 } } );
		transport_type mcd2( (void*)meta, 3,    { { 4, 1 } } );
  }

  void structors_heap()
  {
    using follow_list_type = transport_type::follow_list_type;
    auto mcd1 = std::make_shared< transport_type >( (void*)meta, 2,    follow_list_type( { { 1, 1 } } ) );
		auto mcd2 = std::make_shared< transport_type >( (void*)meta, 2, 4, follow_list_type( { { 1, 4 } } ) );
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
  using transport_type = transport::mcd< int >;
  char meta[ 1024 * 1024 * 1024 ]; 

  scenarios()
  {
 		basic_three_stage_sequence();
    basic_dual_channel_cross_sequence();
  }

  void basic_three_stage_sequence()
	{
    memset ( meta, 0, sizeof( meta ) );

    transport_type injector(   (void*)meta, 1, 1, {{3,1}} );
    transport_type processor(  (void*)meta, 2,    {{1,1}} );
    transport_type terminator( (void*)meta, 3,    {{2,1}} );

    PASSES( injector.aquire_test() == true );
    int &value1 = injector.aquire();
    value1 = 10;
    int *value1addr = &value1;
    injector.commit();

    PASSES( processor.reaquire_test() == true );
    int &value2 = processor.reaquire();
    PASSES( value1 == value2 );
    int *value2addr = &value2;
    PASSES( value1addr == value2addr );
		value2 = 20;
    processor.commit();

    PASSES( terminator.reaquire_test() == true );
    int &value3 = terminator.reaquire();
    PASSES( value2 == value3 );
    int *value3addr = &value3;
    PASSES( value2addr == value3addr );
		terminator.release();
	}

  void basic_dual_channel_cross_sequence()
	{
    memset ( meta, 0, sizeof( meta ) );

    // I1.1 \      / T4.X
    //        P3.X
    // I2.2 /      \ T5.X

    transport_type injector11(   (void*)meta, 1, 1, {{4,1}} );
		transport_type injector22(   (void*)meta, 2, 2, {{5,2}} );
		transport_type processor3x(  (void*)meta, 3,    {{1,1},{2,2}} );
		transport_type terminator41( (void*)meta, 4,    {{3,1}} );
	  transport_type terminator52( (void*)meta, 5,    {{3,2}} );

    // Thru channel 1
		{
    PASSES( injector11.aquire_test() == true );
    int &value1 = injector11.aquire();
    value1 = 11;
    int *value1addr = &value1;
    injector11.commit();

    PASSES( processor3x.reaquire_test() == true );
    int &value2 = processor3x.reaquire();
    PASSES( value1 == value2 );
    int *value2addr = &value2;
    PASSES( value1addr == value2addr );
		value2 = 21;
    processor3x.commit();

    PASSES( terminator41.reaquire_test() == true );
    int &value3 = terminator41.reaquire();
    PASSES( value2 == value3 );
    int *value3addr = &value3;
    PASSES( value2addr == value3addr );
		terminator41.release();
		}  

    // Thru channel 2
		{
    PASSES( injector22.aquire_test() == true );
    int &value1 = injector22.aquire();
    value1 = 12;
    int *value1addr = &value1;
    injector22.commit();

    PASSES( processor3x.reaquire_test() == true );
    int &value2 = processor3x.reaquire();
    PASSES( value1 == value2 );
    int *value2addr = &value2;
    PASSES( value1addr == value2addr );
		value2 = 22;
    processor3x.commit();

    PASSES( terminator52.reaquire_test() == true );
    int &value3 = terminator52.reaquire();
    PASSES( value2 == value3 );
    int *value3addr = &value3;
    PASSES( value2addr == value3addr );
		terminator52.release();
		}
	}
}
scenarios;
//
///////////////////////////////////////////////////////////////////////////////////////////////////