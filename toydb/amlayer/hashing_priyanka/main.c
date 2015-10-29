#include <stdio.h>

main(){
	int fd;	/* file descriptor for the index */
	int recnum;	/* record number */
	
	printf("Initialising\n");
	PF_Init();
	
	printf("Creating hashing structure\n");
	HM_Create("test_hashing");
	//~ HM_Destroy("test_hashing");
	
}
