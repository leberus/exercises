#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>

int main( void )
{
	const char *nodename = "/dev/mydrv";
	int fd;

		fd = open( nodename, O_RDONLY );
		printf( "Abierto, esperare..\n" );
		sleep( 10 );
	
		close( fd );
		return 0;
}
	
