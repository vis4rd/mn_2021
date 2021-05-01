import numpy as np		# importowanie przydatnych modulow
import matplotlib.pyplot as plt

files = ["f2out5.txt", "f2out8.txt", "f2out21.txt"]	# wczytanie pliku z danymi
ans = ["f2-analytic5.txt", "f2-analytic8.txt", "f2-analytic21.txt"]

for (file, file2) in zip(files, ans):
    fig = plt.figure()	# stworzenie domyslnego obiektu figure
    data = np.loadtxt(file)	# do tablicy data
    data2 = np.loadtxt(file2)

    x = data[:, 0]
    knots = data[:, 3]
    x1 = data2[:, 0]
    f1x1 = data2[:, 1]
    sx = data2[:, 2]

    plt.plot(x1, f1x1, 'k-', lw=1, label='f(x)=cos(2x)')
    plt.plot(x1, sx, 'C0', lw=1, label='sx, n='+str(knots.size))
    plt.plot(x, knots, 'C1.', lw=1, label='wezly')

    l1 = plt.legend()	# legenda
    plt.grid()	# siatka pomocnicza

    plt.xlabel('x')	# tytuly osi i wykresu
    plt.ylabel('f(x)')
    plt.title('Interpolacja Funkcjami Sklejanymi')

    fout = "f2pic" + str(knots.size) + ".png"
    plt.savefig(fout)	# zapis wykresu do pliku

fig = plt.figure()
data3 = np.loadtxt("preout.txt")
data4 = np.loadtxt("anf1.txt")

osx = data3[:, 0]
m = data3[:, 1]
df = data3[:, 2]
a1 = data4[:, 0]
a2 = data4[:, 1]

plt.plot(osx, m, 'C0.', lw=1, label='m_i')
plt.plot(osx, df, 'C1.', lw=1, label='f1\'\'(x) num.')
plt.plot(a1, a2, 'k-', lw=1, label='f1\'\'(x) an.')

l2 = plt.legend()
plt.grid()
plt.xlabel('x')
plt.ylabel('f1\'\'(x)')

fout2 = "prepic.png"
plt.savefig(fout2)

