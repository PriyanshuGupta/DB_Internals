#include <stdio.h>


loadFromFile(int FileDesc){
	FILE *fp;
	fp = fopen("insertinput.txt", "r");
	int recID,value;
	while(!feof(fp)){
		
	//~ printf("A2 \n");
		fscanf(fp,"%d %d",&recID,&value);
		if(feof(fp))
		{
			break;
		}
		HM_Insert(FileDesc,recID,value);
	}
	fclose(fp);
}

main(){
	int fd;	/* file descriptor for the index */
	int recnum;	/* record number */
	
	//~ printf("Initialising\n");
	PF_Init();
	
	//~ printf("Creating hashing structure\n");
	HM_Create("test_hashing");
	//~ printf("opening hashmap\n");
	fd = PF_OpenFile("test_hashing");
	
	//~ printf("doing insert\n");
	loadFromFile(fd);
	
	/*printf("printing\n");
	HM_Print(fd);
	
	printf("Searching\n");
	if(HM_Search(fd, 555466))
		printf("%d\n", HM_Search(fd, 555466));
	
	printf("deleting\n");
	HM_Delete(fd, 0, 555466);
	
	printf("deleting\n");
	HM_Delete(fd, 1, 555466);
	
	printf("printing\n");
	HM_Print(fd);*/
	
	PF_CloseFile(fd);
	
	HM_Destroy("test_hashing");
	
}
