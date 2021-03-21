import numpy as np		# importowanie przydatnych modulow
import matplotlib.pyplot as plt

fig = plt.figure()	# stworzenie domyslnego obiektu figure

file = "out2.txt"	# wczytanie pliku z danymi
data = np.loadtxt(file)	# do tablicy data

t = data[:, 0]	
x = data[:, 1]

plt.plot(t, x, 'k-', lw=2, label='beta=0.4, f0=0.0') # rysowanie wykresu

l1 = plt.legend()	# legenda
plt.grid()	# siatka pomocnicza

plt.xlabel('t')	# tytuly osi i wykresu
plt.ylabel('x(t)')
plt.title('wychylenie x(t)')

plt.savefig("obr2.png")	# zapis wykresu do pliku

