BUF_AM_Create(filedesc, attrLength, maxKeys)
int filedesc;
short attrLength,maxKeys;
{
	File * fp;
	fp = fopen("inputfile", "r");
	bool newNode=true;
	int pageNum;
	char *pageBuf;
	int recID, value;
	FILE *fp1;
	fp1 = fopen("outputfile", "w");// "w" means that we are going to write on this file
	int count=0;
	while(!feof(fp)){
		fscanf(fp,"%d %d",recID,value);
		AM_INTHEADER head,*header;
		header=&head;
		bcopy(pageBuf,header,AM_sint);
		bool temp = BUF_AM_AddtoIntPage(pageBuf, (char *)value, pageNum, header);
		newNode=!temp;
		if(newNode){
			count++;
			fprintf(fp1, "%d %d", recID, value);//print header ones to file output.txt
			int errVal;
			errVal=PF_AllocPage(filedesc, &pageNum, &pageBuf);
			BUF_AM_InitializePage(pageBuf, pageNum, attrLength, maxKeys);
			newNode=false;
		}
	}
	if(count==1)RETURN(TRUE)
	else RETURN(FALSE)
}

BUF_AM_Recursion(filedesc, attrLength, maxKeys)
int filedesc;
short attrLength, maxKeys;
{
	bool check=true;
	while(check){
		check=BUF_AM_Create(filedesc, attrLength, maxKeys);
		remove("inputfile");
		rename("outputfile", "inputfile");
	}
}