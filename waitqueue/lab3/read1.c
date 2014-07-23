#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <poll.h>

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
	int ready;
	char buff[1024];
	struct pollfd poll_t;

		fd = open( nodename, O_RDWR );
		if( fd == -1 )
		{
			perror( "open" );
			return -1;
		}

		memset( buff, '\0', sizeof( buff ) );
		poll_t.fd = fd;
		poll_t.events |= POLLIN | POLLRDNORM;

		if( (ready = poll( &poll_t, 1, 5000 ) ) < 0 )
		{
			perror( "poll" );
			close( fd );
			return-1;
		}

		if( ready > 0 )
		{
			printf( "Poll returned: %d\n", ready );
			if( poll_t.revents & POLLIN )
			{
				r_bytes = read( fd, buff, 10 );
				printf( "%d bytes read; %s\n", r_bytes, buff );
			}
			else
			{
				printf( "No pollin configured!\n" );
			}
		}
		else
		{
			printf( "Timedout!\n" );
		}	

		close( fd );
		return 0;
}


		
		

