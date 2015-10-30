import os, random
from time import time

t = open("loadTimevsNumEntries.csv", 'w')
print >> t, "Time taken, Number of entries, Number of buckets"
for i in range(1, 2):
	for k in range(0,5):
		f = open("insertinput.txt", "w")
		for j in range(1, 2):
			print >> f, str(j)+" "+str(random.randint(1, 1000000000))
		f.close()
		os.system("sed -ir 's/NUMBEROFBUCKETS .*/NUMBEROFBUCKETS "+ str(10*(10**k))+" /gI' hashmap_reader.h")
		os.system("make 2>> log.txt >> log.txt")
		t1 = time();
		os.system("./a.out")
		t2=time()
		t3 = t2 - t1
		print i,k
		print >> t, str(t3)+", "+str((2**i)*(10000))+", "+str(10*(10**k))
		t.flush()
	
