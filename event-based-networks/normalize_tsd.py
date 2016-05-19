from netpython import netext
import sys


try:
    if len(sys.argv)!=3:
        raise SyntaxError("Usage: "+sys.argv[0]+" input.txt output.txt")

    inputfile=open(sys.argv[1],'rU')
    outputfile=open(sys.argv[2],'w')

    i=netext.Enumerator()
    for line in inputfile:
        fields=line.strip().split()
        outputfile.write(fields[0]+" "+str(i[fields[1]])+" "+str(i[fields[2]])+"\n")

except Exception,e:
    print e
