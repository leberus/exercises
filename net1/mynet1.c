#include <linux/init.h>
#include <linux/module.h>
#include <linux/netdevice.h>

static void mysetup(struct net_device *mydev);
static int mynet_open(struct net_device *mydev);
static int mynet_close(struct net_device *mydev);

struct mynet_struct
{
	int a;
	int b;
};

static struct mynet_struct mydev_t; 
static struct net_device *mydev;

static void mysetup(struct net_device *d)
{
	ether_setup (d);
/*	d->netdev_ops.ndo_open = mynet_open;
	d->netdev_ops.ndo_stop = mynet_close; */
}

static int mynet_open(struct net_device *p)
{
	pr_info("Activating the interface %s...\n", p->name);
	return 0;
}

static int mynet_close(struct net_device *b)
{
	pr_info("Disabling the interface %s...\n", b->name);
	return 0;
}

static __init int my_init(void)
{
	int ret = 0;

		mydev = alloc_netdev(sizeof(mydev_t), "mynet1", mysetup);

		if( mydev )
			ret = register_netdev(mydev);
		else {
			pr_info("Failed allocating netdev");
			ret = -EINVAL;
		}

		return ret;		
}

static __exit void my_exit(void)
{
	unregister_netdev(mydev);
	free_netdev(mydev);
	pr_info("Device disabled\n");
}

module_init(my_init);
module_exit(my_exit);

