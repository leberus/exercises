#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>

void fillbuff( char *buff )
{
	int i;

		for( i = 0 ; i < 50 ; i++ )
		{
                        buff[i] = i + '0';
			printf( "%c\n", buff[i] );
		}

		buff[49] = '\0';
}

int main( void )
{

	const char *nodename= "/dev/myqueue";
	int fd;
	ssize_t r_bytes, w_bytes;
	char buff[1024];

		fd = open( nodename, O_RDWR );
		if( fd == -1 )
		{
			perror( "open" );
			return -1;
		}

/*		fillbuff( buff );
		w_bytes = write( fd, buff, 50 );
		printf( "Bytes written: %d\n", w_bytes );
		printf( "%s\n", buff );

		memset( buff, '\0', sizeof( buff ) );

		lseek( fd, 0, SEEK_SET );
*/
		memset( buff, '\0', sizeof( buff ) );

		while ( ( r_bytes = read( fd, buff, 10 ) ) > 0 )
		{
			printf( "Buff: %s\n", buff );
			printf( "Bytes read: %d\n", r_bytes );
		}

		close( fd );
		return 0;
}


		
		

