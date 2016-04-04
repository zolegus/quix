//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
#include <behaviour/file_read.cpp>
#include <utility/testing.h>
#include <types/event.hpp>
#include <vector>
#include <string>
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
		std::string filename( "/tmp/mmfile0.dat" );
	  std::ofstream fs( filename );
    char *memory[ 1048576 + 64 ];
    behaviour::file_read< types::event > file_read( memory, filename );
  }

  void structors_heap()
  {
		std::string filename( "/tmp/mmfile0.dat" );
	  std::ofstream fs( filename );
    char *memory[ 1048576 + 64 ];
    auto file_read = std::make_shared< behaviour::file_read< types::event > >( memory, filename );
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
    functor();
  }

  void functor()
  {
		std::vector< std::string > lines = {
			"Mary had a little lamb",
			"whose fleece was white as snow",
			"and everywhere than Mary went",
			"the lamb was sure to go" 
		};

		std::string filename( "/tmp/behaviour_file_read.txt" );
		{
		  std::ofstream fs( filename );
      for( auto &line : lines )
	  		fs << line << "\n";
		}

    char *memory[ 1048576 + 64 ];
		std::memset( memory, 0, sizeof( memory ) );
		behaviour::file_read< types::event > file_read( memory, filename );
    
    for( const auto &line : lines )
		{
		  types::event event;
      file_read( event );
		  PASSES( std::string( event.buffer_mem->data, event.buffer_mem->size ) == line );
	  } 
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
