#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#ifndef __GNUC__
    #error please use gcc to compile the file.
#endif

#if __GNUC__<4 || (__GNUC__==4 && __GNUC_MINOR__<1)
    #error GCC version must be greater or equal than 4.1.2
#endif
/*
 *     ���������������������������������������������Щ�������������������������������������
 *     ��          ����       ��   �����������   ��
 *     ���������������������������������������������੤������������������������������������
 *     ��    �Ϳ�������       ��  ����ʹ��������  ��
 *     ���������������������������������������������ة�������������������������������������
 *     ��    ��������         ��  ����ʹ��������  ��
 *     ���������������������������������������������ة�������������������������������������
 *     ��    ���ڼ���         ��  ����ʹ�û�����  ��
 *     ���������������������������������������������ة�������������������������������������
 *     ��    ��������Ҫ����   ��  ʹ�û�����      ��
 *     ���������������������������������������������ة�������������������������������������
 */

#define my_spinLock_init(name) volatile long name

static __attribute__((always_inline)) void my_spinLock(volatile long* exclusion)
{
    while (__sync_lock_test_and_set(exclusion, 1))
        while (*exclusion)
            ;
}

static __attribute__((always_inline)) void my_spinUnlock(volatile long* exclusion)
{
    __sync_synchronize();
    *exclusion = 0;
}



/***********************************tsst code start*********************************************/


static int counter = 0;

my_spinLock_init(conuter_slock);

void* count_handle_func(void * data)
{
    int i;
    for(i = 0; i < 100000;i++)
    {
        my_spinLock(&conuter_slock);
        counter++;
        my_spinUnlock(&conuter_slock);
    }
} 


int main(int argc,char** argv)
{
    pthread_t thread_id1,thread_id2,thread_id3;
    
    pthread_create(&thread_id1,NULL,count_handle_func,NULL);
    pthread_create(&thread_id2,NULL,count_handle_func,NULL);
    pthread_create(&thread_id3,NULL,count_handle_func,NULL);
    
    pthread_join(thread_id1,NULL);
    pthread_join(thread_id2,NULL);
    pthread_join(thread_id3,NULL);
    
    printf("spin_lock test [%s]\n",counter == 300000? "OK": "NG");
}



/************************************tset code end**********************************************/

