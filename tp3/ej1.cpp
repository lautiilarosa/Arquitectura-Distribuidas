#include <mpi.h>
#include <iostream>
#include <iomanip>
#include <cmath>
#include <sys/time.h>
#include <limits>

using namespace std;

long double calcular_parcial(long double y, long double y2, long long inicio, long long fin) {
    if (inicio >= fin) return 0.0L;

    // y^(2*inicio + 1)
    long double pot = y * powl(y2, (long double)inicio);
    long double suma = 0.0L;

    for (long long k = inicio; k < fin; ++k) {
        long long denom = 2 * k + 1;
        suma += pot / denom;
        pot *= y2;  // siguiente potencia
    }
    return suma;
}

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    long double x = 1500000.0L;
    const long long N = 10000000LL;

    if (rank == 0) {
        cout << "Ingrese x ";
        long double x_in;
        if (cin >> x_in) x = x_in;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    MPI_Bcast(&x, 1, MPI_LONG_DOUBLE, 0, MPI_COMM_WORLD);

    if (x < 1500000.0L) {
        if (rank == 0) cerr << "x debe ser >= 1.5e6" << endl;
        MPI_Finalize();
        return 1;
    }

    long double y = (x - 1.0L) / (x + 1.0L);
    long double y2 = y * y;

    // División balanceada del trabajo
    long long base = N / size;
    long long resto = N % size;
    long long inicio = rank * base + min<long long>(rank, resto);
    long long cantidad = base + (rank < resto ? 1 : 0);
    long long fin = inicio + cantidad;

    // Sincronizar procesos antes de medir el tiempo
    MPI_Barrier(MPI_COMM_WORLD);
    timeval t1{}, t2{};
    if (rank == 0) gettimeofday(&t1, nullptr);

    long double parcial = calcular_parcial(y, y2, inicio, fin);

    long double total = 0.0L;
    MPI_Reduce(&parcial, &total, 1, MPI_LONG_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        gettimeofday(&t2, nullptr);
        double tiempo = (t2.tv_sec - t1.tv_sec) + (t2.tv_usec - t1.tv_usec) / 1e6;

        long double ln_x = 2.0L * total;
        cout << fixed << setprecision(15);
        cout << "ln(" << x << ") ≈ " << ln_x << endl;
        cout << "Tiempo total: " << tiempo << " s" << endl;
        cout << "Valor real (std::log): " << log((double)x) << endl;
    }

    MPI_Finalize();
    return 0;
}
