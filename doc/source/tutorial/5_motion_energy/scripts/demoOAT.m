initOAT

NM = NetworkMonitor('../results/sim_me.dat')

NM.setGroupPlotType(-1, 'heatmap')
NM.plot(-1, 500)

NM.setGroupPlotType(-1, 'flowfield')
NM.plot(-1, 500)