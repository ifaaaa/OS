#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/slab.h>                 
#include <linux/uaccess.h>              
#include <linux/ioctl.h>
#include <linux/err.h>
#include "linux/mm.h"
#include "driver.h"

int32_t value_pid = 1;
 
dev_t dev = 0;
static struct class *dev_class;
static struct cdev etx_cdev;

/*
** Function Prototypes
*/
static int      __init etx_driver_init(void);
static void     __exit etx_driver_exit(void);
static int      etx_open(struct inode *inode, struct file *file);
static int      etx_release(struct inode *inode, struct file *file);
static long     etx_ioctl(struct file *file, unsigned int cmd, unsigned long arg);

/*
** File operation sturcture
*/
static struct file_operations fops =
{
        .owner          = THIS_MODULE,
        .open           = etx_open,
        .unlocked_ioctl = etx_ioctl,
        .release        = etx_release,
};

/*
** This function will be called when we open the Device file
*/
static int etx_open(struct inode *inode, struct file *file)
{
        pr_info("Device File Opened...!!!\n");
        return 0;
}

/*
** This function will be called when we close the Device file
*/
static int etx_release(struct inode *inode, struct file *file)
{
        pr_info("\nDevice File Closed...!!!\n");
        return 0;
}

pte_t *get_pte_by_mm_and_address(struct mm_struct* mm, long address) {
        pgd_t *pgd;
        p4d_t* p4d;
        pud_t *pud;
        pmd_t *pmd;
        pte_t *pte;
        struct page *page = NULL;

        pgd = pgd_offset(mm, address);
        if (!pgd_present(*pgd)) 
                return NULL;
        
        p4d = p4d_offset(pgd, address);
        if (!p4d_present(*p4d)) 
                return NULL;

        pud = pud_offset(p4d, address);
        if (!pud_present(*pud)) 
                return NULL;

        pmd = pmd_offset(pud, address);
        if (!pmd_present(*pmd)) 
                return NULL;
        
        pte = pte_offset_kernel(pmd, address);
        if (!pte_present(*pte)) 
                return NULL;

        return pte;
}
/*
** This function will be called when we write IOCTL on the Device file
*/
static long etx_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
         switch(cmd) {
                case RD_PAGE: {
                        pr_info("\nRead PAGE started !!!");

                        struct pid* pid = find_get_pid(value_pid);
                        struct task_struct* task = get_pid_task(pid, PIDTYPE_PID);
                        struct mm_struct* mm = task->mm;
                        struct vm_area_struct *vm_current;
                        struct page* kernel_page;

                        vm_current = mm->mmap;
                        long start = vm_current->vm_start;
                        long end = vm_current->vm_end;
                        long vaddr = start;
                        pte_t *pte;

                        while (vaddr <= end) {
                                pte = get_pte_by_mm_and_address(mm, vaddr);
                                // kernel_page = get_page_by_mm_and_address(mm, address);
                                kernel_page = pte_page(*pte);
                                if (kernel_page != NULL){
                                        break;
                                }
                                vaddr += PAGE_SIZE;
                        }
                        unsigned long page_addr = pte_val(*pte) & PAGE_MASK;
                        unsigned long page_offset = vaddr & ~PAGE_MASK;
                        unsigned long paddr = page_addr | page_offset;

                        struct user_page user_page = {
                             .flags = kernel_page->flags,
                             ._refcount = kernel_page->_refcount.counter,
                             .page_vir_addr = vaddr,
                             .page_phys_addr = paddr,
                             .page_offset = page_offset
                        };

                        if(copy_to_user((struct my_page *) arg, &user_page, sizeof(user_page))){
                            pr_err("PAGE Read : Err!\n");
                        }            

                        pr_info("Read PAGE finished");
                        break;
                }
                case RD_THREAD_STRUCT: {
                        pr_info("\nRead THREAD_STRUCT started !!!");

                        struct thread_struct* kernel_thread = kmalloc(sizeof(struct thread_struct), GFP_KERNEL);
                        struct pid* pid = find_get_pid(value_pid);
                        struct task_struct* task = get_pid_task(pid, PIDTYPE_PID);
                        kernel_thread = &(task->thread);
                        struct fpu* kernel_fpu = &(kernel_thread->fpu);

                        struct user_thread_struct user_thread = {
                                .stack_pointer = kernel_thread->sp,
                                .es = kernel_thread->es,
                                .ds = kernel_thread->ds,
                                .fsindex = kernel_thread->fsindex,
                                .gsindex = kernel_thread->gsindex,
                                .fsbase = kernel_thread->fsbase,
                                .gsbase = kernel_thread->gsbase,
                                .last_cpu = kernel_fpu->last_cpu,
                                .avx512_timestamp = kernel_fpu->avx512_timestamp
                        };

                        if(copy_to_user((struct user_thread_struct *) arg, &user_thread, sizeof(user_thread))){
                            pr_err("THREAD_STRUCT Read : Err!\n");
                        }            

                        pr_info("Read THREAD_STRUCT finished");
                        break;
                }
                default:
                        pr_info("Default\n");
                        break;
        }
        return 0;
}
 
/*
** Module Init function
*/
static int __init etx_driver_init(void)
{
        /*Allocating Major number*/
        if((alloc_chrdev_region(&dev, 0, 1, "etx_Dev")) <0){
                pr_err("Cannot allocate major number\n");
                return -1;
        }
        pr_info("Major = %d Minor = %d \n",MAJOR(dev), MINOR(dev));
 
        /*Creating cdev structure*/
        cdev_init(&etx_cdev,&fops);
 
        /*Adding character device to the system*/
        if((cdev_add(&etx_cdev,dev,1)) < 0){
            pr_err("Cannot add the device to the system\n");
            goto r_class;
        }
 
        /*Creating struct class*/
        if(IS_ERR(dev_class = class_create(THIS_MODULE,"etx_class"))){
            pr_err("Cannot create the struct class\n");
            goto r_class;
        }
 
        /*Creating device*/
        if(IS_ERR(device_create(dev_class,NULL,dev,NULL,"etx_device"))){
            pr_err("Cannot create the Device 1\n");
            goto r_device;
        }
        pr_info("\n[---------Device Driver Insert...Done!!!---------]\n");
        return 0;
r_device:
        class_destroy(dev_class);
r_class:
        unregister_chrdev_region(dev,1);
        return -1;
}

/*
** Module exit function
*/
static void __exit etx_driver_exit(void)
{
        device_destroy(dev_class,dev);
        class_destroy(dev_class);
        cdev_del(&etx_cdev);
        unregister_chrdev_region(dev, 1);
        pr_info("[---------Device Driver Remove...Done!!!---------]\n\n");
}
 
module_init(etx_driver_init);
module_exit(etx_driver_exit);
 
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Le Ngoc Thien");
MODULE_DESCRIPTION("Linux device driver to read PAGE and THREAD_STRUCT from kernel (IOCTL)");
MODULE_VERSION("1.0");