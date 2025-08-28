#include <iostream>
#include <iomanip>
#include <sys/time.h>
#include <fstream>
#include <map>
#include <thread>
#include <mutex>
#include <algorithm>
#include <vector>
#include <string>
#include <fstream>
using namespace std;


/*
void buscarpatron(string contenido,string patron){
    int contador = 0;
    for (int i = 0; i < contenido.size(); i++){
        int found_pos = contenido.find(patron,i);
        if (found_pos != string::npos){
            contador ++;
        }
    }

    cout << "El patrón: " << patron << " apareció: " << contador << " veces" << endl;

}


int main(){
    ifstream patrontxt("patrones.txt");
    vector<string> patrones;
    string palabra;
    string contenido;
    timeval start,end;

    // Agregar la cantidad de patrones en una lista
    while (getline(patrontxt,palabra)){
        auto found = find(patrones.begin(),patrones.end(),palabra);
        if (found == patrones.end()){
            patrones.push_back(palabra);
        }
        
    }
    patrontxt.close();
    cout << "Total patrones: " << patrones.size() << endl;

    //Pasar texto a string y inicializar array de threads
    vector<thread> hilos;
    ifstream archivo("texto.txt");

    if (archivo.is_open()){
        contenido = string((istreambuf_iterator<char>(archivo)),istreambuf_iterator<char>());
    }
    archivo.close();


    gettimeofday(&start, NULL);
    for (int i = 0;i < patrones.size();i ++ ){
        hilos.emplace_back(buscarpatron,contenido,patrones[i]);
    }   

    for (auto&t : hilos){
        t.join();
    }

    gettimeofday(&end,NULL);

    cout << "Tiempo de ejecución: " << double(end.tv_sec - start.tv_sec) +
         double(end.tv_usec - start.tv_usec)/1000000 << " segundos" << endl;


    return 0;
}
    */

// Función optimizada para contar patrones
int contar_patron(const string& contenido, const string& patron) {
    int contador = 0;
    size_t pos = 0;
    size_t patron_len = patron.length();
    
    while ((pos = contenido.find(patron, pos)) != string::npos) {
        contador++;
        pos += patron_len; // Saltar al final del patrón encontrado
    }
    return contador;
}

// Función para procesar un grupo de patrones
void procesar_grupo(const string& contenido, const vector<string>& patrones, 
                   size_t inicio, size_t fin, vector<int>& resultados) {
    for (size_t i = inicio; i < fin; i++) {
        resultados[i] = contar_patron(contenido, patrones[i]);
    }
}

int main() {
    ifstream patrontxt("patrones.txt");
    vector<string> patrones;
    string palabra;
    string contenido;
    timeval start, end;

    // Leer patrones
    while (getline(patrontxt, palabra)) {
        if (!palabra.empty()) {
            patrones.push_back(palabra);
        }
    }
    patrontxt.close();
    
    cout << "Total patrones: " << patrones.size() << endl;

    // Leer contenido
    ifstream archivo("texto.txt");
    if (archivo.is_open()) {
        contenido = string((istreambuf_iterator<char>(archivo)), istreambuf_iterator<char>());
    }
    archivo.close();

    // Determinar número óptimo de threads
    unsigned int num_threads = thread::hardware_concurrency();
    if (num_threads == 0) num_threads = 4; // Fallback
    
    // Agrupar patrones para threads
    size_t patrones_por_thread = patrones.size() / num_threads;
    vector<thread> hilos;
    vector<int> resultados(patrones.size());
    
    gettimeofday(&start, NULL);
    
    // Crear threads para procesar grupos de patrones
    for (unsigned int i = 0; i < num_threads; i++) {
        size_t inicio = i * patrones_por_thread;
        size_t fin = (i == num_threads - 1) ? patrones.size() : inicio + patrones_por_thread;
        
        hilos.emplace_back(procesar_grupo, ref(contenido), ref(patrones), 
                          inicio, fin, ref(resultados));
    }
    
    // Esperar a que terminen todos los threads
    for (auto& t : hilos) {
        t.join();
    }
    
    gettimeofday(&end, NULL);
    
    // Mostrar resultados
    for (size_t i = 0; i < patrones.size(); i++) {
        cout << "El patrón: " << patrones[i] << " apareció: " << resultados[i] << " veces" << endl;
    }
    
    double tiempo = double(end.tv_sec - start.tv_sec) + 
                   double(end.tv_usec - start.tv_usec) / 1000000;
    cout << "Tiempo de ejecución: " << tiempo << " segundos" << endl;
    cout << "Número de threads utilizados: " << num_threads << endl;

    return 0;
}