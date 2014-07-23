#include <linux/module.h>
#include <linux/init.h>
#include <linux/kallsyms.h>
#include <linux/kprobes.h>

static struct jprobe jp;

static char *name = "my_close";

int my_monit_function( struct inode *inode, struct file *file )
{
	pr_info( "In my monit function\n" );
	jprobe_return();
	return 0;
}

static int __init my_init( void )
{
	jp.kp.addr = ( kprobe_opcode_t * )kallsyms_lookup_name( name );
	jp.entry = ( kprobe_opcode_t * )kallsyms_lookup_name( "my_monit_function" );
	register_jprobe( &jp );

	return 0;
}

static void __exit my_exit( void )
{
	unregister_jprobe( &jp );
	pr_info("Exit..\n");
}

module_init( my_init );
module_exit( my_exit );

MODULE_AUTHOR("Oscar Salvador");
MODULE_DESCRIPTION("jprobe");
MODULE_LICENSE("GPL v2");
