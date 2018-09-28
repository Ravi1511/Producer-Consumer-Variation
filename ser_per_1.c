#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#define RED "\x1b[31m"
#define GREEN "\x1b[32m"
#define BLUE "\x1b[34m"
#define YELLOW "\x1b[33m"
#define RESET "\x1b[0m"
#define MAX 4
#define num 15

struct pc
{
	int arrival_time;
        int service_time;
};
struct pc q1[num];
struct pc key;
int service_time[num];
sem_t full,empty;
int queue[MAX];
int front=0,rear=0;
int total=0;
int flag=0,last=0,slept=0;
pthread_mutex_t mutex;
void* producer()
{      
       for(int i=1;i<=num;i++)
       {
		    sem_wait(&empty);
                    pthread_mutex_lock(&mutex);
                    if((front==0 && rear==MAX-1)||(rear==(front-1)%MAX))
                    {
                       printf(RED "\nQueue is full,cutomer %d leaves" RESET,i);
                       if(i==num)
                          last=1;
                    }
                    else
                    {
			    queue[rear]=i;
			    rear=(rear+1)%MAX;
		             if(slept==1)
                            {
			       printf("\nCustomer %d arrives and wakes up service personnel",i);
                               slept=0;
                            }else
                            {
                                printf(BLUE "\nCustomer %d is added to the queue"RESET,i);
                            }
			    
                     }
                     pthread_mutex_unlock(&mutex);
		    sem_post(&full);
           
       }
        sem_post(&full);
            
}
void* consumer()
{
      int item=0;
      do
      {
              
                      sem_wait(&full);
		      
                      pthread_mutex_lock(&mutex);
                      if(front==rear)
                      {
                            printf(GREEN"\nWaiting queue is empty,Service personnel sleeps" RESET);
                            slept=1;
                      }
                      else
                      {
                              item=queue[front];
		              printf(YELLOW"\nCustomer %d is being serviced"RESET,queue[front]);
			      front=(front+1)%MAX;
                              if(last==1&&item==num-1)
                                   flag=1;
                              sleep(service_time[item]);
		              
                      }
                      pthread_mutex_unlock(&mutex);
		      sem_post(&empty);             
              
      }while(item!=num&&flag!=1);
     
     
}
int main(int argc,char** argv)
{
	int i,j,min=0;
	pthread_t p,c;
             sem_init(&empty, 0, MAX);
             sem_init(&full, 0, 0);
             pthread_mutex_init(&mutex, NULL);
             srand(time(0));
             for(i=0;i<num;i++)
                   service_time[i]=rand()%5;
                  printf("\nLength of waiting queue is %d",MAX-1);
                  printf("\nMax number of customers  is %d",num);
                  printf("\n-----------------Waiting queue empty ,counter is CLOSED---------------");
                  printf("\nCustomer 1 comes,wakes up the service-personnel");
                  pthread_create(&p,NULL,producer,NULL);    
		  pthread_create(&c,NULL,consumer,NULL);
		  pthread_join(p,NULL);
		  pthread_join(c,NULL);
                   printf("\n------------------COUNTER CLOSED FOR TODAY----------------------------");
}
