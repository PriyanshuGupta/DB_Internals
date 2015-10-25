# include <stdio.h>
# include "am.h"
# include "pf.h"

AM_BulkLoad(inputfile,indexfile)
const char* inputfile;
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
