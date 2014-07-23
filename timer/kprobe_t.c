#include <linux/module.h>
#include <linux/init.h>
#include <linux/kprobes.h>

static struct kprobe kt[2];

int k_addtimer( struct kprobe *k, struct pt_regs *r )
{
	static unsigned long counter = 0;
		
		counter++;

		if( printk_ratelimit() )
			pr_info( "add_timer called: %ld\n", counter );

		return 0;
}

int k_deltimer( struct kprobe *k, struct pt_regs *r )
{
	static unsigned long counter = 0;

		counter++;
			
		if( printk_ratelimit() )
			pr_info( "del_timer called: %ld\n", counter );

		return 0;
}

static int __init my_init( void )
{
	kt[0].symbol_name = "add_timer";
	kt[0].pre_handler = k_addtimer;

	kt[1].symbol_name = "del_timer";
	kt[1].pre_handler = k_deltimer;

	register_kprobe( &kt[0] );
	register_kprobe( &kt[1] );
	
	return 0;
}

static void my_exit( void )
{
	unregister_kprobe( &kt[0] );
	unregister_kprobe( &kt[1] );
	pr_info( "Exiting..\n" );
}

module_init( my_init );
module_exit( my_exit );

MODULE_AUTHOR("Oscar Salvador");
MODULE_DESCRIPTION("kprobe_timer");
MODULE_LICENSE("GPL v2");
