#include <linux/init.h>
#include <linux/module.h>
#include <linux/netdevice.h>

static __init int my_init(void)
{

}

static __exit void my_exit(void)
{

}

module_init(my_init);
module_exit(my_exit);

