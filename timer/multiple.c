#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/timer.h>
#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/slab.h>

static ssize_t my_write( struct file *file, const char __user *userbuf, size_t lbuf, loff_t *pos );
static int my_open( struct inode *inode, struct file *file );
static int my_close( struct inode *inode, struct file *file );

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

struct mystruct {
	struct timer_list *mytimmy;
	int data;
};

	

static void function_timer( unsigned long data )
{
	struct mystruct *my_st = ( struct mystruct * )data;

		pr_info( "[Timer] Function timer called! at address 0x%lx\n", data );
		pr_info( "[Timer] Current jiffies: %ld, data %d\n", jiffies, my_st->data );

		kfree( my_st->mytimmy );
		kfree( my_st );
}

static ssize_t my_write( struct file *file, const char __user *userbuf, size_t lbuf, loff_t *pos )
{
	struct mystruct *my_st;
	struct timer_list *tl;
	static unsigned int somedata = 0;

		tl = ( struct timer_list * ) kmalloc( sizeof( struct timer_list ), GFP_KERNEL );
		my_st = ( struct mystruct * ) kmalloc( sizeof( struct mystruct ), GFP_KERNEL );

		init_timer( tl );
		tl->expires = jiffies + ( HZ * 15 );
		tl->function = function_timer;
		tl->data = ( unsigned long ) my_st;
		my_st->mytimmy = tl;
		my_st->data = ++somedata;
		add_timer( tl );
		
		pr_info( "[Timer] Jiffies: %ld , HZ: %d\n", jiffies, HZ );

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


