typedef struct hm_pageheader
	{
		int overflowBucket;
		short numKeys;
		short maxKeys;
	}  HM_PAGEHEADER; /* Header for a bucket */

typedef struct hm_record
	{
		int value;
		int recordID;
	}  HM_RECORD; /* Header for a leaf page */

# define RECORDVALID 1
# define RECORDINVALID -1

# define NOOVERFLOW -1
# define HMHEADERSIZE sizeof(HM_PAGEHEADER)
#define HMRECORDSIZE sizeof(HM_RECORD)
#define NUMBEROFBUCKETS 100000 
