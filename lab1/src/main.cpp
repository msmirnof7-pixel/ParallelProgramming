#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <iomanip>

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

void writeMatrix(const string& filename,
                 const vector<vector<double>>& matrix) {
    ofstream file(filename);

    if (!file) {
        cerr << "Error: cannot create file " << filename << endl;
        exit(1);
    }

    int n = matrix.size();

    file << n << '\n';

    file << fixed << setprecision(6);

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            file << matrix[i][j];

            if (j + 1 < n)
                file << ' ';
        }

        file << '\n';
    }
}

int main(int argc, char* argv[]) {
    if (argc != 4) {
        cerr << "Usage: matrix_mult <matrix_A> <matrix_B> <result>" << endl;
        return 1;
    }

    string fileA = argv[1];
    string fileB = argv[2];
    string fileC = argv[3];

    int nA, nB;

    vector<vector<double>> A = readMatrix(fileA, nA);
    vector<vector<double>> B = readMatrix(fileB, nB);

    if (nA != nB) {
        cerr << "Error: matrices must have the same size." << endl;
        return 1;
    }

    int n = nA;

    vector<vector<double>> C(n, vector<double>(n, 0.0));

    auto start = chrono::high_resolution_clock::now();

    // Matrix multiplication: C = A * B
    for (int i = 0; i < n; ++i) {
        for (int k = 0; k < n; ++k) {
            for (int j = 0; j < n; ++j) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }

    auto finish = chrono::high_resolution_clock::now();

    chrono::duration<double> elapsed = finish - start;

    double operations = 2.0 * n * n * n;
    double gflops = operations / (elapsed.count() * 1e9);

    writeMatrix(fileC, C);

    cout << fixed << setprecision(6);
    cout << "Matrix size: " << n << " x " << n << endl;
    cout << "Execution time: " << elapsed.count() << " seconds" << endl;
    cout << "Task volume: " << operations << " floating-point operations" << endl;
    cout << "Performance: " << gflops << " GFLOPS" << endl;

    return 0;
}
