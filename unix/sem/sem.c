#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#define NUM 100

int queue[NUM];                         //全局数组实现环形队列
sem_t blank_number, product_number;     //空格数,产品数

void *producer(void *arg)
{
    int i = 0;
    while (1)
    {
        sem_wait(&blank_number);        //空格数减少(等于0则阻塞,没有空位生产产品了)
        queue[i] = rand() % 1000 + 1;
        printf("---Produce---%d\n", queue[i]);
        sem_post(&product_number);      //产品数增加

        i = (i + 1) % NUM;              //实现环形
        sleep(rand() % 1);
    }
}

void *consumer(void *arg)
{
    int i = 0;
    while (1)
    {
        sem_wait(&product_number);      //产品数减少(等于0就阻塞,没有产品可以消费了)
        printf("---Consumer---%d\n", queue[i]);
        queue[i] = 0;
        sem_post(&blank_number);        //空格数增加

        i = (i + 1) % NUM;
        sleep(rand() % 3);
    }
}

int main(int argc, char *argv[])
{
    pthread_t pid, cid;

    sem_init(&blank_number, 0, NUM);
    sem_init(&product_number, 0, 0);

    pthread_create(&pid, NULL, producer, NULL);
    pthread_create(&cid, NULL, consumer, NULL);

    pthread_join(pid, NULL);
    pthread_join(cid, NULL);

    sem_destroy(&blank_number);
    sem_destroy(&product_number);

    return 0;
}

