#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>

const char *mydev = "/dev/mydevice";
const char *myfile = "open.c";

int main( void )
{
	int fd_o;
	int fd_d;
	int nbytes_r, nbytes_w;
	
	char buf[80];

		fd_o = open( myfile, O_RDONLY );
		if( fd_o == -1 )
		{
			perror( "Open - " );
			return -1;
		}

		fd_d = open( mydev, O_CREAT | O_RDWR, S_IRWXU );
		if( fd_d == -1 )
		{
			perror( "Open - mydev" );
			return -1;
		}

		sleep( 5 );
		
		while( ( nbytes_r = read( fd_o, buf, sizeof( buf ) - 1 ) ) > 0 )
		{
			nbytes_w = write( fd_d, buf, nbytes_r );
		}

/*		if( lseek( fd_d, 79, SEEK_SET ) == -1 )
			perror( "lseek" ); */

		while( ( nbytes_r = read( fd_d, buf, sizeof( buf ) - 1 ) ) > 0 )
                {       
                        nbytes_w = write( 1, buf, nbytes_r );
                }

		
		close( fd_o );
		close( fd_d );

		return 0;
}
