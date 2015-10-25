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

AM_BulkLoad(inputfile,fileDesc)
const char* inputfile;
int fileDesc;
{
	short maxKeys,attrLength;
	BUF_MAXKEYS_attrLength(fileDesc,&maxKeys,&attrLength);
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
	fclose(fp);
}
