#ifndef __GUARD__ARGUMENTS_H__
#define __GUARD__ARGUMENTS_H__


void init_arguments( int argc, char *argv[] );
char get_argument_as_char( char opt );
int get_argument_as_int( char opt );
char *get_argument_as_string( char opt );


#endif // __GUARD__ARGUMENTS_H__

