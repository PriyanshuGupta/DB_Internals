AM_IndexLoad(inputfile,fileDesc)
const char* inputfile;
int fileDesc;
{
	FILE *fp;
	fp = fopen(inputfile, "r");
	int recID,value;
	while(!feof(fp)){
		
	//~ printf("A2 \n");
		fscanf(fp,"%d %d",&recID,&value);
		char * VLUE=(char*)&value;
		if(feof(fp))
		{
			break;
		}
		AM_InsertEntry(fileDesc,INT_TYPE,sizeof(int),VLUE,recID);
	}
	fclose(fp);
	
}
