import eventpy
t=eventpy.elist(0)
t.readFile_tsd("/proj/net_scratch/Raj/Research/Network/Phone/SI/Data/cityEvents/events_city_1.txt")
t.normalizeNodeIndices()
nodes=t.getNumberOfNodes()
ip=eventpy.ip(nodes,t)
ip.reset(0,0)
ip.infectAll()
