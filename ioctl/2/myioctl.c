#include <linux/module.h>
#include <linux/init.h>
#include <linux/ioctl.h>
#include <linux/fs.h>
#include <linux/pid.h>
#include <linux/sched.h>
#include <asm/uaccess.h>
#include <linux/cdev.h>

#define ON 1
#define OFF 2

#define MYTYPE 'k'

#define SET_PROCESS _IO( MYTYPE, 0 )
#define SET_SIGNAL _IO( MYTYPE, 1 )
#define SEND_SIGNAL _IO( MYTYPE, 2 )

static int io_close( struct inode *inode, struct file *file );
static long io_ioctl( struct file *file, unsigned int cmd, unsigned long p );
static int io_open( struct inode *inode, struct file *file );

static struct file_operations fops =
{
	.owner = THIS_MODULE,
	.open = io_open,
	.release = io_close,
	.unlocked_ioctl = io_ioctl,
};

struct driver_state
{
	int state;
	int letter;
};

static struct cdev *mydev;
static dev_t dev;
static char *myname = "myioctl";

static struct task_struct *tsk = NULL;
static unsigned int signal;
static int pid;

static int io_open( struct inode *inode, struct file *file )
{
	return 0;
}

static int io_close( struct inode *inode, struct file *file )
{
	return 0;
}

static long io_ioctl( struct file *file, unsigned int cmd, unsigned long p )
{
	int ret = 0;

		if( _IOC_TYPE( cmd ) != MYTYPE )
			return -EINVAL;

		switch( cmd )
		{
			case SET_PROCESS:
			
				pid = (int)p;
				tsk = pid_task( find_vpid( pid ), PIDTYPE_PID );
				pr_info( "Pid: %s %d\n", tsk->comm, tsk->pid );
				break;

			case SET_SIGNAL:

				signal = (unsigned int )p;
				pr_info( "Signal: %d\n", signal );
				break;

			case SEND_SIGNAL: 

				ret = send_sig( signal, tsk, 0 );
				pr_info( "Signal sent\n" );
				break;

			default:

				ret = -EINVAL;
				break;
		}

		return ret;
}


static int __init my_init( void )
{
	int ret;

		ret = alloc_chrdev_region( &dev, 0, 1, myname );
		mydev = cdev_alloc();
		cdev_init( mydev, &fops );
		cdev_add( mydev, dev, 1 );

		pr_info( "Please create the proper device in /dev/%s with Major %d and Minor %d\n", myname, MAJOR( dev ), MINOR( dev ) );

		return 0;
}

static void __exit my_exit( void )
{
	unregister_chrdev_region( dev, 1 );
	cdev_del( mydev );
	pr_info( "Exiting from ioctl driver..\n" );
}

module_init( my_init );
module_exit( my_exit );
MODULE_LICENSE("GPL v2");
