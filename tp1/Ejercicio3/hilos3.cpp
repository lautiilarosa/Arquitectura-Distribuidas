#include <iostream>
#include <cmath>
#include <cstdlib>
#include <vector>
#include <sys/time.h>
#include <thread>
#include <mutex>
using namespace std;

mutex mtx;

void comprobar(int n){
    while (n > 3000 || n <= 0){
        cout << "El tamño es incorrecto, por favor intente de vuelta: ";
        cin >> n;
    }
}

void generarmatrices(vector<vector<long double>> &A , vector<vector<long double>> &B, int n){
    for (int i = 0; i < n; i++){
        for (int j = 0; j < n; j++){
            A[i][j] = rand() % 10000;
            B[i][j] = rand() % 10000;
        }
    }
}

void multiplicar(vector<vector<long double>>& a,vector<vector<long double>>& b , vector<vector<long double>>& c , long double& global ,int start,int end ){
    long double sumalocal = 0;
    int n = a.size();

    for (int i = start ; i < end ; i++){
        for (int j = 0 ; j < n ; j++){
            int sum = 0;
            for (int k = 0 ; k < n ; k++){
                sum += a[i][k] * b[k][j];
            }
            sumalocal += sum;
            c[i][j] = sum;
        }
    }

    lock_guard<mutex> lock(mtx);
    global += sumalocal;

}

/*
void imprimirmatriz(const vector<vector<float>> &M){
    for (int i = 0; i < M.size(); i++){
        for (int j = 0; j < M.size(); j++){
            cout << M[i][j] << " ";
        }
        cout << endl;
    }
}

void generarmatrizcero (vector<vector<float>> &C, int n){
    for (int i = 0; i < n; i++){
        for (int j = 0; j < n; j++){
            C[i][j] = 0;
        }
    }
}
*/

int main(){
    // Inicializar semilla para números aleatorios
    srand(time(0));  // ¡IMPORTANTE!

    // Ingresar el tamaño de las matrices
    timeval start,end;
    int tamaño;
    cout << "Ingrese el tamaño de las matrices: " ;
    cin >> tamaño;
    comprobar(tamaño);

    // inicializar las matrices de tamaaño nxn con números aleatorios
    vector<vector<long double>> matrizA(tamaño,vector<long double>(tamaño));
    vector<vector<long double>> matrizB(tamaño,vector<long double>(tamaño));
    generarmatrices(matrizA, matrizB, tamaño);

    // Inicializar matriz C, array de hilos y multiplicar las matrices usando multihilos
    vector<vector<long double>> matrizC(tamaño,vector<long double>(tamaño));
    long double sumatoriaglobal = 0;

    vector<thread> hilos;
    int numthreads = (tamaño + 9) / 10;

    gettimeofday(&start,NULL);
    for (int i = 0; i < numthreads; i++){
        int startR = i*10;
        int endR = min(startR+10,tamaño);

        hilos.emplace_back(multiplicar,ref(matrizA),ref(matrizB),ref(matrizC),ref(sumatoriaglobal),startR,endR);
        
    }

    for (auto& t : hilos) {
        t.join();
    }
    gettimeofday(&end,NULL);

    //Mostrar resultados

    cout << "Extremos de Las matrices: " << endl;
    cout << "[ " << matrizC[0][0] << "......... " << matrizC[0][tamaño-1] << " ]" << endl;
    cout << "[ " << matrizC[tamaño-1][0] << "......... " << matrizC[tamaño-1][tamaño-1] << " ]" << endl;
    cout << "El resultado de la sumatoria es: " << sumatoriaglobal << endl;
    cout << "Tiempo de ejecución: " << double(end.tv_sec - start.tv_sec) +
         double(end.tv_usec - start.tv_usec)/1000000 << " segundos" << endl;
    






    return 0;
}