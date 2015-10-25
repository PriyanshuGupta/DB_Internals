a.out : am.o amfns.o amsearch.o aminsert.o amstack.o amglobals.o ../pflayer/pflayer.o main.o amscan.o amprint.o
	cc am.o amfns.o amsearch.o aminsert.o  amstack.o amglobals.o ../pflayer/pflayer.o main.o amscan.o amprint.o 

amlayer.o : am.o amfns.o amsearch.o aminsert.o amstack.o amglobals.o amscan.o amprint.o
	ld -r am.o amfns.o amsearch.o aminsert.o  amstack.o amglobals.o amscan.o amprint.o  -o amlayer.o

am.o : am.cpp am.h pf.h
	cc -c am.cpp

amfns.o : amfns.cpp am.h pf.h
	cc -c amfns.cpp

amsearch.o : amsearch.cpp am.h pf.h
	cc -c amsearch.cpp

aminsert.o : aminsert.cpp am.h pf.h
	cc -c aminsert.cpp

amscan.o : amscan.cpp am.h pf.h
	cc -c amscan.cpp

amstack.o : amstack.cpp am.h pf.h
	cc -c amstack.cpp

amglobals.o : amglobals.cpp am.h
	cc -c amglobals.cpp

amprint.o : amprint.cpp am.h pf.h 
	cc -c amprint.cpp
	
main.o : main.cpp am.h pf.h 
	cc -c main.cpp


