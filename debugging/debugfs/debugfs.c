#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/debugfs.h>
#include <asm/user.h>

static ssize_t my_read( struct file *file, char __user *buf, size_t count, loff_t *ppos );
static ssize_t my_write( struct file *file, const char __user *buf, size_t count, loff_t *ppos );

static const char *dname = "debug_test_d";
static const char *fname = "debug_test_f";

static char kstring[] = "test";

struct file_operations fops = 
{
	.owner = THIS_MODULE,
	.read = my_read,
	.write = my_write,
};

static struct dentry *d;
static struct dentry *f;

static ssize_t my_read( struct file *file, char *buf, size_t count, loff_t *ppos )
{
	return simple_read_from_buffer( buf, count, ppos, kstring, 5 );
}

static ssize_t my_write( struct file *file, const char *buf, size_t count, loff_t *ppos )
{
	return -EINVAL;
}

static int __init my_init( void )
{
	d = debugfs_create_dir( dname, NULL );
	if( d )
		f = debugfs_create_file( fname, 0666, d, NULL, &fops );
		if( !f )
			pr_info( "Debugfs file entry could not be created\n" );

	return 0;
}

static void __exit my_exit( void )
{
	pr_info( "Exiting..\n" );
}

module_init( my_init );
module_exit( my_exit );

MODULE_AUTHOR("Oscar Salvador");
MODULE_DESCRIPTION("debufgs");
MODULE_LICENSE("GPL v2");
