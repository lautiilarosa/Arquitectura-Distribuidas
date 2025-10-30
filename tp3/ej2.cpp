#include <mpi.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sys/time.h>
#include <iomanip>

using namespace std;

// Función para contar cuántas veces aparece un patrón en el texto
long long contar_patron(const string& texto, const string& patron) {
    long long count = 0;
    size_t pos = 0;
    while ((pos = texto.find(patron, pos)) != string::npos) {
        ++count;
        ++pos; // avanzar 1 posición para permitir solapamiento
    }
    return count;
}

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size != 32) {
        if (rank == 0) cerr << "Este programa debe ejecutarse con 32 procesos." << endl;
        MPI_Finalize();
        return 1;
    }

    string texto;
    vector<string> patrones(32);

    // Solo el proceso 0 lee los archivos y difunde
    if (rank == 0) {
        // Leer texto
        ifstream file_texto("texto.txt", ios::in);
        if (!file_texto) {
            cerr << "No se pudo abrir texto.txt" << endl;
            MPI_Abort(MPI_COMM_WORLD, 1);
        }
        texto.assign((istreambuf_iterator<char>(file_texto)),
                      istreambuf_iterator<char>());
        file_texto.close();

        // Leer patrones
        ifstream file_patrones("patrones.txt");
        if (!file_patrones) {
            cerr << "No se pudo abrir patrones.txt" << endl;
            MPI_Abort(MPI_COMM_WORLD, 1);
        }
        string linea;
        for (int i = 0; i < 32; ++i) {
            getline(file_patrones, linea);
            patrones[i] = linea;
        }
        file_patrones.close();
    }

    // Difundir texto a todos los procesos
    long long texto_size = texto.size();
    MPI_Bcast(&texto_size, 1, MPI_LONG_LONG, 0, MPI_COMM_WORLD);
    if (rank != 0) texto.resize(texto_size);
    MPI_Bcast(&texto[0], texto_size, MPI_CHAR, 0, MPI_COMM_WORLD);

    // Difundir patrones
    for (int i = 0; i < 32; ++i) {
        long long pat_size = patrones[i].size();
        MPI_Bcast(&pat_size, 1, MPI_LONG_LONG, 0, MPI_COMM_WORLD);
        if (rank != 0) patrones[i].resize(pat_size);
        MPI_Bcast(&patrones[i][0], pat_size, MPI_CHAR, 0, MPI_COMM_WORLD);
    }

    MPI_Barrier(MPI_COMM_WORLD); // sincronizar antes de medir
    timeval t1{}, t2{};
    if (rank == 0) gettimeofday(&t1, nullptr);

    // Cada proceso busca su patrón
    long long count = contar_patron(texto, patrones[rank]);

    // Reunir resultados en proceso 0
    vector<long long> resultados(32, 0);
    MPI_Gather(&count, 1, MPI_LONG_LONG, resultados.data(), 1, MPI_LONG_LONG, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        gettimeofday(&t2, nullptr);
        double tiempo = (t2.tv_sec - t1.tv_sec) + (t2.tv_usec - t1.tv_usec) / 1e6;

        // Mostrar resultados
        for (int i = 0; i < 32; ++i) {
            cout << "el patron " << i << " aparece " << resultados[i] << " veces." << endl;
        }
        cout << "Tiempo total: " << tiempo << " s" << endl;
    }

    MPI_Finalize();
    return 0;
}
