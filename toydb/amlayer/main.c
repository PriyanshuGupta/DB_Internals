/* test3.c: tests deletion and scan. */
#include <stdio.h>
#include "testam.h"
#include "amBufferLoad.c"
#include "amIndexLoad.c"

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
	//~ AM_IndexLoad("file",fd);
	//~ AM_PrintTree(fd,0,'i'); 
	//~ int x,y;
	//~ scanf("%d",&x);
	//~ AM_PrintTree(fd,0,'i');
	//~ printf("X \n");
	//~ scanf("%d",&y); 
	numrec= 0;
	sd = AM_OpenIndexScan(fd,INT_TYPE,sizeof(int),EQ_OP,NULL);
	while(numrec<1576 && (recnum=AM_FindNextEntry(sd))>= 0){
		//~ printf("%d\n",recnum);
		numrec++;
	}
	printf("retrieved %d records\n",numrec);
	AM_CloseIndexScan(sd);


	numrec= 0;
	sd = AM_OpenIndexScan(fd,INT_TYPE,sizeof(int),EQ_OP,NULL);
	while((recnum=AM_FindNextEntry(sd))>= 0){
		//~ printf("%d\n",recnum);
		numrec++;
	}
	printf("retrieved %d records\n",numrec);
	AM_CloseIndexScan(sd);
//~ 
	/* print out what remains */
   //~ printf("printing between 100 and 150\n");
	numrec= 0;
	sd = AM_OpenIndexScan(fd,INT_TYPE,sizeof(int),EQ_OP,NULL);
	while((recnum=AM_FindNextEntry(sd))>= 0){
		//~ printf("%d\n",recnum);
		numrec++;
	}
	printf("retrieved %d records\n",numrec);
	AM_CloseIndexScan(sd);
	
	/* destroy everything */
	printf("closing down\n");
	PF_CloseFile(fd);
	AM_DestroyIndex(RELNAME,0);

	printf("test3 done!\n");
}
