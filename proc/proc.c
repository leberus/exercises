#include <linux/module.h>
#include <linux/init.h>
#include <linux/proc_fs.h>
#include <asm/uaccess.h>
#include <linux/sched.h>
#include <linux/signal.h>


static struct proc_dir_entry *myproc;
static struct task_struct *tsk;
static const char *proc_name = "killme";
static char *kbuff;
static char *pbuff;

int myreadproc( char *buf, char **start, off_t off, int count, int *eof, void *data )
{
	unsigned long bytes;

		bytes = pbuff - kbuff;
		
		if( pbuff != kbuff )
			memcpy( buf, kbuff, bytes );

		*eof = 1;
		return bytes;
}

int mywriteproc( struct file *file, const char __user *buffer, unsigned long count, void *data )
{
	int nbytes;
	int pid;
	int ret;
	int signal = 9;

		pbuff = kbuff + count;
		nbytes = copy_from_user( kbuff, buffer, count );

		sscanf(kbuff, "%d", &pid);
		tsk = pid_task( find_vpid( pid ), PIDTYPE_PID );
		pr_info( "Sending signal %d to %s [ %d ]\n", signal, tsk->comm, tsk->pid );
		ret = send_sig( signal, tsk, 0 );

		return count;
}

		

static __init int myinit( void )
{
	myproc = create_proc_entry( proc_name, S_IRUGO | S_IWUSR, NULL );
	myproc->read_proc = myreadproc;
	myproc->write_proc = mywriteproc; 
	pbuff = kbuff = kmalloc( 1024, GFP_KERNEL );

	return 0;
}

static __exit void myexit( void )
{
	remove_proc_entry( proc_name, NULL );
	pr_info( "Exiting proc..\n" );
}

module_init( myinit );
module_exit( myexit );
MODULE_LICENSE("GPL v2");

