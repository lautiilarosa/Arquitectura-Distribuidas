#include <iostream>
#include <fstream>
#include <vector>
#include <thread>
#include <sys/time.h>
using namespace std;

void contar_patron(const vector<string>& patrones, int inicio, int fin, vector<int>& contadores, const string& texto) {
    for (int i = inicio; i < fin; i++) {
        int pos = 0;
        while (true) {
            pos = texto.find(patrones[i], pos);
            if (pos == string::npos) break;
            contadores[i]++;
            pos++;  
        }
    }
}

int main() {
    ifstream patronesFile("patrones.txt");
    if (!patronesFile) {
        cout << "No se pudo abrir patrones.txt" << endl;
        return 1;
    }

    vector<string> patrones;
    string patron;
    while (getline(patronesFile, patron)) {
        patrones.push_back(patron);
    }
    patronesFile.close();

    ifstream textoFile("texto.txt");
    if (!textoFile) {
        cout << "No se pudo abrir texto.txt" << endl;
        return 1;
    }

    string texto;
    getline(textoFile, texto);
    textoFile.close();

    vector<int> contadores(patrones.size(), 0);

    timeval t1, t2;
    gettimeofday(&t1, NULL);
    
    const int num_hilos = 32; 
    int patrones_por_hilo = patrones.size() / num_hilos;
    int patrones_extra = patrones.size() % num_hilos;
    
    vector<thread> hilos;
    int inicio = 0;

    for (int i = 0; i < num_hilos; i++) {

        int fin = inicio + patrones_por_hilo + (i < patrones_extra ? 1 : 0);
        

        hilos.push_back(thread(contar_patron, cref(patrones), inicio, fin, ref(contadores), cref(texto)));        
     
        inicio = fin;
    }


    for (auto& h : hilos) {
        h.join();
    }
    
    
    

    gettimeofday(&t2, NULL);

    for (int i = 0; i < patrones.size(); i++) {
        cout << "el patron " << i << " aparece " << contadores[i] << " veces" << endl;
    }

    double tiempo = (t2.tv_sec - t1.tv_sec) + (t2.tv_usec - t1.tv_usec) / 1000000.0;
    cout << "Tiempo de ejecucion : " << tiempo << " segundos" << endl;

    return 0;
}