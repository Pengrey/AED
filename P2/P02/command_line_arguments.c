//
// Tomás Oliveira e Silva, AED, September 2020
//
// list all command line arguments
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc,char *argv[argc])
{	
    for(int i = 0;i < argc;i++){
        int val = atoi(argv[i]);	  
	printf("argv[%2d] = \"%i\"\n",i, val);
    }
    return 0;
}
