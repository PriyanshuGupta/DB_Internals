import random, sys

i = int(sys.argv[1])

for j in range(1, i+1):
	print str(j)+" "+str(random.randint(1, 10000001))
