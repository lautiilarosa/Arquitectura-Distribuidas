#include <iostream>
#include <cmath>
#include <cstdlib>
#include <vector>
#include <sys/time.h>

using namespace std;

void comprobar(int n){
    while (n > 3000 || n <= 0){
        cout << "El tamño es incorrecto, por favor intente de vuelta: ";
        cin >> n;
    }
}

void generarmatrices(vector<vector<float>> &A , vector<vector<float>> &B, int n){
    for (int i = 0; i < n; i++){
        for (int j = 0; j < n; j++){
            A[i][j] = rand() % 10000;
            B[i][j] = rand() % 10000;
        }
    }
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
    // Ingresar el tamaño de las matrices
    timeval start,end;
    int tamaño;
    cout << "Ingrese el tamaño de las matrices: " ;
    cin >> tamaño;
    comprobar(tamaño);

    // inicializar las matrices de tamaaño nxn con números aleatorios
    vector<vector<float>> matrizA(tamaño,vector<float>(tamaño));
    vector<vector<float>> matrizB(tamaño,vector<float>(tamaño));
    generarmatrices(matrizA, matrizB, tamaño);

    //Multiplicar matrices y sumatoria
    vector<vector<float>> matrizC(tamaño,vector<float>(tamaño));
    float sumatoria = 0;
    gettimeofday(&start,NULL);
    for (int i = 0 ; i < tamaño ; i ++ ){
        for (int j = 0 ; j < tamaño ; j ++ ){
            for (int k = 0 ; k < tamaño ; k ++ ) {
                matrizC[i][j] = matrizC[i][j] + (matrizA[i][k] * matrizB[k][j]); 
            }
        }
    }

    for (int x = 0 ; x < tamaño ; x ++){
        for (int y = 0 ; y < tamaño ; y ++){
            sumatoria = sumatoria + matrizC[x][y];
        }
    }

    gettimeofday(&end,NULL);
    
    // Imprimir Resultados

    cout << "Extremos de Las matrices: " << endl;
    cout << "[ " << matrizC[0][0] << "......... " << matrizC[0][tamaño-1] << " ]" << endl;
    cout << "[ " << matrizC[tamaño-1][0] << "......... " << matrizC[tamaño-1][tamaño-1] << " ]" << endl;
    cout << "El resultado de la sumatoria es: " << sumatoria << endl;
    cout << "Tiempo de ejecución: " << double(end.tv_sec - start.tv_sec) +
         double(end.tv_usec - start.tv_usec)/1000000 << " segundos" << endl;
    

}