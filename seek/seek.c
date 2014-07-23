#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/kernel.h>
#include <asm/uaccess.h>
#include <linux/cdev.h>
#include <linux/slab.h>

static dev_t dev;
static char *myname = "mydevice";
static char *kbuff;

/* Size of the buffer */		
static int b_size = 2048;
/* Pointer to the last element of memory */
static char *p_kbuff;

static int my_open( struct inode *inode, struct file *file );
static ssize_t my_read( struct file *file, char __user *userbuf, size_t lbuf, loff_t *pos );
static ssize_t my_write( struct file *file, const char __user *userbuf, size_t lbuf, loff_t *pos );
static loff_t my_llseek( struct file *file, loff_t pos, int whence );
static int my_close( struct inode *inode, struct file *file );

static struct cdev *mydev;


static struct file_operations fops =
{
	.owner = THIS_MODULE,
	.open = my_open,
	.read = my_read,
	.release = my_close,
	.write = my_write,
	.llseek = my_llseek,
}; 


struct priv_data
{
	int nr;
};


	
static struct priv_data *alloc_priv_data( void )
{
	return kmalloc( sizeof( struct priv_data ), GFP_KERNEL );
}	

static int my_close( struct inode *inode, struct file *file )
{
	struct priv_data *priv = ( struct priv_data * ) file->private_data;
			
		kfree( priv );
		printk( KERN_INFO "Closing ( %d ) private data\n", priv->nr );
		printk( KERN_INFO "Private data freed" );

	return 0;
}

static int my_open( struct inode *inode, struct file *file )
{
	static int count = 0 ;
	struct priv_data *priv = alloc_priv_data();

		priv->nr = ++count;
		file->private_data = ( void * ) priv;

		file->f_mode &= ~FMODE_LSEEK;

		printk( KERN_INFO "private_data: OPEN %d\n", priv->nr );
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

loff_t my_llseek( struct file *file, loff_t pos, int whence )
{
	loff_t offset;
	
		switch( whence )
		{
			case SEEK_SET:	

				if( pos > b_size )
					offset = -1;
				else
					file->f_pos = offset = pos;
				break;

			case SEEK_CUR:

				if( file->f_pos + pos > b_size )
					offset = -1;
				else
				{
					file->f_pos += pos;
					offset = file->f_pos;
				}

				break;
		
			case SEEK_END:
	
				offset = file->f_pos = p_kbuff - kbuff;
				break;
	
			default:

				offset = -1;
				break;
		}

		printk( KERN_INFO "SEEK: %lld\n", offset );

		return offset;
}

static int __init my_load( void )
{
	int ret;

		ret = alloc_chrdev_region( &dev, 0, 1, myname );
		mydev = cdev_alloc();
		cdev_init( mydev, &fops );
		cdev_add( mydev, dev, 1 );

		kbuff = kmalloc( b_size, GFP_KERNEL );
		p_kbuff = kbuff;
		
		printk( KERN_INFO "Please create the proper device: Major-> %d Minor-> %d with ( %s ) name\n", MAJOR( dev ), MINOR( dev ), myname );

		return 0;
		 
}

static void __exit my_unload( void )
{
	unregister_chrdev_region( dev, 1 );
	cdev_del( mydev );
	kfree( kbuff );
	printk( KERN_INFO "Device unregistered, please remove the proper entry ( %s )\n", myname );
}

module_init( my_load );
module_exit( my_unload );

