#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <signal.h>

int main( void )
{

	int fd;
	int cmd;
	int pid = 29202;
	int signal = SIGKILL;
	const char *devname = "/dev/myioctl";
	

		fd = open( devname, O_RDWR );
		if( fd == -1 )
			return -1;

		cmd = _IO( 'k', 0 );
		ioctl( fd, cmd, pid );

		cmd = _IO( 'k', 1 );
		ioctl( fd, cmd, signal );

		cmd = _IO( 'k', 2 );
		ioctl( fd, cmd, NULL );

		close( fd );
		return 0;
}

		
