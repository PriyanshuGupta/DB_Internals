# include <stdio.h>
# include "am.h"
# include "pf.h"

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
	int i;

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
BUF_AM_InitializePage(pageBuf,pageNum1,value,attrLength,maxKeys)
char *pageBuf;/* buffer to new root */
int pageNum1;/* pagenumbers of it's child*/
char *value; /* attr value to be inserted */
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



AM_BulkLoad(inputfile,filedesc)
const char* inputfile;
int filedesc;
{
	FILE *fp;
	fp = fopen(inputfile, "r");
	int recID,value;
	while(!feof(fp)){
		fscanf(fp,"%d %d",&recID,&value);
		if(feof(fp))
		{
			break;
		}
		
		printf("%d %d \n",recID,value);
	}
}
