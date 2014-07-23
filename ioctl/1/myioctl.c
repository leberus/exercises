#include <linux/module.h>
#include <linux/init.h>
#include <linux/ioctl.h>
#include <linux/fs.h>
#include <asm/uaccess.h>
#include <linux/cdev.h>

#define ON 1
#define OFF 2

static char MYTYPE = 'k';

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

static struct driver_state mystate =
{
	.state = OFF,
	.letter = 'f',
};

static struct cdev *mydev;
static dev_t dev;
static char *myname = "myioctl";

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
	void __user *arp = ( void __user* )p;
	unsigned int size;
	int ret;

		if( _IOC_TYPE( cmd ) != MYTYPE )
			return -EINVAL;

		size = _IOC_SIZE( cmd );

		switch( _IOC_DIR( cmd ) )
		{
			case _IOC_READ:

				ret = copy_to_user( arp, &mystate, size );
				break;

			case _IOC_WRITE:
		
				ret = copy_from_user( &mystate, arp, size );
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

