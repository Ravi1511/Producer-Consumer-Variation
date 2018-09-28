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
#define MAX 5
#define num 10

struct pc
{
	int arrival_time;
        int service_time;
};
struct pc q1[num];
struct pc key;
sem_t full,empty;
int service_time[num];
int queue[MAX];
int front=0,rear=0;
int flag=0,slept=0;
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
                                printf("\nCustomer %d is added to the queue",i);
                            }
                     }
                     pthread_mutex_unlock(&mutex);
		    sem_post(&full);
           
       }
        sem_post(&full);
            
}
void* consumer1()
{
      int item=0;
      do
      {
              
                      sem_wait(&full);
		      
                      pthread_mutex_lock(&mutex);
                      if(front==rear)
                      {
                            printf(GREEN"\nWaiting queue is empty,Service personnels sleeps" RESET);
                            slept=1;
                            pthread_mutex_unlock(&mutex);
                      }
                      else
                      {
                              item=queue[front];
		              printf(BLUE"\nCustomer %d is being serviced by service personnel 1"RESET,queue[front]);
			      front=(front+1)%MAX;
                              pthread_mutex_unlock(&mutex);
                              if(item==num)
                                  flag=1;
		              sleep(service_time[item]);
                      }
                      
		      sem_post(&empty);             
              
      }while(item!=num&&flag!=1);
      flag=1; 
          
     
}
void* consumer2()
{
      int item=0;
      do
      {
              
                      sem_wait(&full);
		      
                      pthread_mutex_lock(&mutex);
                      if(front==rear)
                      {
                            printf(GREEN"\nWaiting queue is empty,Service personnels sleeps" RESET);
                            pthread_mutex_unlock(&mutex);
                      }
                      else
                      {
                              item=queue[front];
		              printf(YELLOW"\nCustomer %d is being serviced by service personnel 2"RESET,queue[front]);
			      front=(front+1)%MAX;
                              pthread_mutex_unlock(&mutex);
                              if(item==num)
                                  flag=1;
		              sleep(service_time[item]);
                      }
                      
		      sem_post(&empty);             
              
      }while(item!=num&&flag!=1);
      flag=1;
      
}
int main(int argc,char** argv)
{
	int i,j,min=0;
	pthread_t p,c1,c2;
             sem_init(&empty, 0, MAX);
             sem_init(&full, 0, 0);
             pthread_mutex_init(&mutex, NULL);
             srand(time(0));
             for(i=0;i<num;i++)
                   service_time[i]=rand()%10;
                  printf("\nLength of waiting queue is %d",MAX-1);
                  printf("\nMax number of customers  is %d",num);
                  printf("\n-----------------Waiting queue empty ,counter is CLOSED---------------");
                  printf("\nCustomer 1 comes,wakes up the service-personnel");
                  pthread_create(&p,NULL,producer,NULL);    
		  pthread_create(&c1,NULL,consumer1,NULL);
                  pthread_create(&c2,NULL,consumer2,NULL);
		  pthread_join(p,NULL);
		  pthread_join(c1,NULL);
                  pthread_join(c2,NULL);
                  printf("\n------------------COUNTER CLOSED FOR TODAY----------------------------");
}
