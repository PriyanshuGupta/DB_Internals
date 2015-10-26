# include <stdio.h>
# include "am.h"
# include "pf.h"
#include <stdbool.h>
/* adds a key to an internal node */
BUF_AM_AddtoIntPage(pageBuf,value,pageNum,header)
char *pageBuf;
char *value; /* value to be added to the node */
int pageNum; /* page number of child to be inserted */
AM_INTHEADER *header;
{
	if(header->numKeys==header->maxKeys)
	{
		return(FALSE);
	}
	int recSize;

	recSize = header->attrLength + AM_si;

	/* copy the attribute value into the appropriate place */
	bcopy(value,pageBuf + AM_sint + header->numKeys*recSize + AM_si,header->attrLength);

	/* copy the pagenumber of the child */
	bcopy((char *)&pageNum,pageBuf + AM_sint + (header->numKeys+1)*recSize,AM_si);

	/* one more key added*/
	header->numKeys++;
	return(TRUE);
}


/* Fills the header and inserts a key into a new root */
BUF_AM_InitializeLeafPage(pageBuf,attrLength,maxKeys)
char *pageBuf;/* buffer to new root */
short attrLength,maxKeys; /* some info about the header */

{
	AM_LEAFHEADER head,*header;
	header = &head;
	header->pageType = 'l';
	header->nextLeafPage = AM_NULL_PAGE;
	header->recIdPtr = PF_PAGE_SIZE;
	header->keyPtr = AM_sl;
	header->freeListPtr = AM_NULL;
	header->numinfreeList = 0;
	header->attrLength = attrLength;
	header->numKeys = 0;
	header->maxKeys = maxKeys;
	bcopy(header,pageBuf,AM_sl);

}

	
/* Fills the header and inserts a key into a new root */
BUF_AM_InitializePage(pageBuf,pageNum1,attrLength,maxKeys)
char *pageBuf;/* buffer to new root */
int pageNum1;/* pagenumbers of it's child*/
short attrLength,maxKeys; /* some info about the header */

{
	AM_INTHEADER temphead,*tempheader;
	tempheader = &temphead;
	/* fill the header */
	tempheader->pageType = 'i';
	tempheader->attrLength = attrLength;
	tempheader->maxKeys = maxKeys;
	tempheader->numKeys = 1;
	bcopy((char *)&pageNum1,pageBuf + AM_sint ,AM_si);
	bcopy(tempheader,pageBuf,AM_sint);

}

BUF_MAXKEYS_attrLength(fileDesc,maxkeys,attrlength)
short* maxkeys;
short* attrlength;
int fileDesc;
{
	char* pageBuf;
	int pageNum;
	AM_LEAFHEADER lhead,*lheader; /* local pointer to leaf header */
	AM_INTHEADER ihead,*iheader; /* local pointer to internal node header */
  /* initialise the headers */	
	lheader = &lhead;
	iheader = &ihead;
        /* get the root of the B+ tree */
	int errVal = PF_GetFirstPage(fileDesc,&pageNum,&pageBuf);
	AM_Check;
	if (*pageBuf == 'l' ) 
		/* if root is a leaf page */
	{
		bcopy(pageBuf,lheader,AM_sl);
		*maxkeys=lheader->maxKeys;
		*attrlength=lheader->attrLength;
	}
	else /* root is not a leaf */
	{
		bcopy(pageBuf,iheader,AM_sint);
		*maxkeys=iheader->maxKeys;
		*attrlength=iheader->attrLength;
	}
	errVal = PF_UnfixPage(fileDesc,pageNum,FALSE);
	AM_Check;
}

BUF_AM_Create(filedesc, attrLength, maxKeys)
int filedesc;
short attrLength,maxKeys;
{
	FILE * fp;
	fp = fopen("inputfile", "r");
	bool newNode=TRUE;
	int pageNum;
	char *pageBuf;
	int pageID, value;
	int errVal;
	FILE *fp1;
	fp1 = fopen("outputfile", "w");// "w" means that we are going to write on this file
	int count=0;
	while(!feof(fp)){
		fscanf(fp,"%d %d",&pageID,&value);
		if(feof(fp))
		{
			break;
		}
		AM_INTHEADER head,*header;
		header=&head;
		bcopy(pageBuf,header,AM_sint);
		if(newNode==TRUE)
		{
			count++;
			fprintf(fp1, "%d %d", pageID, value);//print header ones to file output.txt
			errVal=PF_AllocPage(filedesc, &pageNum, &pageBuf);
			BUF_AM_InitializePage(pageBuf, pageID, attrLength, maxKeys);
			newNode=FALSE;
		}
		bool temp = BUF_AM_AddtoIntPage(pageBuf, (char *)value, pageID, header);
		if(temp==FALSE){
			count++;
			fprintf(fp1, "%d %d", pageID, value);//print header ones to file output.txt
			errVal=PF_AllocPage(filedesc, &pageNum, &pageBuf);
			BUF_AM_InitializePage(pageBuf, pageID, attrLength, maxKeys);
			newNode=false;
		}
	}
	if(count==1) return(TRUE);
	else return(FALSE);
}

BUF_AM_Recursion(filedesc, attrLength, maxKeys)
int filedesc;
short attrLength, maxKeys;
{
	bool check=TRUE;
	while(check){
		check=BUF_AM_Create(filedesc, attrLength, maxKeys);
		remove("inputfile");
		rename("outputfile", "inputfile");
	}
}

AM_BulkLoad(inputfile,fileDesc)
const char* inputfile;
int fileDesc;
{
	printf("HHH");
	short maxKeys,attrLength;
	BUF_MAXKEYS_attrLength(fileDesc,&maxKeys,&attrLength);
	
	FILE *fp;
	fp = fopen(inputfile, "r");
	
	FILE *fp1;
	fp1 = fopen("inputfile", "w");
	
	
	int recID,value,errVal;
	int prevValue=-1;
	char* pageBuf;
	int pageNum;
	bool leaffinished=FALSE;
	bool valueInserted=FALSE;
	//Initialize a leaf
	
	errVal=PF_AllocPage(fileDesc, &pageNum, &pageBuf);
	AM_Check;
	printf("HH %d \n",pageNum);
	//~ errVal=PF_GetThisPage(fileDesc, pageNum, &pageBuf);
	//~ AM_Check;
	printf("A2 \n");
	BUF_AM_InitializeLeafPage(pageBuf,attrLength, maxKeys);
	AM_Check;
	while(!feof(fp)){
		
	//~ printf("A2 \n");
		fscanf(fp,"%d %d",&recID,&value);
		char * VLUE=(char*)&value;
		if(feof(fp))
		{
			break;
		}
		if(valueInserted==FALSE)
		{
			//Print pageNumber and Value into inputfile
			fprintf(fp1,"%d %d \n",pageNum,value);
			valueInserted=TRUE;
		}
		
		AM_LEAFHEADER head,*header;
		/* initialise the header */
		header = &head;
		
		bcopy(pageBuf,header,AM_sl);
		
		//~ //Get the value of numKeys
		int numKeys=header->numKeys;
		printf("%d \n",numKeys);
		if(value==prevValue)
		{
			leaffinished=AM_InsertintoLeaf(pageBuf,attrLength,VLUE,recID,numKeys,AM_FOUND);
		}
		else
		{
			leaffinished=AM_InsertintoLeaf(pageBuf,attrLength,VLUE,recID,numKeys+1,AM_NOT_FOUND);
		}
		if(leaffinished==FALSE)
		{
			//Initialize new leaf
			errVal = PF_UnfixPage(fileDesc,pageNum,FALSE);
			AM_Check;
			
			errVal=PF_AllocPage(fileDesc, &pageNum, &pageBuf);
			AM_Check;
			//~ errVal=PF_GetThisPage(fileDesc, pageNum, &pageBuf);
			//~ AM_Check;
			BUF_AM_InitializeLeafPage(pageBuf,attrLength, maxKeys);
			header->nextLeafPage=pageNum; //Set pointer from prev to new
			leaffinished=AM_InsertintoLeaf(pageBuf,attrLength,VLUE,recID,1,AM_NOT_FOUND);
			prevValue=-1;
			fprintf(fp1,"%d %d \n",pageNum,value); //Print pageNumber and Value into inputfile
		}	
		//~ printf("%d %d \n",recID,value);
	}
	BUF_AM_Recursion(fileDesc,attrLength,maxKeys);
	fclose(fp);
}
