#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/kernel.h>
#include <asm/uaccess.h>
#include <linux/cdev.h>
#include <linux/slab.h>

static dev_t dev;
static char *myname = "mydevice";
static char kbuff[80] = "You're reading from the kernel buffer\0";

static int my_open( struct inode *inode, struct file *file );
static ssize_t my_read( struct file *file, char __user *userbuf, size_t lbuf, loff_t *pos );
static int my_close( struct inode *inode, struct file *file );

static struct cdev *mydev;


static struct file_operations fops =
{
	.owner = THIS_MODULE,
	.open = my_open,
	.read = my_read,
	.release = my_close
};

struct priv_data
{
	int nr;
};

struct priv_data *alloc_priv_data( void )
{
	return kmalloc( sizeof( struct priv_data ), GFP_KERNEL );
}	

static int my_close( struct inode *inode, struct file *file )
{
	struct priv_data *priv = ( struct priv_data * ) file->private_data;
			
		printk( KERN_INFO "Closing ( %d ) private data\n", priv->nr );
		kfree( priv );
		printk( KERN_INFO "Private data freed" );

	return 0;
}

static int my_open( struct inode *inode, struct file *file )
{
	static int count = 0 ;
	struct priv_data *priv = alloc_priv_data();

		priv->nr = ++count;
		file->private_data = ( void * ) priv;

		printk( KERN_INFO "private_data: OPEN %d\n", priv->nr );
		printk( KERN_INFO "Current reference count: %d - %d times opened\n", module_refcount( THIS_MODULE ), count );		
		return 0;
}

static ssize_t my_read( struct file *file, char __user *userbuf, size_t lbuf, loff_t *pos )
{
	int nbytes;
	int ret;
	
		if( *pos > 80 )
			return 0;
		else
		{
		
			ret = copy_to_user( userbuf, kbuff + *pos, 80 );
			nbytes = lbuf - ret;
			*pos += nbytes;
			printk( KERN_INFO " %d byted readed\n", nbytes );

			return nbytes;
		}
}

static int __init my_load( void )
{
	int ret;

		ret = alloc_chrdev_region( &dev, 0, 1, myname );
		mydev = cdev_alloc();
		cdev_init( mydev, &fops );
		cdev_add( mydev, dev, 1 );
		
		printk( KERN_INFO "Please create the proper device: Major-> %d Minor-> %d with ( %s ) name\n", MAJOR( dev ), MINOR( dev ), myname );

		return 0;
		 
}

static void __exit my_unload( void )
{
	unregister_chrdev_region( dev, 1 );
	cdev_del( mydev );
	printk( KERN_INFO "Device unregistered, please remove the proper entry ( %s )\n", myname );
}

module_init( my_load );
module_exit( my_unload );

