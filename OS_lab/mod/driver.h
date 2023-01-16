#ifndef __IOCTL_H
#define __IOCTL_H

#include <linux/ioctl.h>
#include <linux/module.h>

struct user_page {
    unsigned long flags;
    unsigned long page_vir_addr;
    unsigned long page_phys_addr;
    unsigned long page_offset;
    int _refcount;
};

struct user_thread_struct {
    unsigned long		stack_pointer;
    unsigned short		es;
    unsigned short		ds;
    unsigned short		fsindex;
	unsigned short		gsindex;
    unsigned long		fsbase;
	unsigned long		gsbase;
    unsigned int		last_cpu;
	unsigned long		avx512_timestamp;
};

#define IOCTL_BASE 'i'
#define RD_PAGE _IOR(IOCTL_BASE, 1, struct user_page*)
#define RD_THREAD_STRUCT _IOR(IOCTL_BASE, 2, struct user_thread_struct*)

#endif
