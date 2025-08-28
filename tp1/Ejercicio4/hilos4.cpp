#include <iostream>
#include <vector>
#include <cmath>
#include <sys/time.h>
#include <thread>
#include <mutex>
#include <atomic>
#include <algorithm>
using namespace std;

mutex mtx;
vector<long int> primosGlobal = {2};

void comprobar(int n) {
    while (n > 10000000 || n <= 1) {
        cout << "Número incorrecto intente de vuelta: " << endl;
        cin >> n;
    }
}

void imprimir(vector<long int> array) {
    int j = 0;
    for (int i = array.size()-1; i >= 0; i--) {
        cout << array[i] << endl;
        j += 1;
        if (j == 10) {
            break;
        }
    }
}

// Función thread-safe para verificar y agregar primos
void verificarYAgregarPrimo(int numero) {
    if (numero < 3 || numero % 2 == 0) return;
    
    double raiz = sqrt(numero);
    int limite = floor(raiz);
    bool esPrimo = true;
    
    // Obtener copia local de primos para verificación
    vector<long int> primosLocal;
    {
        lock_guard<mutex> lock(mtx);
        primosLocal = primosGlobal;
    }
    
    for (long primo : primosLocal) {
        if (primo > limite) break;
        if (numero % primo == 0) {
            esPrimo = false;
            break;
        }
    }
    
    if (esPrimo) {
        lock_guard<mutex> lock(mtx);
        // Verificar nuevamente por si otro hilo ya agregó este primo
        bool yaExiste = false;
        for (long p : primosGlobal) {
            if (p == numero) {
                yaExiste = true;
                break;
            }
        }
        if (!yaExiste) {
            primosGlobal.push_back(numero);
        }
    }
}

int main() {
    timeval start, end;
    long int n;
    cout << "Ingrese un número menor a 10^7: ";
    cin >> n;
    comprobar(n);
    
    if (n == 2) {
        cout << "Hay 0 números primos menores a 2.";
        return 0; 
    }
    
    gettimeofday(&start, NULL);
    
    int numHilos = thread::hardware_concurrency();
    if (numHilos == 0) numHilos = 4;
    
    vector<thread> hilos;
    
    // Crear hilos para procesar números impares
    for (int i = 0; i < numHilos; i++) {
        hilos.emplace_back([n, numHilos, i]() {
            for (int numero = 3 + i * 2; numero <= n; numero += numHilos * 2) {
                verificarYAgregarPrimo(numero);
            }
        });
    }
    
    for (auto& hilo : hilos) {
        hilo.join();
    }
    
    // Ordenar los primos
    sort(primosGlobal.begin(), primosGlobal.end());
    
    gettimeofday(&end, NULL);

    cout << "Hay " << primosGlobal.size() << " números primos menores que " << n << ", siendo los mayores:" << endl;
    imprimir(primosGlobal);
    cout << "..." << endl;
    cout << "Número de hilos utilizados: " << numHilos << endl;
    cout << "Tiempo de ejecución: " << double(end.tv_sec - start.tv_sec) +
         double(end.tv_usec - start.tv_usec)/1000000 << " segundos" << endl;

    return 0;
}