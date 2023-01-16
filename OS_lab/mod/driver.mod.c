#include <linux/module.h>
#define INCLUDE_VERMAGIC
#include <linux/build-salt.h>
#include <linux/elfnote-lto.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

BUILD_SALT;
BUILD_LTO_INFO;

MODULE_INFO(vermagic, VERMAGIC_STRING);
MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__section(".gnu.linkonce.this_module") = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

#ifdef CONFIG_RETPOLINE
MODULE_INFO(retpoline, "Y");
#endif

static const struct modversion_info ____versions[]
__used __section("__versions") = {
	{ 0xf704969, "module_layout" },
	{ 0xd0da656b, "__stack_chk_fail" },
	{ 0x7c797b6, "kmem_cache_alloc_trace" },
	{ 0xd731cdd9, "kmalloc_caches" },
	{ 0x6b10bee1, "_copy_to_user" },
	{ 0x97651e6c, "vmemmap_base" },
	{ 0xc2250af7, "get_pid_task" },
	{ 0x479052b, "find_get_pid" },
	{ 0xa648e561, "__ubsan_handle_shift_out_of_bounds" },
	{ 0x7cd8d75e, "page_offset_base" },
	{ 0x1d19f77b, "physical_mask" },
	{ 0xdad13544, "ptrs_per_p4d" },
	{ 0xa92ec74, "boot_cpu_data" },
	{ 0x72d79d83, "pgdir_shift" },
	{ 0xc946dda0, "cdev_del" },
	{ 0x82e7bb9c, "device_destroy" },
	{ 0x6091b333, "unregister_chrdev_region" },
	{ 0x933c4a18, "class_destroy" },
	{ 0xefc94da8, "device_create" },
	{ 0x325cb5cb, "__class_create" },
	{ 0xc378cea7, "cdev_add" },
	{ 0x2d725fd4, "cdev_init" },
	{ 0xe3ec2f2b, "alloc_chrdev_region" },
	{ 0x92997ed8, "_printk" },
	{ 0xbdfb6dbb, "__fentry__" },
	{ 0x5b8239ca, "__x86_return_thunk" },
	{ 0x4c55f5ef, "pv_ops" },
};

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "72CBC5A9DB3F325D9A7091A");
