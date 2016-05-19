from python_aux import binner
import pylab as p
#bc=p.loadtxt("nodebc_1.txt")
#sbc=p.loadtxt("static_nodebc_1.txt")
#k=p.loadtxt("degs_1.txt")

bc=p.loadtxt("/proj/net_scratch/Mikko/phone_dynamics/nodebc_92.txt")
sbc=p.loadtxt("/proj/net_scratch/Mikko/phone_dynamics/static_nodebc_92.txt")
k=p.loadtxt("/proj/net_scratch/Mikko/phone_dynamics/degs_92.txt")

bc=bc/p.mean(bc)
sbc=sbc/p.mean(sbc)

p.figure(1)
p.loglog(bc,sbc,'.')
p.xlabel(r"$C_B(v)/\langle C_B(v) \rangle$")
p.ylabel(r"$\hat{C}_B(v)/\langle \hat{C}_B(v) \rangle$")

kbins=binner.Bins(int,1,max(k),"linlog",1.5)

r=bc-sbc


rvsk=kbins.bin_average(zip(k,r))

sbcvsk=kbins.bin_average(zip(k,sbc))
bcvsk=kbins.bin_average(zip(k,bc))


p.figure(2)
p.plot(kbins.centers,rvsk)
p.xlabel(r"Degree $k$")
p.ylabel(r"$C_B(v)/\langle C_B(v) \rangle-\hat{C}_B(v)/\langle \hat{C}_B(v) \rangle$")

p.figure(3)
p.plot(kbins.centers,bcvsk/sbcvsk)
p.xlabel(r"Degree $k$")
p.ylabel(r"$C_B(v)/\langle C_B(v) \rangle/ (\hat{C}_B(v)/\langle \hat{C}_B(v) \rangle)$")

p.figure(4)
rr=bc/sbc
p.semilogy(k,rr,'.')
p.xlabel(r"Degree $k$")
p.ylabel(r"$C_B(v)/\langle C_B(v) \rangle/ (\hat{C}_B(v)/\langle \hat{C}_B(v) \rangle)$")


p.figure(5)
p.semilogy(kbins.centers,bcvsk,kbins.centers,sbcvsk)
p.xlabel(r"Degree $k$")
p.legend([r"$C_B(v)/\langle C_B(v) \rangle$",r"$\hat{C}_B(v)/\langle \hat{C}_B(v) \rangle$"],loc="lower right")

binned_bc=kbins.bin_average(zip(k,bc))
p.figure(6)
p.plot(kbins.centers,binned_bc)
p.xlabel(r"Degree $k$")
p.ylabel(r"$C_B(v)/\langle C_B(v) \rangle$")
