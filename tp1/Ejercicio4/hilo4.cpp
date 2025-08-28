#include <iostream>
#include <vector>
#include <cmath>
#include <sys/time.h>
using namespace std;


void comprobar(int n){
    while (n > 10000000 || n <= 1){
        cout << "Número incorrecto intente de vuelta: " << endl;
        cin >> n;
    }
}

void imprimir(vector<long int> array){
    int j = 0;
    for (int i = array.size()-1 ; i >= 0 ; i--){
        cout << array[i] << endl;
        j += 1;
        if (j == 10){
            break;
        }
    }
}

int main(){
    timeval start,end;
    long int n;
    cout << "Ingrese un número menos a 10^7: ";
    cin >> n;
    comprobar(n);
    if (n == 2){
        cout << "Hay 0 números primos menores a 2.";
        return 0; 
    }
    
    gettimeofday(&start,NULL);
    vector<long int> primos = {2};
    
    for (int i = 3 ; i < (n+1) ; i += 2){
        double raiz = sqrt(i);
        int limite = floor(raiz);
        bool esPrimo = true;
        
        // Verificar divisibilidad solo con primos ya encontrados
        for (int j = 0; j < primos.size(); j++){
            // Si el primo actual es mayor que la raíz, podemos parar
            if (primos[j] > limite) {
                break;
            }
            if (i % primos[j] == 0){
                esPrimo = false;
                break;
            }
        }
        
        if (esPrimo){
            primos.push_back(i);
        }
    }
    gettimeofday(&end,NULL);

    cout << "Hay " << primos.size() << " números primos menores que " << n << ", siendo los mayores:" << endl;
    imprimir(primos);
    cout << "..." << endl;
    cout << "Tiempo de ejecución: " << double(end.tv_sec - start.tv_sec) +
         double(end.tv_usec - start.tv_usec)/1000000 << " segundos" << endl;

    return 0;
}