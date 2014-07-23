#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);

struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
 .name = KBUILD_MODNAME,
 .init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
 .exit = cleanup_module,
#endif
 .arch = MODULE_ARCH_INIT,
};

static const struct modversion_info ____versions[]
__used
__attribute__((section("__versions"))) = {
	{ 0x568fba06, "module_layout" },
	{ 0x364c8cd1, "misc_deregister" },
	{ 0xf20dabd8, "free_irq" },
	{ 0xe523ad75, "synchronize_irq" },
	{ 0xcdca3691, "nr_irqs" },
	{ 0xd6b8e852, "request_threaded_irq" },
	{ 0x37a0cba, "kfree" },
	{ 0x6633eeb3, "misc_register" },
	{ 0x7a172b67, "kmem_cache_alloc_trace" },
	{ 0x4d884691, "malloc_sizes" },
	{ 0x71de9b3f, "_copy_to_user" },
	{ 0xa1c76e0a, "_cond_resched" },
	{ 0x27e1a049, "printk" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";

