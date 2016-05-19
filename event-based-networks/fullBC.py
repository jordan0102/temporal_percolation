import eventpy,sys


try:
    if len(sys.argv)!=4:
        raise SyntaxError("Usage: "+sys.argv[0]+" input.txt outputNodeBc.txt outputEventBc")

    elist=eventpy.elist(0)
    elist.readFile_tsd(sys.argv[1])
    nodes=elist.getNumberOfNodes()
    events=elist.size

    bcNodes=eventpy.longArray(nodes)
    for i in range(nodes):
        bcNodes[i]=0

    bcEvents=eventpy.longArray(events)
    for i in range(events):
        bcEvents[i]=0

    for i in range(events):
        event=elist[i]
        eventpy.getBC(elist,nodes,event.getSource(),i,bcEvents,bcNodes)
        eventpy.getBC(elist,nodes,event.getDest(),i,bcEvents,bcNodes)

    npaths=float(2*events/float(nodes))

    nodeFile=open(sys.argv[2],'w')
    for i in range(nodes):
        nodeFile.write(str(float(bcNodes[i])/npaths)+"\n")

    eventFile=open(sys.argv[3],'w')
    for i in range(events):
        eventFile.write(str(float(bcEvents[i])/npaths)+"\n")

except Exception,e:
    print e

