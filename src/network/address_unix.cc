//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
#include <network/address_unix.cpp>
#include <utility/testing.h>
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
		std::string address_arg( "/tmp/address_unix_test.unix" );
		network::address_unix address( address_arg );
  }

  void structors_heap()
  {
		std::string address_arg( "/tmp/address_unix_test.unix" );
		auto address = std::make_shared< network::address_unix >( address_arg );
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
    addr();
    size();
  }

  void addr()
  {
		std::string address_arg( "/tmp/address_unix_test.unix" );
		network::address_unix address( address_arg );

    struct sockaddr_un sa;
    bzero( &sa, sizeof( sa ) );
	  sa.sun_family = AF_LOCAL;
  	strcpy( sa.sun_path, address_arg.c_str() );

    PASSES( address.addr()->sa_family == reinterpret_cast< struct sockaddr& >( sa ).sa_family );
    const struct sockaddr_un *address_ptr = reinterpret_cast< const struct sockaddr_un* >( address.addr() ) ;
    PASSES( address_ptr->sun_family == sa.sun_family );
    PASSES( std::strcmp( address_ptr->sun_path, sa.sun_path ) == 0 );
  }

  void size()
  {
		std::string address_arg( "/tmp/address_unix_test.unix" );
		network::address_unix address( address_arg );

    PASSES( address.size() == sizeof( sockaddr_un ) );
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
