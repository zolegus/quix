//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
#include <network/socket_connected.cpp>
#include <network/socket_listening.cpp>
#include <network/address_inet.cpp>
#include <network/address_unix.cpp>
#include <utility/testing.h>
#include <memory>
#include <thread>
#include <sstream>
#include <functional>
#include <future>
#include <cstring>
#include <ctime>
#include <iostream>
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
namespace {
struct address
{
  int port_mem;
  static int next_port()
  {
    static int port = 10000 + std::clock() % 40000;
    return ++port;
  }
  address() : port_mem( next_port() ) {};


  std::string inet( const std::string &hostname ) 
  {
    std::stringstream ss;
    ss << hostname << ":" << port_mem;
    return ss.str();
  }
  std::string any() { return inet( "" ); }
  std::string localhost() { return inet( "127.0.0.1" ); }


  std::string unix() 
  {
    std::stringstream ss;
    ss << "/tmp/domain" << port_mem << ".unix";
    return ss.str();
  }
};}
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
struct plumbing
{
  static
  void
  listener(
    std::promise< void > &vprom,
    const std::string address_arg
    )
  {
    network::socket_listening< network::address_inet > listening( address_arg );
    vprom.set_value();
    network::socket_connected< network::address_inet > connected( listening.accept() );
  }

  plumbing()
  {
    structors_stack();
    structors_heap();
  }

  void structors_stack()
  {
    address addr;

    std::promise< void > vprom;
    auto vfut = vprom.get_future();
    std::thread listener_thread( plumbing::listener, std::ref( vprom ), addr.any() );
    vfut.get();
    std::this_thread::yield();

    network::socket_connected< network::address_inet > connected( addr.localhost() );

    listener_thread.join();
  }

  void 
  structors_heap()
  {
    address addr;

    std::promise< void > vprom;
    auto vfut = vprom.get_future();
    std::thread listener_thread( listener, std::ref( vprom ), addr.any() );
    vfut.get();

    network::socket_connected< network::address_inet > connected( addr.localhost() );

    listener_thread.join();
  }
}
plumbing;
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
struct api
{
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
  template< typename A >
  static
  void
  echo(
    std::promise< void > &vprom,
    const std::string address_arg
    )
  {
    network::socket_listening< A > listening( address_arg );
    vprom.set_value();
    network::socket_connected< A > connected( listening.accept() );

    char buff[1024]; memset( buff, 0, 1024 );
    auto recvd  = connected.recv( buff, 1024 );
    auto sent = connected.send( buff, recvd );
    if( recvd != sent ) throw std::logic_error( "received size did not match sent size" );
  }

  static
  void
  echo_fd(
    std::promise< void > &vprom,
    const std::string address_arg
    )
  {
    network::socket_listening< network::address_unix > listening( address_arg );
    vprom.set_value();
    network::socket_connected< network::address_unix > connected( listening.accept() );

    int fd;
    char buff[1024]; memset( buff, 0, 1024 );
    auto recvd = connected.recv_fd( buff, 1024, fd );
    auto sent = connected.send_fd( buff, recvd, fd );
    if( recvd != sent ) throw std::logic_error( "received size did not match sent size" );
  }

  static
  void
  connector(
    std::future< void > &vfut,
    const std::string address_arg
    )
  {
    vfut.get();
	  std::this_thread::yield();
	  network::socket_connected< network::address_inet > connected( address_arg );

    char buff[1024]; memset( buff, 0, 1024 );
    auto recvd  = connected.recv( buff, 1024 );
    auto sent = connected.send( buff, recvd );
    if( recvd != sent ) throw std::logic_error( "received size did not match sent size" );
  }

  api()
  {
    send_recv_inet();
    send_recv_unix();
    send_recv_fd();
  }

  void send_recv_inet()
  {
    address addr;

    std::promise< void > vprom;
    auto vfut = vprom.get_future();
    std::thread echo_thread( api::echo< network::address_inet >, std::ref( vprom ), addr.any() );
    vfut.get();
    std::this_thread::yield();

    network::socket_connected< network::address_inet > connected( addr.localhost() );

    const std::string send_buff( "Mary had a little lamb" );
    auto send_size = connected.send( send_buff.c_str(), send_buff.size() );

    char recv_buff[1024]; memset( recv_buff, 0, 1024 );
    auto recv_size = connected.recv( recv_buff, 1024 );

    echo_thread.join();

    PASSES( send_size == recv_size );
    PASSES( send_buff == std::string( recv_buff, recv_size ) );
  }

  void send_recv_unix()
  {
    address addr;

    std::promise< void > vprom;
    auto vfut = vprom.get_future();
    std::thread echo_thread( api::echo< network::address_unix >, std::ref( vprom ), addr.unix() );
    vfut.get();
    std::this_thread::yield();

    network::socket_connected< network::address_unix > connected( addr.unix() );

    const std::string send_buff( "Mary had a little lamb" );
    auto send_size = connected.send( send_buff.c_str(), send_buff.size() );

    char recv_buff[1024]; memset( recv_buff, 0, 1024 );
    auto recv_size = connected.recv( recv_buff, 1024 );

    echo_thread.join();

    PASSES( send_size == recv_size );
    PASSES( send_buff == std::string( recv_buff, recv_size ) );
  }

  void send_recv_fd()
  {
    address addr_inet;

   	network::socket_listening< network::address_inet > listening( addr_inet.any() );

    std::promise< void > vprom_inet;
    auto vfut_inet = vprom_inet.get_future();
		std::thread connector_thread( api::connector, std::ref( vfut_inet ), addr_inet.localhost() );

    vprom_inet.set_value();
  	int send_fd = listening.accept();


    address addr;

    std::promise< void > vprom;
    auto vfut = vprom.get_future();
    std::thread echo_fd_thread( api::echo_fd, std::ref( vprom ), addr.unix() );
    vfut.get();
    std::this_thread::yield();

    network::socket_connected< network::address_unix > connected( addr.unix() );

    const std::string send_buff( "And every where than Mary went" );
    auto send_size = connected.send_fd( send_buff.c_str(), send_buff.size(), send_fd );

    char recv_buff[1024]; memset( recv_buff, 0, 1024 );
    int recvd_fd;
    auto recv_size = connected.recv_fd( recv_buff, 1024, recvd_fd );

  	network::socket_connected< network::address_inet > connected_inet( recvd_fd );

    const std::string send_buff_inet( "Whose fleese was white as snow" );
    auto send_size_inet = connected_inet.send( send_buff_inet.c_str(), send_buff_inet.size() );

    char recv_buff_inet[1024]; memset( recv_buff_inet, 0, 1024 );
    auto recv_size_inet = connected_inet.recv( recv_buff_inet, 1024 );

    echo_fd_thread.join();
    connector_thread.join();

    PASSES( send_size == recv_size );
    PASSES( send_buff == std::string( recv_buff, recv_size ) );

    PASSES( send_size_inet == recv_size_inet );
    PASSES( send_buff_inet == std::string( recv_buff_inet, recv_size_inet ) );
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

