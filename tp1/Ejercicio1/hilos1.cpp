#include <iostream>
#include <cmath>
#include <iomanip>
#include <sys/time.h>
#include <thread>
#include <mutex>
using namespace std;

void chequear1(long double& x){
    while (x <= 1500000)
    {
        cout << "Número inválido, intente de vuelta: ";
        cin >> x;
    }
}

void chequear2(int x){
    while (x <= 0 || 1000000 % x != 0) {
        cout << "Número inválido, intente de vuelta: ";
        cin >> x;
    }
}

void sumar(int inicio,int final,long double& resultado,mutex& mtx, long double aux){
    long double suma = 0.0;
    for (int i = inicio; i <= final; i ++){
        long double termino = (1.0 / (2 * i + 1)) * pow(aux, 2 * i + 1);
        suma += termino;
    }
    mtx.lock();
    resultado += suma;
    mtx.unlock();
}


int main(){
    long double numero,resultado = 0.0,aux;
    int hilos;
    timeval start,end;
    mutex mtx;

    cout << "Ingrese un número mayor a 1.500.000:";
    cin >> numero;
    chequear1(numero);

    cout << "Ingrese el número de hilos: ";
    cin >> hilos;
    chequear2(hilos);

    //Armar array con tamaño de hilos y inicializar
    const int total_terminos = 1000000;
    thread array[hilos];
    aux = ( (numero-1) / (numero+1) );

    //Inicializar los hilos y el speedup
    gettimeofday(&start, NULL);
    int inicio = 0;
    int terminos_por_hilo = total_terminos / hilos;

    for (int i = 0 ; i < hilos - 1 ; i++){
        int final = (i == hilos - 1) ? total_terminos : inicio + terminos_por_hilo;
        array[i] = thread(sumar,inicio,final,ref(resultado),ref(mtx),aux);
        inicio = final;
    } 
    for (int i = 0; i < hilos; i++){
        if (array[i].joinable() == true){
            array[i].join();
        }
    }

    resultado = resultado * 2;
    gettimeofday(&end, NULL);
    cout << "El resultado es: " << setprecision(15) << resultado << endl;
    cout << "El valor verdadero es: " << setprecision(15) << log(numero) << endl;
    cout << "Tiempo de ejecución: " << double(end.tv_sec - start.tv_sec) +
    double(end.tv_usec - start.tv_usec) / 1000000 << " segundos" << endl;
    return 0;

}