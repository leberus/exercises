#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/kernel.h>
#include <asm/uaccess.h>
#include <linux/cdev.h>
#include <linux/slab.h>

static dev_t dev;
static char *myname = "mydevice";

/* PAGE SIZE */
static int p_size = PAGE_SIZE;

/* Pointer to buffer */
static char *kbuff;
/* Pointer to the last element of memory */
static char *p_kbuff;
/* Size of the buffer */
static unsigned int b_size;

static int my_open( struct inode *inode, struct file *file );
static ssize_t my_read( struct file *file, char __user *userbuf, size_t lbuf, loff_t *pos );
static ssize_t my_write( struct file *file, const char __user *userbuf, size_t lbuf, loff_t *pos );
static int my_close( struct inode *inode, struct file *file );

static struct cdev *mydev;
static struct kmem_cache *mycache;

static struct file_operations fops =
{
	.owner = THIS_MODULE,
	.open = my_open,
	.read = my_read,
	.release = my_close,
	.write = my_write,
}; 

static int my_close( struct inode *inode, struct file *file )
{
	pr_info( "[DELETING OBJECT] kbuff - %p\n", kbuff );
	kmem_cache_free( mycache, kbuff );
	return 0;
}

static int my_open( struct inode *inode, struct file *file )
{
	static int count = 0 ;
	
		p_kbuff = kbuff = kmem_cache_alloc( mycache, GFP_KERNEL );
		pr_info( "[ALLOCATING OBJECT] kbuff - %p\n", kbuff );
		printk( KERN_INFO "Current reference count: %d - %d times opened\n", module_refcount( THIS_MODULE ), count );		
		return 0;
}

 static ssize_t my_write( struct file *file, const char __user *userbuf, size_t lbuf, loff_t *pos )
{
	ssize_t nbytes;
	int ret;

		printk( KERN_INFO "W - POS: %lld LBUF: %ld\n", *pos, lbuf );

		


		if( ( lbuf + *pos ) > b_size )
		{
			printk( "Returning zero.. Pos: %lld\n", *pos );
			return 0;
		}
		else
		{
			printk( KERN_INFO "Write\n" );
			
			ret = copy_from_user( kbuff + *pos, userbuf, lbuf );
			printk( KERN_INFO " %d ret\n", ret );
			nbytes = lbuf - ret;
			*pos += nbytes;
			p_kbuff += nbytes;
			printk( KERN_INFO " %ld bytes written\n", nbytes );

			printk( KERN_INFO " bUFF: %s\n", kbuff );

			return nbytes;
		}
} 

static ssize_t my_read( struct file *file, char __user *userbuf, size_t lbuf, loff_t *pos )
{
	ssize_t nbytes;
	int p_p = p_kbuff - kbuff;

	
		printk( KERN_INFO "R - POS: %lld LBUF: %ld p_p: %d\n", *pos, lbuf, p_p );
	
		if( ( lbuf + *pos ) > p_p )
			return 0;
		else
		{
			nbytes = lbuf - copy_to_user( userbuf, kbuff + *pos, lbuf );
			*pos += nbytes;
			printk( KERN_INFO " %ld bytes readed\n", nbytes );

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

		mycache = kmem_cache_create( "mycache", p_size, 0, SLAB_HWCACHE_ALIGN, NULL );
		b_size = p_size;
		pr_info( "[ALLOCATING CACHE] mycache - %p\n", mycache );
		printk( KERN_INFO "Please create the proper device: Major-> %d Minor-> %d with ( %s ) name\n", MAJOR( dev ), MINOR( dev ), myname );

		return 0;
		 
}

static void __exit my_unload( void )
{
	unregister_chrdev_region( dev, 1 );
	cdev_del( mydev );
	pr_info( "[DELETING CACHE] mycache - %p\n", mycache );
	kmem_cache_destroy( mycache );
	printk( KERN_INFO "Device unregistered, please remove the proper entry ( %s )\n", myname );
}

module_init( my_load );
module_exit( my_unload );

