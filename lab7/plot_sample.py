import numpy as np		# importowanie przydatnych modulow
import matplotlib.pyplot as plt

for i in range(5, 21, 5):
    fig = plt.figure()	# stworzenie domyslnego obiektu figure
    file = "out"+str(i)+".txt"	# wczytanie pliku z danymi
    data = np.loadtxt(file)	# do tablicy data

    t = data[:, 0]
    x = data[:, 1]

    plt.plot(t, x, 'C0', lw=2, label='x') # rysowanie wykresu
    plt.plot(t, 1/(1+t**2), 'k-', lw=1, label='1/(1+x*x)')

    l1 = plt.legend()	# legenda
    plt.grid()	# siatka pomocnicza

    plt.xlabel('x')	# tytuly osi i wykresu
    plt.ylabel('f(x)')
    plt.title('Interpolacja Newtona')

    fout = "chebpic" + str(i) + ".png"
    plt.savefig(fout)	# zapis wykresu do pliku

