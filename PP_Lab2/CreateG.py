import matplotlib.pyplot as plt

def parse_results(filename):
    sizes = []
    times = []
    with open(filename, 'r') as file:
        for line in file:
            size = int(line.split('size')[1].split('x')[0])

            time = int(line.split('-')[1].strip().split('ms')[0].strip())
            sizes.append(size)
            times.append(time)
    return sizes, times

def plot_results(sizes1, times1, sizes2, times2):
    plt.figure(figsize=(10, 5))
    plt.plot(sizes1, times1, marker='o', color='blue', label='Данные из первого эксперимента')
    plt.plot(sizes2, times2, marker='o', color='red', label='Данные из второго эксперимента(OpenMP)')
    plt.title('Зависимость времени вычисления от размера матрицы')
    plt.xlabel('Размер матрицы (n x n)')
    plt.ylabel('Время (мс)')
    plt.legend()
    plt.grid(True)
    plt.show()


filename1 = 'dataOpenMP/ResultExperiment.txt'
filename2 = 'dataOpenMP/ResultExperimentOpenMP_5.txt'

sizes1, times1 = parse_results(filename1)
sizes2, times2 = parse_results(filename2)

plot_results(sizes1, times1, sizes2, times2)