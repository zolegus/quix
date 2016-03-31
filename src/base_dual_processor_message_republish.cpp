//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
#include <utility/configuration.hpp>
#include <memory/mapped.hpp>
#include <transport/dual.hpp>
#include <behaviour/event_republish.hpp>
#include <node/processor.hpp>
#include <fstream>
#include <iostream>
#include <cstddef>
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
using event_type = uint64_t;
using transport_memory_type = memory::mapped;
using transport_type = transport::dual< event_type >;
using behaviour_memory_type = memory::mapped;
using behaviour_type = behaviour::event_republish< event_type >;
using node_type = node::processor< transport_type, behaviour_type >;
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
int main(
  int argc,
  char *argv[]
  )
{
  utility::configuration cfg( argc, argv );
  std::cout << toString( cfg ) << std::endl;

  transport_memory_type transport_memory( 
    cfg.get< std::string >( "transport.memory.file" ), 
    cfg.get< int >( "transport.memory.base" ),
    cfg.get< int >( "transport.memory.size" )
    );

  transport_type transport(	
    transport_memory.data( 
      cfg.get< int >( "transport.memory.offset" ) 
      ), 
    cfg.get< transport_type::node_id_type >( "transport.id" ),
    cfg.get< transport_type::follow_list_type >( "transport.follow" )
    );

  behaviour_memory_type behaviour_memory( 
    cfg.get< std::string >( "behaviour.memory.file" ), 
    cfg.get< int >( "behaviour.memory.base" ),
    cfg.get< int >( "behaviour.memory.size" )
    );

  behaviour_type behaviour(
    behaviour_memory.data( 
      cfg.get< int >( "behaviour.memory.offset" ) 
 		  ),
    cfg.get< int >( "behaviour.loops" ) 
    );

  node_type node( 
    transport, 
    behaviour 
    );
  node();

  return 0;
}
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
