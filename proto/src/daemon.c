#include "daemon.h"
#include "arguments.h"
#include <stdio.h> // printf
#include <stdlib.h> //exit
#include <fcntl.h> //open
#include <unistd.h> // close
#include <errno.h> //errno
#include <string.h> //strerror
#include <sys/stat.h> // umask


void init_daemon()
{
  const char *stdout_filename = get_argument_as_string(  'd' );
  if( !stdout_filename || !*stdout_filename ) return;
  printf( "Stdout Filename: %s\n", stdout_filename );

  int fd;
  if( ( fd = open( stdout_filename, O_WRONLY|O_CREAT, 0 ) ) < 0 )
  {
    printf( "Failed to open file write daemon stdout %s: %d %s\n", 
      stdout_filename, errno, strerror( errno ) );
    exit( 1 );
  }

  pid_t pid = fork();
  
  // Something went wrong
  if( pid < 0 )
  {
    printf( "Failed to init as daemon\n" );
    exit( EXIT_FAILURE );
  }

  // If we are the parent, exit
  if( pid > 0 )
  {
    printf( "Daemon: started\n" );
    exit( EXIT_SUCCESS );
  }

  // We must be the child
  setsid();
  chdir("/");
  umask(0);

  fclose(stdin);
  fclose(stdout);
  fclose(stderr);

  dup2(fd, STDOUT_FILENO);
  close( fd );
}

