//
//  server.c
//  
//
//  Created by Can Zhang on 17/3/15.
//
//

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <sys/wait.h>

int main(){
    int number=0;
    int lowZip=0;
    int highZip=0;
    int data_processed;
    int file_pipe1[2];
    int file_pipe2[2];
    int file_pipe3[2];
    int file_pipe4[2];
    int zip=0;
    int clientid1;
    int clientid2;
    char pipe_read[BUFSIZ+1];
    char pipe_write[BUFSIZ+1];
    char lowZ[6];
    char highZ[6];
    
    pid_t fork_result;
	pid_t pid1, pid2;
    FILE *fp;
    fp = fopen("123.txt","r");
    
    if(fp == NULL){
        printf("file open failure\n");
        return 0;
    }
    
    fscanf(fp,"%d",&number);
    printf("%d",number);
    if(pipe(file_pipe1)==0&&pipe(file_pipe2)==0&&pipe(file_pipe3)==0&&pipe(file_pipe4)==0){
        for(int i=0;i<number/2;i++){
            fscanf(fp,"%d",&zip);
            printf(" %d ",zip);
            write(file_pipe1[1],&zip,sizeof(int));
        }
        close(file_pipe1[1]);
        
        for (int i=number/2;i<number;i++){
            fscanf(fp,"%d",&zip);
            printf(" %d ",zip);
            write(file_pipe2[1],&zip,sizeof(int));
        }
        close(file_pipe2[1]);
        
        printf("client1 was sent the first %d zipcodes\n",number/2);
        printf("client2 was sent the remaining %d zipcodes\n",number-number/2);
        printf("Low zip? ");
        scanf("%d",&lowZip);
        printf("High zip? ");
        scanf("%d",&highZip);
        
        pid1 = fork();
        if (pid1<0){
            fprintf(stderr,"fork error!\n");
            exit(EXIT_FAILURE);
        }
        
   
        sprintf(lowZ,"%d",lowZip);
        sprintf(highZ,"%d",highZip);
 
        
        if (pid1 ==0){
            clientid1=getpid();
            printf("this is the value of childid1 %d \n",clientid1);
            printf("start to execl pipe1=client1 \n");
            sprintf(pipe_read,"%d",file_pipe1[0]);
            sprintf(pipe_write,"%d",file_pipe3[1]);
            (void)execl("client",pipe_read,pipe_write,lowZ,highZ,(char*)0);
            exit(EXIT_FAILURE);
        }
        else {
            pid2=fork();
            if (pid2<0){
                fprintf(stderr,"fork error!!!\n");
                exit(EXIT_FAILURE);
            }
            if (pid2 ==0){
                clientid2=getpid();
           
                sprintf(pipe_read,"%d",file_pipe2[0]);
                sprintf(pipe_write,"%d",file_pipe4[1]);
                (void)execl("client",pipe_read,pipe_write,lowZ,highZ,(char*)0);
                exit(EXIT_FAILURE);
            }
            else  {
                int pr;
                int count=0;
                int status=0;
                int finalzip=0;
				int index = 0;
                for (int i=0;i<=1;i++){
                    //printf("ready to wait \n");
                    pr=wait(&status); // wait an chile process to finish
             
                    if (WIFEXITED(status)==0){
                        printf("child process exit unnormally \n");
                    }
                    else if (pr==pid1){
                        printf("notice! \n");
                        close(file_pipe3[1]);
                        count=WEXITSTATUS(status);
                        printf("\nclient1 found %d\n",count);
						index = 0;
                        while (1){
                            //printf("server while in \n");
                            data_processed=read(file_pipe3[0],&finalzip,sizeof(int));
                            if (data_processed<=0){ 
                                break;
                            }
							index++;
                            printf("%d ",finalzip);
                            if (index%8==0)
                                printf(" \n");
                            
                        }
                    } 
                    else if (pr==pid2){
                     
                        close(file_pipe4[1]);
                        count=WEXITSTATUS(status);
                        printf("\nclient2 found %d\n",count);
						index = 0;
                        while (1){
                            data_processed=read(file_pipe4[0],&finalzip,sizeof(int));
                            if (data_processed<=0)
                                break;
							index++;
                            printf("%d ",finalzip);
                            if (index %8==0)
                                printf(" \n");
                        }
                    }
                }
            }
        }
    }
                exit(EXIT_SUCCESS);
}
