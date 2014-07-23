#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/kernel.h>
#include <asm/uaccess.h>
#include <linux/cdev.h>
#include <linux/slab.h>
#include <linux/mm.h>
#include <linux/io.h>

#define MYDEV_GET_BUFF_SIZE 1
#define MYDEVTYPE 'k'

static dev_t dev;
static char *myname = "mymmap";

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
static int mymmap( struct file *file, struct vm_area_struct *vma );
static long myioctl( struct file *file, unsigned int cmd, unsigned long p );
static loff_t myllseek( struct file *file, loff_t pos, int whence);

static struct cdev *mydev;

static struct file_operations fops =
{
	.owner = THIS_MODULE,
	.open = my_open,
	.read = my_read,
	.release = my_close,
	.write = my_write,
	.mmap = mymmap,
	.unlocked_ioctl = myioctl,
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

static long myioctl( struct file *file, unsigned int cmd, unsigned long p )
{
	unsigned long nr;
	unsigned long ret = 0;
	int nbytes;

		nr = _IOC_NR( cmd );
		switch( nr )
		{
			case MYDEV_GET_BUFF_SIZE:
				nbytes = copy_to_user( (void *)p, &b_size, sizeof( b_size ) );
				break;

			default:
				ret = -EINVAL;
				break;
		}

		return ret;
}

static int mymmap( struct file *file, struct vm_area_struct *vma )
{
	int ret = 0;
	unsigned long offset = vma->vm_pgoff << PAGE_SHIFT;
	unsigned long len = vma->vm_end - vma->vm_start;
	unsigned long pfn;

		pr_info("%s: mapping %ld bytes of ramdisk at offset %ld to addr 0%lx\n", __stringify(KBUILD_BASENAME), len, offset, vma->vm_start);

		pfn = virt_to_phys( kbuff ) >> PAGE_SHIFT;
		pr_info( "Kernel addr memory shift -> 0%lx - 0%lx\n", pfn, pfn );

		if( remap_pfn_range( vma, vma->vm_start, pfn, len,  vma->vm_page_prot ) )
			ret = -EAGAIN;

		return ret; 
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
	ssize_t nbytes;
	int ret;

		if( ( lbuf + *pos ) > b_size )
			ret = 0;
		else
		{
			nbytes = lbuf - copy_from_user( kbuff + *pos, userbuf, lbuf );
			*pos += nbytes;
			p_kbuff += nbytes;
			ret = nbytes;
		}

		return ret;
} 

static ssize_t my_read( struct file *file, char __user *userbuf, size_t lbuf, loff_t *pos )
{
	ssize_t nbytes;
	int ret;
	int p_p = p_kbuff - kbuff;

		if( ( lbuf + *pos ) > p_p )
			ret = 0;
		else
		{
			nbytes = lbuf - copy_to_user( userbuf, kbuff + *pos, lbuf );
			*pos += nbytes;

			ret = nbytes;
		}

		return ret;
}

static int __init my_load( void )
{
	int ret;

		ret = alloc_chrdev_region( &dev, 0, 1, myname );
		mydev = cdev_alloc();
		cdev_init( mydev, &fops );
		cdev_add( mydev, dev, 1 );

		kbuff = kmalloc( PAGE_SIZE, GFP_KERNEL );
		b_size = PAGE_SIZE;
		pr_info( "Kbuf allocated at %p\n", kbuff );

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

