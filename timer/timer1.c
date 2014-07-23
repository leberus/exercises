#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/timer.h>
#include <linux/cdev.h>
#include <linux/fs.h>

static ssize_t my_write( struct file *file, const char __user *userbuf, size_t lbuf, loff_t *pos );
static int my_open( struct inode *inode, struct file *file );
static int my_close( struct inode *inode, struct file *file );

static struct timer_list mytimmy;
static struct cdev *mydev;

static const char *devname = "leberus";
static dev_t dev;

static unsigned int o_counter = 0;

static struct file_operations fops =
{
        .owner = THIS_MODULE,
        .open = my_open,
        .release = my_close,
        .write = my_write,
};

static void function_timer( unsigned long data )
{
	pr_info( "[Timer] Function timer called! at address 0x%lx\n", data );
	pr_info( "[Timer] Current jiffies: %ld\n", jiffies );
}

static ssize_t my_write( struct file *file, const char __user *userbuf, size_t lbuf, loff_t *pos )
{
	init_timer( &mytimmy );
	mytimmy.expires = jiffies + ( HZ * 15 );
	pr_info( "[Timer] Jiffies: %ld , HZ: %d\n", jiffies, HZ );
	mytimmy.function = function_timer;
	mytimmy.data = ( unsigned long ) function_timer;
	add_timer( &mytimmy );

	return -1;
}

static int my_open( struct inode *inode, struct file *file )
{
	return 0;
}

static int my_close( struct inode *inode, struct file *file )
{
	--o_counter;
	return 0;
}

static int __init my_init( void )
{
	int ret;
	
		ret = alloc_chrdev_region( &dev, 0, 1, devname );
		mydev = cdev_alloc();
		cdev_init( mydev, &fops );
		cdev_add( mydev, dev, 1 );

		pr_info( "[Timer] Please create the proper device: Major-> %d Minor-> %d with ( %s ) name\n", MAJOR( dev ), MINOR( dev ), devname );

		return 0;
}

static void __exit my_exit( void )
{
	unregister_chrdev_region( dev, 1 );
        cdev_del( mydev );
}

module_init( my_init );
module_exit( my_exit );


