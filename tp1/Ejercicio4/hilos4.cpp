#include <iostream>
#include <vector>
#include <cmath>
#include <thread>
#include <sys/time.h>
#include <algorithm>
using namespace std;

// --- Criba simple para obtener primos hasta sqrt(n)
vector<long> primosBaseHastaRaiz(long n) {
    long limite = floor(sqrt(n));
    vector<bool> esPrimo(limite + 1, true);
    esPrimo[0] = esPrimo[1] = false;
    for (long i = 2; i * i <= limite; i++) {
        if (esPrimo[i]) {
            for (long j = i * i; j <= limite; j += i) {
                esPrimo[j] = false;
            }
        }
    }

    vector<long> primos;
    for (long i = 2; i <= limite; i++)
        if (esPrimo[i]) primos.push_back(i);
    return primos;
}

// --- Buscar primos en un rango [inicio, fin]
vector<long> buscarPrimosEnRango(long inicio, long fin, const vector<long>& primosBase) {
    vector<bool> esPrimo(fin - inicio + 1, true);

    for (long primo : primosBase) {
        long multInicial = max(primo * primo, ((inicio + primo - 1) / primo) * primo);
        for (long j = multInicial; j <= fin; j += primo) {
            esPrimo[j - inicio] = false;
        }
    }

    vector<long> primosLocales;
    for (long i = inicio; i <= fin; i++) {
        if (i >= 2 && esPrimo[i - inicio]) {
            primosLocales.push_back(i);
        }
    }
    return primosLocales;
}

int main() {
    long n;
    cout << "Ingrese un número menor a 10^7: ";
    cin >> n;
    while (n > 10000000 || n <= 1) {
        cout << "Número incorrecto, intente de vuelta: ";
        cin >> n;
    }

    timeval start, end;
    gettimeofday(&start, NULL);

    // Paso 1: obtener primos base hasta sqrt(n)
    vector<long> primosBase = primosBaseHastaRaiz(n);

    // Paso 2: determinar cantidad de hilos
    int numHilos = thread::hardware_concurrency();
    if (numHilos == 0) numHilos = 4;

    vector<thread> hilos;
    vector<vector<long>> resultados(numHilos);

    long rango = n / numHilos;

    // Paso 3: crear hilos para distintos subrangos
    for (int i = 0; i < numHilos; i++) {
        long inicio = i * rango + 1;
        long fin = (i == numHilos - 1) ? n : (i + 1) * rango;
        hilos.emplace_back([&, i, inicio, fin]() {
            resultados[i] = buscarPrimosEnRango(inicio, fin, primosBase);
        });
    }

    // Paso 4: esperar a todos los hilos
    for (auto& hilo : hilos) hilo.join();

    // Paso 5: combinar resultados
    vector<long> primosTotales;
    for (auto& vec : resultados) {
        primosTotales.insert(primosTotales.end(), vec.begin(), vec.end());
    }

    sort(primosTotales.begin(), primosTotales.end());

    gettimeofday(&end, NULL);

    // Mostrar resultados
    cout << "Hay " << primosTotales.size() << " números primos menores que " << n << ", siendo los mayores:" << endl;
    int j = 0;
    for (int i = primosTotales.size() - 1; i >= 0 && j < 10; i--, j++) {
        cout << primosTotales[i] << endl;
    }

    cout << "..." << endl;
    cout << "Número de hilos utilizados: " << numHilos << endl;
    cout << "Tiempo de ejecución: "
         << double(end.tv_sec - start.tv_sec) + double(end.tv_usec - start.tv_usec) / 1e6
         << " segundos" << endl;

    return 0;
}
