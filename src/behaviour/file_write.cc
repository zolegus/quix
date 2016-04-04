//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
#include <behaviour/file_write.cpp>
#include <utility/testing.h>
#include <types/event.hpp>
#include <vector>
#include <memory>
#include <cstring>
#include <iostream>
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
    behaviour::file_write< types::event > file_write( filename );
  }

  void structors_heap()
  {
		std::string filename( "/tmp/mmfile0.dat" );
    auto file_write = std::make_shared< behaviour::file_write< types::event > >( filename );
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

		std::string filename( "/tmp/behaviour_file_write.txt" );
		{
		  behaviour::file_write< types::event > file_write( filename );
      for( const std::string &line : lines )
	  	{
		  	types::event::buffer_type buffer;
			  std::memcpy( buffer.data, line.c_str(), line.size() );
  			buffer.size = line.size();
	  	  types::event event;
        event.buffer_mem = &buffer;
        file_write( event );
	    }
		} 

    std::ifstream fs( filename );

		std::string input;
    for( const auto &line : lines )
		{
			std::getline( fs, input );
			std::cout << line << " == " << input << "\n";
		  PASSES( line == input );
		}
		std::getline( fs, input );
	  PASSES( input.size() == 0 );
	  PASSES( fs.eof() );
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
