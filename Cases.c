#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

struct pc
{
  int arrival_time;
  int service_time;
};
//----Global Variable and Function Declaration---------------------
int counter=0;
pthread_mutex_t buffer_mutex;
sem_t fillCount1, emptyCount1,fillCount2,emptyCount2,fillCount3,emptyCount3;
int a,b;
int *buffer,BufSize;
pthread_t p1id,c1id[2]; 
pthread_t p2id,c2id[2]; 
pthread_t p3id,c3id[2];       
pthread_attr_t attr;
void *Producer1();
void *Consumer11();
void *Consumer12();
void *Producer2(); 
void *Consumer21();
void *Consumer22();
void *Producer3(); 
void *Consumer31();
void *Consumer32();
int j,k,i;
int *arrival_time,*service_time;
int total_time;

//-----------------------------Main begins here-----------------------------------------------
int main(int argc,char *argv[])
{  
  if(argc==1)
   {
   printf("\n\n-----------------------------------------------------\n");
   printf("Implementation 1: Without Mutex \n");
   printf("|argv[2]= 1 : Single queue and Single Service Personnel\n");
   printf("|argv[2]= 2 : Single queue and Two Service Personnel\n");
   printf("\nImplementation 2: With Mutex (interleaving)\n");
   printf("|argv[2]= 3 : Single queue and Single Service Personnel\n");
   printf("|argv[2]= 4 : Single queue and Two Service Personnel\n");
   printf("\nImplementation 3: With Mutex (No interleaving)\n");
   printf("|argv[2]= 5: Single queue and Single Service Personnel\n");
   printf("|argv[2]= 6 : Single queue and Two Service Personnel\n");
   printf("-----------------------------------------------------\n");
   printf("argv[2]= 7 : Exit\n");
   exit(1);
   }
   
   int i,j,x;
   BufSize=atoi(argv[1]);
   x=atoi(argv[2]);
   buffer=(int *)malloc(BufSize*sizeof(int));
   arrival_time=(int *)malloc(BufSize*sizeof(int));
   service_time=(int *)malloc(BufSize*sizeof(int));
   struct pc q1[BufSize];
   struct pc key[BufSize];
   
   if(argc!=3)
   {
    printf("Incorrect Format..Format Should be <Filename,Maximum_Customers_in_a_queue,Case_number>\n");
    exit(1);
   } 

   srand(time(0));
   for(j=0;j<=BufSize;j++)
   {
    q1[j].arrival_time=rand()%10;
    q1[j].service_time=rand()%100;
   }
      
    q1[0].arrival_time=0;
  
//-------------------------Sorting----------------------------------------------
      for(i=1;i<=BufSize;i++)
   {
       key[i]=q1[i];
      j = i-1;
     while (j >= 0 && q1[j].arrival_time> key[i].arrival_time)
       {
           q1[j+1]=q1[j];
           j = j-1;
       }
       q1[j+1] = key[i];
   }


  
   for(j=0;j<=BufSize;j++)
      {
        arrival_time[j]=q1[j].arrival_time;
        service_time[j]=q1[j].service_time;
       }
  

   pthread_mutex_init(&buffer_mutex, NULL);//non recursive
   sem_init(&fillCount1, 0, 0);//last is the value from where it is initialized
   sem_init(&emptyCount1, 0, BufSize);
   sem_init(&fillCount2, 0, 0);//last is the value from where it is initialized
   sem_init(&emptyCount2, 0, BufSize);
   sem_init(&fillCount3, 0, 0);//last is the value from where it is initialized
   sem_init(&emptyCount3, 0, BufSize);
   pthread_attr_init(&attr);

   if(x==1)//Single queue and Single Service Personnel
   {
    
    pthread_create(&p1id,&attr,Producer1,NULL);
    sleep(4);
    pthread_create(&c1id[0],&attr,Consumer11,NULL);
    pthread_join(p1id,NULL);
    pthread_join(c1id[0],NULL);
  }
    

    else if(x==2)//Single queue and Two Service Personnel
    {
    pthread_create(&p1id,&attr,Producer1,NULL);
    sleep(4);
    pthread_create(&c1id[0],&attr,Consumer11,NULL);
    pthread_create(&c1id[1],&attr,Consumer12,NULL);
    pthread_join(p1id,NULL);
    pthread_join(c1id[0],NULL);
    pthread_join(c1id[1],NULL);
  }
    
    
    else if(x==3)//Single queue and Single Service Personnel(interleaving)
    {
    pthread_create(&p2id,&attr,Producer2,NULL);
    sleep(4);
    pthread_create(&c2id[0],&attr,Consumer21,NULL);
    pthread_join(p2id,NULL);
    pthread_join(c2id[0],NULL);
  }
    

    else if(x==4)// Single queue and Two Service Personnel(interleaving)
    {
    pthread_create(&p2id,&attr,Producer2,NULL);
    sleep(4);
    pthread_create(&c2id[0],&attr,Consumer21,NULL);
    pthread_create(&c2id[1],&attr,Consumer22,NULL);
    pthread_join(p2id,NULL);
    pthread_join(c2id[0],NULL);
    pthread_join(c2id[1],NULL);
    }

    else if(x==5)//Single queue and Single Service Personnel(No interleaving)
    {
    pthread_create(&p3id,&attr,Producer3,NULL);
    sleep(4);
    pthread_create(&c3id[0],&attr,Consumer31,NULL);
    pthread_join(p3id,NULL);
    pthread_join(c3id[0],NULL);
    }

    else if(x==6)//Single queue and Two Service Personnel(No interleaving)
    {
    pthread_create(&p2id,&attr,Producer3,NULL);
    sleep(4);
    pthread_create(&c3id[0],&attr,Consumer31,NULL);
    pthread_create(&c3id[1],&attr,Consumer32,NULL);
    pthread_join(p3id,NULL);
    pthread_join(c3id[0],NULL);
    pthread_join(c3id[1],NULL);
    }
    
    else
    exit(1);
    
  return 0;
}

//---------------------------------------------------------------------------------
void *Producer1() 
{   
    int item,i=0,j,k;
    srand(time(0));
    int customer=1;
    while (1) 
    {  
      if(b<BufSize)
       {
         
       sem_wait(&emptyCount1);
       printf("Customer%d_service_time : ",customer++);
       item=rand()%100;
       
       buffer[i]=item;//Shared Buffer
       total_time+=buffer[i];
       printf("%d ",buffer[i]);
       
       printf("\nTotal_time = %d \n\n",total_time);
       i++;
       b++;
       sem_post(&fillCount1);

     }
     else
     {
     printf("\nMaximum Customer Arrived..thus next Customer leaves\n",item );
     break;
   }
  }
}

void *Consumer11() 
{   
    int item,customer=0;
    while (1) 
    {    
      
      if(b>0)
      {
      sem_wait(&fillCount1);
      item=buffer[k];
       total_time-=item;
    
      k++;
      b--;
      printf("\nCustomer_service_time : %d ,Service Personnel 1",item);
        printf("\nTotal_time_left = %d \n\n",total_time);
      sem_post(&emptyCount1);
    }
       else
       {
          printf("\n Service Personnel 1 sleeps\n\n");
       
        break;
      }
        
    }
}

void *Consumer12() 
{   
    int item,customer=0;
    while (1) 
    {   
       
      if(b>0)
      {
        sem_wait(&fillCount1);
        item=buffer[k];
        total_time-=item;
        k++;
        b--;
        printf("\nCustomer_service_time: %d  Service Personnel 2",item);
        printf("\nTotal_time_left = %d \n\n",total_time);

        sem_post(&emptyCount1);
      }
        
        else
        {
          printf("\n Service personnel 2 sleeps\n");
           
          break;
        }
        
        
    }
  }

//----------------------------Interleaving (With Mutex)-----------------------------------
void *Producer2() 
{   
    int item,j=0,customer=0;
    static int flag2=0,flag=0;
    printf("Customers Arriving In Queue.....\n\n");
    static int c=1;
    
    if(flag2==0)
    {
       printf("Initially Queue Is Empty..Now customers are arriving\n");
       
  
     flag2=1;
     }  
  
  
   srand(time(0));
    while (1) 
    {    
        if(customer==1&&flag==0)
        {
           printf("\nWaking up Service_personnel\n");
          flag=1;
        }
      
      else if(flag==1&&customer==1)
      {
        printf("Maximum Customers Arrived...Exiting\n");
        break;
      }

       
      
    customer++;
    

       if(a<BufSize)
       {
        sem_wait(&emptyCount2);
            pthread_mutex_lock(&buffer_mutex);
              buffer[i]=service_time[i];
              total_time+=buffer[i];
              printf("Customer to be serviced: arrival_time= %d,service_time= %d\n",arrival_time[i],service_time[i] );
              service_time[0]=service_time[BufSize];
              printf("Buffer : %d\n",buffer[i] );
              printf("Total_time: %d\n",total_time);
              a++;
           pthread_mutex_unlock(&buffer_mutex);
        sem_post(&fillCount2);
           


      
        i++;
        printf("\n");
       }
    

      else
      {
        printf("Maximum Customer Arrived..thus Customer leaves\n\n");
        break;
      }
     
        
    }
     
}


void *Consumer21() 
{   
    int item,customer=0;
    printf("\nCONSUMPTION\n");
    while (1) 
    {   
      if(a>0)
      {
        sem_wait(&fillCount2);
            pthread_mutex_lock(&buffer_mutex);
               item=buffer[j];
               total_time-=item;
               buffer[j]=0;
               j++;
               a--;
        printf("Customer With service_time: %d  Service Personnel 1\n",item);
        printf("Total_time: %d\n",total_time);
              pthread_mutex_unlock(&buffer_mutex);
        sem_post(&emptyCount2);
        if(j==1)
         Producer2();
      }
      else
             {
              printf("\nService Personnel Sleeps\n");
              exit(1);
              }
  }

}


void *Consumer22() 
{   
    int item;
    
    while (1) 
    {  
      if(a>0)
      {
        sem_wait(&fillCount2);
            pthread_mutex_lock(&buffer_mutex);
               item=buffer[j];
               buffer[j]=0;
               total_time-=item;
               j++;
               a--;
              printf("\nCustomer with service_time :  %d  Service Personnel 2\n",item);
                printf("Total_time: %d\n",total_time);
            pthread_mutex_unlock(&buffer_mutex);
        sem_post(&emptyCount2);
        if(j==1)
         Producer2();
      }
       else
             {
              printf("\n Service Personnel 2 sleeps\n");
               printf("\n Service Personnel 1 sleeps\n");
              exit(1);
             
             }
    }
}

//----------------No interleaving(With Mutex)------------------------------------------------
void *Producer3() 
{   
    int item,j=0,customer=1;
    static int flag=0;
     int cons;
    if(flag==0)
    {
    printf("Initially Queue is empty..Now customer arriving\n");
    flag=1;
    }

   printf("\n");
    srand(time(0));
    while (1) 
    {    
        printf("Customer%d_id :  ",customer);
        printf("Customer=%d,Arrival= %d,Service= %d\n",customer,arrival_time[i],service_time[i]);
        
        if(customer==1){
          printf("\nWaking Up Service_personnel\n\n");
        }

     




        
       if(a<BufSize)

       {
        sem_wait(&emptyCount3);
            pthread_mutex_lock(&buffer_mutex);
              buffer[i]=service_time[i];
               total_time+=buffer[i];
              printf("Customer to be serviced: arrival_time= %d,service_time= %d\n",arrival_time[i],service_time[i] );
              printf("Total_time: %d\n",total_time);
            
              printf("Buffer : %d\n",buffer[i] );
              a++;
           pthread_mutex_unlock(&buffer_mutex);
        sem_post(&fillCount3);
      
        }
        
      else
      { 
        printf("Maximum Customer Arrived..New customer leaves\n",item );
        break;
      }
      i++;
      
        customer++;
    }
  }



void *Consumer31() 
{   
    int item,customer=0;
    printf("\nCONSUMPTION\n");
    while (1) 
    {   
      if(a>0)
      {
        sem_wait(&fillCount3);
            pthread_mutex_lock(&buffer_mutex);
               item=buffer[j];
                total_time-=item;
               buffer[j]=0;
               j++;
               a--;
        printf("Customer With service_time: %d  Service Personnel 1\n",item);
        printf("Total_time_left: %d\n",total_time);

             pthread_mutex_unlock(&buffer_mutex);
        sem_post(&emptyCount3);
        
      }
      else
             {
              printf("\n Service Personnel Sleeps\n");
            
              exit(1);
              }
            }
          }



void *Consumer32() 
{   
    int item,customer=0;
    while (1) 
    {  
      if(a>0)
      {
        sem_wait(&fillCount3);
            pthread_mutex_lock(&buffer_mutex);
               item=buffer[j];
                total_time-=item;
               buffer[j]=0;
               j++;
               a--;
              printf("\nCustomer with service_time :  %d , Service Personnel 2\n",item);
              printf("Total_time_left: %d\n",total_time);
            
            pthread_mutex_unlock(&buffer_mutex);
        sem_post(&emptyCount3);
      }
       else
             {
              printf("\n Service Personnel 2 Sleeps\n");
              printf("\n Service Personnel 1 Sleeps\n");
              exit(1);
             }
    }
}


//---------------------------End Of Program-----------------------------------------------





