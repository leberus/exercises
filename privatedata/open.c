#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>

const char *dev = "/dev/mydevice";

int main( void )
{
	int fd;
	int nbytes;
	char buff2[12];
	char buf[] = "hola!";

		fd = open( dev, O_RDWR );
		printf( "%d\n", fd );
		sleep( 10 );

		nbytes = write( fd, buf, sizeof( buf ));
		printf( "Nbytes: %d\n", nbytes );
		sleep( 10 );

		nbytes = read( fd, buff2, 11 );
		printf( "Nbytes: %d\n", nbytes );

		printf( "Read-> %s\n", buff2 );
		close( fd );

		return 0;
}
