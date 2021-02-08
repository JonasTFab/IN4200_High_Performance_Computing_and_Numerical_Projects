import numpy as np

n = 100
T = 1440
tt = np.linspace(0,T,n)
deg = np.zeros(tt.shape)


file = open("daily_temperature.txt", "w")
for i,t in enumerate(tt):
    deg[i] = 10*np.exp(-((t-T/2)/(T/4))**2) + 0.5*np.random.normal(0,1)

    hour = int(t // 60)
    min = int(t - hour*60)

    file.write("%.2i:%.2i   %.1f\n" % (hour,min, deg[i]))

file.close()
