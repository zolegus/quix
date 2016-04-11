//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
#include <transport/slot.cpp>
#include <transport/uni.cpp>
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
  }

  void structors_stack()
  {
  }

  void structors_heap()
  {
  }
}
plumbing;
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
struct scenarios
{
  typedef struct
	{
		char id;
    short key;
		int data;
	}
	event_type;

  char data[ 1024 * 1024 * 1024 ];

  scenarios()
  {
 		basic_three_stage_sequence();
  }

  void basic_three_stage_sequence()
	{
    using transport_type = transport::uni< event_type >;

    transport_type injector( (void*)data, 1, {0} );
		{
      auto slot = transport::make_aquire_commit_slot( injector );
      slot->id = 1;
      slot->key = 22;
      slot->data = 3333;
		}

    transport_type processor( (void*)data, 2, {1} );
		{
      auto slot = transport::make_reaquire_commit_slot( processor );
      CHECK( slot->id == 1 );
      CHECK( slot->key == 22 );
      CHECK( slot->data == 3333 );
      slot->id = 4;
      slot->key = 55;
      slot->data = 666;
		}

    transport_type terminator( (void*)data, 3, {2} );
		{
      auto slot = transport::make_reaquire_release_slot( terminator );
      CHECK( slot->id == 4 );
      CHECK( slot->key == 55 );
      CHECK( slot->data == 666 );
		}
	}
}
scenarios;
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
