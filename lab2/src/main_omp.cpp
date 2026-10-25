#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <iomanip>
#include <omp.h>
#include <cstdlib>

using namespace std;

vector<vector<double>> readMatrix(const string& filename, int& n) {
    ifstream file(filename);
    if (!file) {
        cerr << "Error: cannot open file " << filename << endl;
        exit(1);
    }
    file >> n;
    vector<vector<double>> matrix(n, vector<double>(n));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            file >> matrix[i][j];
        }
    }
    return matrix;
}

void writeMatrix(const string& filename, const vector<vector<double>>& matrix) {
    ofstream file(filename);
    if (!file) {
        cerr << "Error: cannot create file " << filename << endl;
        exit(1);
    }
    int n = matrix.size();
    file << n << '\n' << fixed << setprecision(6);
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            file << matrix[i][j] << (j + 1 < n ? ' ' : '\n');
        }
    }
}

int main(int argc, char* argv[]) {
    if (argc != 4) {
        cerr << "Usage: matrix_mult_omp <matrix_A> <matrix_B> <result>" << endl;
        return 1;
    }

    int num_threads = 4;
    if (const char* env_threads = getenv("OMP_NUM_THREADS")) {
        num_threads = atoi(env_threads);
    }
    omp_set_num_threads(num_threads);

    int nA, nB;
    vector<vector<double>> A = readMatrix(argv[1], nA);
    vector<vector<double>> B = readMatrix(argv[2], nB);

    if (nA != nB) {
        cerr << "Error: matrices must have the same size." << endl;
        return 1;
    }

    int n = nA;
    vector<vector<double>> C(n, vector<double>(n, 0.0));

    auto start = chrono::high_resolution_clock::now();

    #pragma omp parallel for schedule(static)
    for (int i = 0; i < n; ++i) {
        for (int k = 0; k < n; ++k) {
            double a_ik = A[i][k];
            for (int j = 0; j < n; ++j) {
                C[i][j] += a_ik * B[k][j];
            }
        }
    }

    auto finish = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed = finish - start;

    double operations = 2.0 * n * n * n;
    double gflops = operations / (elapsed.count() * 1e9);

    writeMatrix(argv[3], C);

    cout << fixed << setprecision(6);
    cout << "MATRIX_SIZE: " << n << "\n";
    cout << "THREADS: " << num_threads << "\n";
    cout << "TIME_SECONDS: " << elapsed.count() << "\n";
    cout << "OPERATIONS: " << operations << "\n";
    cout << "PERFORMANCE_GFLOPS: " << gflops << "\n";

    return 0;
}