#include <iostream>
#include <cmath>
#include <iomanip>
#include <sys/time.h>
using namespace std;

void chequear(long double x){
    while (x <= 1500000)
    {
        cout << "Número inválido,intente de vuelta:";
        cin >> x;
    }
}

int main() {
    long double x,resultado,aux;
    timeval start,end;
    cout << "Ingrese un número mayor a 1.500.000: ";
    cin >> x;
    chequear(x);
    aux = ( (x-1) / (x+1) );
    resultado = 0;
    gettimeofday(&start, NULL);

    for (int i = 0; i < 9999999; i++){
        resultado += (1.0 / (2*i + 1)) * pow(aux, 2*i + 1);
    }
    gettimeofday(&end, NULL);

    resultado = resultado * 2;
    cout << "El resultado es: " << setprecision(15) << resultado << endl;
    cout << "El valor verdadero es :" << setprecision(15) << log(x) << endl;
    cout << "Tiempo de ejecución: " << double(end.tv_sec - start.tv_sec) +
+ double(end.tv_usec-start.tv_usec)/1000000 << " segundos" << endl;
    return 0;

}



