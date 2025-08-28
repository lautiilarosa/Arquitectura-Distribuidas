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


    // Abrimos el archivo de texto y buscamos los patrones
    ifstream texto;
    texto.open("texto.txt");
    gettimeofday(&start, NULL);
    while (getline(texto, linea)) {
        for (auto& [patron, contador] : patrones) {
            int pos = 0;
            while (pos < linea.length()) {
                int found_pos = linea.find(patron, pos);
                if (found_pos == string::npos) {
                    break; // No se encontró más ocurrencias
                }
                contador++;
                pos = found_pos + 1; // Avanza 1 posición para buscar coincidencias superpuestas
            }
        }
    }

    gettimeofday(&end, NULL);
    
    texto.close();
    for (const auto& [patron,contador] : patrones){
        if (contador > 0){
            cout << "El patrón: "<< patron << " aparece: " << contador << " veces." << endl;
        }    
    }
    cout << "Los demás patrones aparecen 0 veces en el texto." << endl << endl;
    cout << "Tiempo de ejecución: " << double(end.tv_sec - start.tv_sec) +
         double(end.tv_usec - start.tv_usec)/1000000 << " segundos" << endl;
    
    return 0;
}

