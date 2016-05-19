import math
import pylab
from numpy import *
from operator import *
import os

results = [(0,1,1),(0,3,3),(1,2,1),(1,3,2),(1,4,3),(2, 4, 2),(2,5,3),(4,5,1),(5,6,1)]

os.system("g++ dagMaker2.cpp -o dagMaker2.exe")
os.system("./dagMaker2.exe")

fi=open("DAG.edg","r")
li=[]
for line in fi:
	data = line.rstrip('\n\r').split(" ")
	li.append((int(data[0]),int(data[1]),int(data[2])))
	
li=sorted(li, key=itemgetter(0,2,1))
results=sorted(results, key=itemgetter(0,2,1))

for i in li:
	print i
print '\n'
for i in results:
	print i		
	
if li==results:
	print '\n OK'
else: 
	print '\n not OK'
		
fi.close() 
