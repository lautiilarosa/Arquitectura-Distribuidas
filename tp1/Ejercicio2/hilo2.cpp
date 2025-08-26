#include <iostream>
#include <iomanip>
#include <sys/time.h>
#include <fstream>
#include <map>
using namespace std;

int main(){
    // Hacemos un map con los 32 patrones
    ifstream patrontxt;
    map<string,int> patrones;
    string linea;
    string palabra;
    timeval start,end;

    patrontxt.open("patrones.txt");

    while (getline(patrontxt,palabra)) {
        patrones.insert({palabra,0});
    }
    patrontxt.close();
    cout << "Total patrones: " << patrones.size() << endl << endl;


    gettimeofday(&start, NULL);
    // Abrimos el archivo de texto y buscamos los patrones
    ifstream texto;
    texto.open("texto.txt");

    while (getline(texto, linea)) {
        for (auto& [patron, contador] : patrones) {
            size_t pos = 0;
            while ((pos = linea.find(patron, pos)) != string::npos) {
                contador++;
                pos += patron.length(); // Avanza la longitud del patrón
            }
        }
    }

    gettimeofday(&end, NULL);
    
    texto.close();
    for (const auto& [patron,contador] : patrones){
        cout << "El patrón: "<< patron << " aparece: " << contador << " veces." << endl;
    }
    cout << "Tiempo de ejecución: " << double(end.tv_sec - start.tv_sec) +
         double(end.tv_usec - start.tv_usec)/1000000 << " segundos" << endl;
    
    return 0;
}

/*  
#include <iostream>
#include <cmath>
#include <iomanip>
#include <sys/time.h>
#include <thread>
#include <fstream>
#include <mutex>
#include <vector>
#include <map>
using namespace std;

int main(){
    ifstream patrontxt;
    vector<pair<string, int>> patrones;  // Usamos vector para mantener duplicados
    string linea;
    string palabra;
    timeval start,end;

    // Leer TODOS los patrones (incluyendo duplicados)
    patrontxt.open("patrones.txt");
    if (!patrontxt.is_open()) {
        cout << "Error: No se pudo abrir patrones.txt" << endl;
        return 1;
    }
    
    cout << "Leyendo patrones (incluyendo duplicados):" << endl;
    int contador_patrones = 0;
    while (patrontxt >> palabra) {
        patrones.push_back({palabra, 0});
        cout << contador_patrones + 1 << ": '" << palabra << "'" << endl;
        contador_patrones++;
    }
    patrontxt.close();
    
    cout << "\nTotal patrones leídos: " << contador_patrones << endl;

    gettimeofday(&start, NULL);
    
    ifstream texto;
    texto.open("texto.txt");
    if (!texto.is_open()) {
        cout << "Error: No se pudo abrir texto.txt" << endl;
        return 1;
    }

    // Contar ocurrencias para cada patrón (incluyendo duplicados)
    while (getline(texto, linea)) {
        for (auto& par : patrones) {
            string& patron = par.first;
            int& contador = par.second;
            
            size_t pos = 0;
            while ((pos = linea.find(patron, pos)) != string::npos) {
                contador++;
                pos += patron.length();
            }
        }
    }
    texto.close();

    gettimeofday(&end, NULL);
    
    // Mostrar resultados de los 32 patrones
    cout << "\nRESULTADOS:" << endl;
    for (size_t i = 0; i < patrones.size(); i++){
        cout << "Patrón " << i + 1 << ": " << patrones[i].first 
             << " aparece: " << patrones[i].second << " veces." << endl;
    }
    
    double tiempo = double(end.tv_sec - start.tv_sec) + 
                   double(end.tv_usec - start.tv_usec)/1000000;
    cout << "\nTiempo de ejecución: " << tiempo << " segundos" << endl;
    
    return 0;
}


*/