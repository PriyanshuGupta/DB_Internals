/* test3.c: tests deletion and scan. */
#include <stdio.h>
#include "testam.h"
#include "amBufferLoad.c"

#define MAXRECS	10000	/* max # of records to insert */
#define FNAME_LENGTH 80	/* file name size */

main()
{
int fd;	/* file descriptor for the index */
char fname[FNAME_LENGTH];	/* file name */
int recnum;	/* record number */
int sd;	/* scan descriptor */
int numrec;	/* # of records retrieved */
int testval;	

	/* init */
	printf("initializing\n");
	PF_Init();

	/* create index */
	printf("creating index\n");
	AM_CreateIndex(RELNAME,0,INT_TYPE,sizeof(int));

	/* open the index */
	printf("opening index\n");
	sprintf(fname,"%s.0",RELNAME);
	fd = PF_OpenFile(fname);

	AM_BulkLoad("file",fd);

	/* destroy everything */
	printf("closing down\n");
	PF_CloseFile(fd);
		AM_DestroyIndex(RELNAME,0);

	printf("test3 done!\n");
}
