#include <linux/module.h>
#include <linux/init.h>
#include <linux/ioport.h>

#define START 0xE0
#define LONG 0x9
#define END START+LONG

/*unsigned long addr = 0xE0;
unsigned long extend = 0x9; */

static int __init my_load( void )
{
	struct resource *p;
        int ret = 0;
	unsigned long b = 100;
		
		p = request_region( START, LONG, "oscar" );
		if( !p )
		{
			pr_info( "Fail!\n" );
			ret = -1;
		}

		outb( b, START );
		pr_info( "Write: %ld\n", b );

		b = inb( END );
		pr_info( "Read: %ld\n", b );

                return ret;
}

static void __exit my_unload( void )
{
	release_region( START, LONG );
}

module_init( my_load );
module_exit( my_unload );

MODULE_LICENSE("GPL v2");

