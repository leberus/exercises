#include <stdio.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>

#define MYTYPE 'k'
#define MYDEV_GET_BUFF_SIZE 1

void compare( char *read_b, char *map_b, int fd, int len )
{
	if( read( fd, read_b, len ) != len )
		printf( "Some error with read\n" );

	printf( "R-> %c\n", *(read_b + 128 ));
	

	if( memcmp( read_b, map_b, len ) != 0 )
		printf( "Buffer not equal\n" );
}

int main( void )
{
	int fd;
	int size;
	int cmd;
	const char *nodename = "/dev/mymmap";
	char *mem_area;
	char *read_buf;

		fd = open( nodename, O_RDWR );
		if( fd == -1 )
		{
			fprintf(stderr, "open of %s failed:  %s\n", nodename, strerror(errno));
			return -1;
		}

		cmd = _IOR( MYTYPE, MYDEV_GET_BUFF_SIZE, int );
		ioctl( fd, cmd, &size );

		read_buf = ( char * )malloc( size );

		mem_area = mmap( NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0 );
		if( mem_area == MAP_FAILED )
		{
			fprintf(stderr, "mmap of %s failed:  %s\n", nodename, strerror(errno));
			return -1;
		}
		
		compare( read_buf, mem_area, fd, size );

		*(mem_area + 128 ) = 'b';
		printf( "C-> %c\n", *(mem_area + 128 ));
		lseek(fd, 0, SEEK_SET);
		compare( read_buf, mem_area, fd, size );
		sleep( 1200 );

		close( fd );
		free( read_buf );

		return 0;
}
		
