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
	{ 0x27e1a049, "printk" },
	{ 0xc2a3e73d, "debugfs_create_file" },
	{ 0xd5f8162f, "debugfs_create_dir" },
	{ 0x5a5e7ea3, "simple_read_from_buffer" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";

