#include <iostream>
#include <cmath>
#include <cstdlib>
#include <vector>

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

void imprimirmatriz(const vector<vector<float>> &M, int n){
    for (int i = 0; i < n; i++){
        for (int j = 0; j < n; j++){
            cout << M[i][j] << " ";
        }
        cout << endl;
    }
}



int main(){
    // Ingresar el tamaño de las matrices
    int tamaño;
    cout << "Ingrese el tamaño de las matrices: " ;
    cin >> tamaño;
    comprobar(tamaño);

    // inicializar las matrices de tamaaño nxn con números aleatorios
    vector<vector<float>> matrizA(tamaño,vector<float>(tamaño));
    vector<vector<float>> matrizB(tamaño,vector<float>(tamaño));
    generarmatrices(matrizA, matrizB, tamaño);

    //Imprimir Matrices
    imprimirmatriz(matrizA, tamaño);
    cout << endl;
    imprimirmatriz(matrizB, tamaño);
    cout << endl;

    

}