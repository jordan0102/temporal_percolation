import sys
from netpython import *

if len(sys.argv)!=3:
    raise SyntaxError("Usage: "+sys.argv[0]+" input.txt output.txt")

#first we aggregate the network
net=pynet.SymmNet()
inputfile=open(sys.argv[1],'r')
for line in inputfile:
    time,source,dest=map(int,line.split())
    net[source,dest]=net[source,dest]+1


#then calculate the degs
outputfile=open(sys.argv[2],'w')
for node in range(len(net)):
    assert node in net
    outputfile.write(str(net[node].deg())+"\n")
