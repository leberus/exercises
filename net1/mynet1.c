#include <linux/init.h>
#include <linux/module.h>
#include <linux/netdevice.h>
#include <linux/kallsyms.h>
#include <linux/kprobes.h>

static void mysetup(struct net_device *d);
static int mynet_open(struct net_device *p);
static int mynet_close(struct net_device *b);

static struct net_device *mydev;

/*
	Debugging purposes
*/

static struct jprobe jp;
static const char *f_to_debug = "register_netdev";

static int debugof_register_netdev( struct net_device *dev )
{

	pr_info("Here!\n");
	if( dev->netdev_ops == NULL )
		pr_info("It's NULL\n");
	else
		pr_info("It's NOT NULL\n");

	jprobe_return();
	return 0;
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

static struct net_device_ops ndo = {
        .ndo_open = mynet_open,
        .ndo_stop = mynet_close,
        //.ndo_start_xmit = stub_start_xmit,
};

static void mysetup(struct net_device *d)
{
        int j;

                for(j = 0 ; j < ETH_ALEN ; j++)
                        d->dev_addr[j] = (char)j;

                ether_setup (d);
		d->netdev_ops = &ndo;

		pr_info("Setted up ( %s )\n", d->name );
}



static int __init my_init(void)
{
	int ret = 0;
	
		jp.kp.addr = (kprobe_opcode_t *)register_netdev;
		jp.entry = (kprobe_opcode_t *)kallsyms_lookup_name("debugof_register_netdev");
		register_jprobe(&jp);

		mydev = alloc_netdev(0, "mynet%d", mysetup);

		pr_info("Addr of register_netdev: %x\n", (int)jp.kp.addr);
	
		if( register_netdev(mydev) ) {
			pr_debug("Failed allocating netdev");
			free_netdev(mydev);
			ret = -EINVAL;
		}	
		pr_info("Sucees!\n");

		return ret;		
}

static void __exit my_exit(void)
{
	unregister_netdev(mydev);
	free_netdev(mydev);
	unregister_jprobe(&jp);
	pr_info("Device disabled\n");
}

module_init(my_init);
module_exit(my_exit);

MODULE_DESCRIPTION("ethernet");
MODULE_LICENSE("GPL v2");

