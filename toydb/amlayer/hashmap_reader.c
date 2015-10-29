# include "hashmap_header.h"
# include <stdio.h>
# include "pf.h"
# include <stdbool.h>


bcopy(char* s1, char *s2, int nbytes)
{
memcpy(s2,s1,nbytes);
}

hash(int x)
{
	return x; 
}

HM_Create(char * filename)
{
	PF_CreateFile(filename);
	int fileDesc = PF_OpenFile(filename);
	int i=0;
	for(i=0;i<NUMBEROFBUCKETS;i++)
	{
		int pageNum;
		char* pageBuf;
		PF_AllocPage(fileDesc,&pageNum,&pageBuf);
		HM_PAGEHEADER head,*header;
		header = &head;
		header->numKeys=0;
		header->maxKeys=(int) ((PF_PAGE_SIZE-HMHEADERSIZE)/HMRECORDSIZE)-1;
		header->overflowBucket=NOOVERFLOW;
		bcopy(header,pageBuf,HMHEADERSIZE);
		PF_UnfixPage(fileDesc,pageNum,TRUE);
	}
	PF_CloseFile(fileDesc);
	return;
}

HM_Insert(int fileDesc,int recID,int val)
{
	int pageToInsert=hash(val);
	bool inserted=FALSE;
	do
	{
		char* pagebuf;
		PF_GetThisPage(fileDesc,pageToInsert,&pagebuf);
		HM_PAGEHEADER head,*header;
		header = &head;
		bcopy(pagebuf,header,HMHEADERSIZE);
		if(header->overflowBucket!=NOOVERFLOW)
		{
			inserted=FALSE;
			pageToInsert=header->overflowBucket;
		}
		else
		{
			HM_RECORD rec,*record;
			record = &rec;
			record->value=val;
			record->recordID=recID;
			bcopy(record,pagebuf+HMHEADERSIZE+HMRECORDSIZE*(header->numKeys),HMRECORDSIZE);
			inserted=TRUE;
			header->numKeys=header->numKeys+1;
			if(header->numKeys==header->maxKeys)
			{
				int newpage;
				char* newpagebuf;
				PF_AllocPage(fileDesc,&newpage,&newpagebuf);
				HM_PAGEHEADER newhead,*newheader;
				newheader = &newhead;
				newheader->numKeys=0;
				newheader->maxKeys=(int) ((PF_PAGE_SIZE-HMHEADERSIZE)/HMRECORDSIZE)-1;
				newheader->overflowBucket=NOOVERFLOW;
				bcopy(newheader,newpagebuf,HMHEADERSIZE);
				PF_UnfixPage(fileDesc,newpage,TRUE);
				header->overflowBucket=newpage;
			}
				bcopy(header,pagebuf,HMHEADERSIZE);
		}
		PF_UnfixPage(fileDesc,pageToInsert,TRUE);
	}while(!inserted);
	
	return;
	
}

HM_SearchPage(int val, int pageNum, int fileDesc)
{
	int recID = -1;
	char* pagebuf;
	PF_GetThisPage(fileDesc,pageNum,&pagebuf);
	HM_PAGEHEADER head,*header;
	header = &head;
	bcopy(pagebuf,header,HMHEADERSIZE);
	int i;
	for(i=0;i<header->numKeys;i++){
		HM_RECORD rec,*record;
		record = &rec;
		bcopy(pagebuf+HMHEADERSIZE+HMRECORDSIZE*i,record,HMRECORDSIZE);
		if(record->value = val)
			recID = record->recordID;
	}
	
	PF_UnfixPage(fileDesc,pageNum,FALSE);
	
	return recID;
}

HM_Search(int fileDesc,int val)
{
	int check = -1;
	int pageToFind=hash(val);
	bool found=FALSE;
	do
	{
		char* pagebuf;
		PF_GetThisPage(fileDesc,pageToFind,&pagebuf);
		HM_PAGEHEADER head,*header;
		header = &head;
		bcopy(pagebuf,header,HMHEADERSIZE);
		int search = HM_SearchPage(val, pageToFind, fileDesc);
		
		if(search==-1)
		{
			found=FALSE;
			if(header->overflowBucket!=NOOVERFLOW)
				pageToFind=header->overflowBucket;
			else
				check = 0;
				
		}
		else
		{
			found = TRUE;
			//~ HM_RECORD rec,*record;
			//~ record = &rec;
			//~ record->value=val;
			//~ record->recordID=recID;
			//~ bcopy(record,pagebuf+HMHEADERSIZE+HMRECORDSIZE*(header->numKeys),HMRECORDSIZE);
			//~ inserted=TRUE;
			//~ header->numKeys=header->numKeys+1;
			//~ if(header->numKeys==header->maxKeys)
			//~ {
				//~ int newpage;
				//~ char* newpagebuf;
				//~ PF_AllocPage(fileDesc,&newpage,&newpagebuf);
				//~ HM_PAGEHEADER newhead,*newheader;
				//~ newheader = &newhead;
				//~ newheader->numKeys=0;
				//~ newheader->maxKeys=(int) ((PF_PAGE_SIZE-HMHEADERSIZE)/HMRECORDSIZE)-1;
				//~ newheader->overflowBucket=NOOVERFLOW;
				//~ bcopy(newheader,newpagebuf,HMHEADERSIZE);
				//~ PF_UnfixPage(fileDesc,newpage,TRUE);
				//~ header->overflowBucket=newpage;
			//~ }
				//bcopy(header,pagebuf,HMHEADERSIZE);
		}
		PF_UnfixPage(fileDesc,pageToInsert,TRUE);
	}while(!found && check==-1);
	
	return search;
	
}
