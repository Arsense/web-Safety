
#include <string.h>
#include <stdio.h>

void main( void )
{
	char string[100] = "The String to End All Strings hello world!";
	char *copy1, *copy2;

	copy2 = _strupr( _strdup( string ) );

	printf( "Upper: %s\n", copy2 );
}
