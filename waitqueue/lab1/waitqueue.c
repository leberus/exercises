#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/kernel.h>
#include <linux/cdev.h>
#include <linux/slab.h>
#include <linux/sched.h>
#include <asm/uaccess.h>
#include <asm/atomic.h>


static int my_open( struct inode *inode, struct file *file );
static ssize_t my_read( struct file *file, char __user *userbuf, size_t lbuf, loff_t *pos );
static ssize_t my_write( struct file *file, const char __user *userbuf, size_t lbuf, loff_t *pos );
static int my_close( struct inode *inode, struct file *file );
static loff_t myllseek( struct file *file, loff_t pos, int whence);

static dev_t dev;
static char *myname = "myqueue";

/* Pointer to buffer */
static char *kbuff;
/* Pointer to the last element of memory */
static char *p_kbuff;
/* Size of the buffer */
static unsigned int b_size;

/* Structure for character device */
static struct cdev *mydev;
/* Structure for wait_queue */
static wait_queue_head_t wq;


/* Atomic variable for wait_queue condition */
atomic_t data = ATOMIC_INIT( 0 );

static struct file_operations fops =
{
	.owner = THIS_MODULE,
	.open = my_open,
	.read = my_read,
	.release = my_close,
	.write = my_write,
	.llseek = myllseek,
};

static loff_t myllseek( struct file *file, loff_t pos, int whence )
{
	int new_pos;

		switch( whence )
		{
			case SEEK_SET:
				new_pos = pos;
				break;

			case SEEK_CUR:
				new_pos = file->f_pos + pos;
				break;

			case SEEK_END:
				return -EINVAL;
				break;

			default:
				return -EINVAL;
				break;
		}

		file->f_pos = new_pos;
		return new_pos;
}

static int my_close( struct inode *inode, struct file *file )
{
	return 0;
}

static int my_open( struct inode *inode, struct file *file )
{
	static int count = 0 ;
		
		printk( KERN_INFO "Current reference count: %d - %d times opened\n", module_refcount( THIS_MODULE ), count );		
		return 0;
}

 static ssize_t my_write( struct file *file, const char __user *userbuf, size_t lbuf, loff_t *pos )
{
	ssize_t m_bytes;
	ssize_t w_bytes;
	int ret;

		m_bytes = b_size - *pos;
		m_bytes = m_bytes > lbuf ? lbuf : m_bytes;
				
		w_bytes = m_bytes - copy_from_user( kbuff + *pos, userbuf, lbuf );
		*pos += w_bytes;
		p_kbuff += w_bytes;

		ret = w_bytes;
		pr_info( "Write: Pos [%lld] - Bytes written %ld\n", *pos, w_bytes );
		atomic_set( &data, 1 );
		wake_up_interruptible( &wq ); 


		return ret;
} 

static ssize_t my_read( struct file *file, char __user *userbuf, size_t lbuf, loff_t *pos )
{
	ssize_t m_bytes;
	ssize_t r_bytes;
	int ret;

		if( *pos == 0 )
		{	
			pr_info( "Putting in sleeping mode: [ %d ] - [%s ]\n", current->pid, current->comm );
			wait_event_interruptible( wq, atomic_read( &data ) ); 
			pr_info( "Putting in awake mode: [ %d ] - [%s ]\n", current->pid, current->comm );
		}

		if( *pos >= ( p_kbuff - kbuff ) )
			return 0;

		m_bytes = b_size - *pos;
		m_bytes = m_bytes > lbuf ? lbuf : m_bytes;
		r_bytes = m_bytes - copy_to_user( userbuf, kbuff + *pos, lbuf );
		*pos += r_bytes;

		ret = r_bytes;
		pr_info( "Read: Pos [%lld] - Bytes read %ld\n", *pos, r_bytes );

		atomic_set( &data, 0 );
		return ret;
}

static int __init my_load( void )
{
	int ret;

		ret = alloc_chrdev_region( &dev, 0, 1, myname );
		mydev = cdev_alloc();
		cdev_init( mydev, &fops );
		cdev_add( mydev, dev, 1 );

		p_kbuff = kbuff = kmalloc( PAGE_SIZE, GFP_KERNEL );
		b_size = PAGE_SIZE;
	
		init_waitqueue_head( &wq );
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

