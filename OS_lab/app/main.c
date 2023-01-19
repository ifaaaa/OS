#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include<sys/ioctl.h>
#include "../mod/driver.h"
 
int main()
{
        int fd;
        int32_t value, number;
        printf("*****************************************************\n");
        printf("*******Byeibitkhan Khalifa ioctl:page, thread_struct*******\n");
 
        printf("\n*[Opening Driver]*\n");
        fd = open("/dev/etx_device", O_RDWR);
        if(fd < 0) {
                printf("Cannot open device file...\n");
                return 0;
        }
 
        printf("*[Reading PAGE from Driver]*\n");
        struct user_page user_page;
        if(ioctl(fd, RD_PAGE, &user_page) < 0)
            printf("ERR");
        else {
            printf("-----PAGE-----\n%p\n", &user_page);
            printf("flags = %lu\n", user_page.flags);
            printf("page virtual address = %lx\n", user_page.page_vir_addr);
            printf("page physical address = %lx\n", user_page.page_phys_addr);
            printf("page offset = %lx\n", user_page.page_offset);
            printf("count = %u\n", user_page._refcount);
        }
 
        printf("\n\n*[Reading THREAD_STRUCT from Driver]*\n");
        struct user_thread_struct user_thread;
        if(ioctl(fd, RD_THREAD_STRUCT, &user_thread) < 0)
            printf("ERR");
        else {
            printf("-----USER_THREAD-----\n%p\n", &user_thread);
            printf("stack pointer = %lu\n", user_thread.stack_pointer);
            printf("es = %hu\n", user_thread.es);
            printf("ds = %hu\n", user_thread.ds);
            printf("fsindex = %hu\n", user_thread.fsindex);
            printf("gsindex = %hu\n", user_thread.gsindex);
            printf("fsbase = %lu\n", user_thread.fsbase);
            printf("gsbase = %lu\n", user_thread.gsbase);
            printf("last cpu = %hi\n", user_thread.last_cpu);
            printf("avx512 timestamp = %lu\n", user_thread.avx512_timestamp);
        }

        printf("\n*[Closing Driver]*\n");
        printf("*****************************************************\n");

        close(fd);
}
