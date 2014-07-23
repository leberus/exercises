#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/kernel.h>
#include <linux/cdev.h>
#include <linux/slab.h>
#include <linux/sched.h>
#include <linux/interrupt.h>
#include <linux/workqueue.h>
#include <asm/uaccess.h>
#include <asm/atomic.h>


static int my_open( struct inode *inode, struct file *file );
static ssize_t my_read( struct file *file, char __user *userbuf, size_t lbuf, loff_t *pos );
static ssize_t my_write( struct file *file, const char __user *userbuf, size_t lbuf, loff_t *pos );
static int my_close( struct inode *inode, struct file *file );
static loff_t myllseek( struct file *file, loff_t pos, int whence);
static void t_func( struct work_struct *w );

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


static struct file_operations fops =
{
	.owner = THIS_MODULE,
	.open = my_open,
	.read = my_read,
	.release = my_close,
	.write = my_write,
	.llseek = myllseek,
};

static struct myworkqueue {
	int a;
	int b;
	struct work_struct work;
}myworkqueue_t;


static void t_func( struct work_struct *w )
{
	struct myworkqueue *myw = container_of( w, struct myworkqueue, work );

		pr_info( "Function of workqueue executed at %ld jiffies, a-> %d, b-> %d\n", jiffies, myw->a, myw->b );
}

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
		
		printk( KERN_INFO "Current reference count: %ld - %d times opened\n", module_refcount( THIS_MODULE ), count );		
		return 0;
}

 static ssize_t my_write( struct file *file, const char __user *userbuf, size_t lbuf, loff_t *pos )
{
	ssize_t m_bytes;
	ssize_t w_bytes;
	int ret;

		pr_info( "Scheluding a workqueue: %d - %s [ at %ld jiffies ]\n", current->pid, current->comm, jiffies );
		schedule_work( &myworkqueue_t.work );

		m_bytes = b_size - *pos;
		m_bytes = m_bytes > lbuf ? lbuf : m_bytes;
				
		w_bytes = m_bytes - copy_from_user( kbuff + *pos, userbuf, lbuf );
		*pos += w_bytes;
		p_kbuff += w_bytes;

		ret = w_bytes;
		pr_info( "Write: Pos [%lld] - Bytes written %ld\n", *pos, w_bytes );

		return ret;
} 

static ssize_t my_read( struct file *file, char __user *userbuf, size_t lbuf, loff_t *pos )
{
	ssize_t m_bytes;
	ssize_t r_bytes;
	int ret;

		if( *pos >= ( p_kbuff - kbuff ) )
			return 0;

		m_bytes = b_size - *pos;
		m_bytes = m_bytes > lbuf ? lbuf : m_bytes;
		r_bytes = m_bytes - copy_to_user( userbuf, kbuff + *pos, lbuf );
		*pos += r_bytes;

		ret = r_bytes;
		pr_info( "Read: Pos [%lld] - Bytes read %ld\n", *pos, r_bytes );

		return ret;
}

static int __init my_load( void )
{
	int ret;

		pr_info( "Init at: %p\n", my_load );

		ret = alloc_chrdev_region( &dev, 0, 1, myname );
		mydev = cdev_alloc();
		cdev_init( mydev, &fops );
		cdev_add( mydev, dev, 1 );

		p_kbuff = kbuff = kmalloc( PAGE_SIZE, GFP_KERNEL );
		b_size = PAGE_SIZE;
		INIT_WORK( &myworkqueue_t.work, t_func );

		myworkqueue_t.a = 10;
		myworkqueue_t.b = 5;
	
		printk( KERN_INFO "Please create the proper device: Major-> %d Minor-> %d with ( %s ) name\n", MAJOR( dev ), MINOR( dev ), myname );

		return 0;
		 
}

static void __exit my_unload( void )
{
	unregister_chrdev_region( dev, 1 );
	cdev_del( mydev );
	kfree( kbuff );
	printk( KERN_INFO "Device unregistered, please remove the proper entry ( %s )\n", myname );
	pr_info( "Unload at: %p\n", my_unload );
}

module_init( my_load );
module_exit( my_unload );
MODULE_AUTHOR( "Oscar Salvador" );
MODULE_DESCRIPTION( "workqueue" );
MODULE_LICENSE( "GPL v2" );
