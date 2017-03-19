//
//  client.c
//  
//
//  Created by Can Zhang on 17/3/15.
//
//

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]){
    int zipCode=0;
    int fp1=0;
    int fp2=0;
    int lZip=0;
    int hZip=0;
    int data_processed;
    int count=0;
    
    sscanf(argv[0],"%d",&fp1);
    sscanf(argv[1],"%d",&fp2);
    sscanf(argv[2],"%d",&lZip);
    sscanf(argv[3],"%d",&hZip);
    
  
    
    while(1) {
      
        data_processed=read(fp1,&zipCode,sizeof(int));
        if (data_processed<=0){
          
            break;
        }
		  
        if (zipCode>=lZip&&zipCode<=hZip){
            write(fp2,&zipCode,sizeof(int));
            count++;
         

        }
    }
    exit(count);
}

