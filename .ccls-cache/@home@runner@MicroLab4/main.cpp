#include <iostream>
#include <vector>
#include <omp.h>

int main() {
    int num_threads;
    std::cout << "Ingrese el número de hilos (8, 16 o 32): ";
    std::cin >> num_threads;

    if (num_threads != 8 && num_threads != 16 && num_threads != 32) {
        std::cerr << "Error. Número inválido de hilos." << std::endl;
        return 1;
    }
  
    // Se crea el array x con el tamaño de hilos seleccionado
    std::vector<int> x(num_threads);

    // Se llena el array x
    for (int i = 0; i < num_threads; ++i) {
        x[i] = 2 * (i + 1);
    }

    // Se crea el array y
    std::vector<double> y(num_threads);

    // Se calculan los resultados para el array y en paralelo
    #pragma omp parallel num_threads(num_threads)
    {
        #pragma omp for
        for (int i = 0; i < num_threads; ++i) {
            double a = 5.0;
            y[i] = (0.5 * x[i] * x[i]) - a;
        }
    }

    // Se imprime tabla de resultados
    std::cout << "x\t y" << std::endl;
    for (int i = 0; i < num_threads; ++i) {
        std::cout << x[i] << "\t " << y[i] << std::endl;
    }

    return 0;
}
