import eventpy
t=eventpy.elist(306218217)
t.readFile_tsd("/proj/net_scratch/Raj/Research/Network/Phone/SI/eventList_LCC_mutual.call.txt")
ip=eventpy.ip(4572735,t)
ip.reset(0,0)
#%time ip.infectAll()
