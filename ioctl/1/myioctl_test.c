#include <sys/ioctl.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>

struct mydata {
	int state;
	char letter;
};

void set_newdata( struct mydata *n )
{
	n->state = 5;
	n->letter = 'b';
}

int main( void )
{

	int fd;
	int nbytes;
	int ret;
	unsigned int ioctl_n;
	char *devicename = "/dev/myioctl";
	struct mydata newdata;

		fd = open( devicename, O_RDWR );
		if( fd == -1 )
			return -1;
	
		ioctl_n = _IOR( 'k', 1, struct mydata );
		ret = ioctl( fd, ioctl_n, &newdata );

		printf( "Got the state: State: %d, letter: %c\n", newdata.state, newdata.letter );

		set_newdata( &newdata );

		ioctl_n = _IOW( 'k', 1, struct mydata );
		ret = ioctl( fd, ioctl_n, &newdata );

		ioctl_n = _IOR( 'k', 1, struct mydata );
                ret = ioctl( fd, ioctl_n, &newdata );
	
		printf( "Got the state: State: %d, letter: %c\n", newdata.state, newdata.letter );

		close( fd );

		return 0;
}
			

