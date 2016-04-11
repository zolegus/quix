//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
#include <network/address_inet.cpp>
#include <utility/testing.h>
#include <memory>
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
		std::string address_arg( "localhost:12345" );
		network::address_inet address( address_arg );
  }

  void structors_heap()
  {
		std::string address_arg( "localhost:12345" );
		auto address = std::make_shared< network::address_inet >( address_arg );
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
		std::string address_arg( "localhost:12345" );
		network::address_inet address( address_arg );

    struct sockaddr_in sa;
    bzero( &sa, sizeof( sa ) );
  	sa.sin_family = AF_INET;
    sa.sin_addr.s_addr = inet_addr( "localhost" );
    sa.sin_port = htons( 12345 );

    CHECK( address.addr()->sa_family == reinterpret_cast< struct sockaddr& >( sa ).sa_family );
    const struct sockaddr_in *address_ptr = reinterpret_cast< const struct sockaddr_in* >( address.addr() ) ;
    CHECK( address_ptr->sin_family == sa.sin_family );
    CHECK( address_ptr->sin_addr.s_addr == sa.sin_addr.s_addr );
    CHECK( address_ptr->sin_port == sa.sin_port );
  }

  void size()
  {
		std::string address_arg( "localhost:12345" );
		network::address_inet address( address_arg );

    CHECK( address.size() == sizeof( sockaddr_in ) );
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
