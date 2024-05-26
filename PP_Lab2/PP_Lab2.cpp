#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <fstream>
#include <direct.h>
#include <stdio.h>
#include <string>
#include <omp.h> 

using namespace std;
using namespace std::chrono;


void writeMatrixToFile(const vector<vector<int>>& matrix, const string& filename) {
    ofstream file(filename);
    for (const auto& row : matrix) {
        for (const auto& element : row) {
            file << element << " ";
        }
        file << "\n";
    }
    file.close();
}


vector<vector<int>> generateMatrix(int size) {
    vector<vector<int>> matrix(size, vector<int>(size));
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> distrib(-100, 100);

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            matrix[i][j] = distrib(gen);
        }
    }

    return matrix;
}




vector<vector<int>> multiplyMatrices(const vector<vector<int>>& A, const vector<vector<int>>& B) {
    int n = A.size();
    vector<vector<int>> C(n, vector<int>(n, 0));
    int i, j, k;
#pragma omp parallel for shared(A, B, C)  private(i, j, k)
    for ( i = 0; i < n; i++) {
        for ( j = 0; j < n; j++) {
            int sum = 0;
            for ( k = 0; k < n; k++) {
                sum += A[i][k] * B[k][j];
            }
            C[i][j] = sum;
        }
    }

    return C;
}

int main() { 
    omp_set_num_threads(omp_get_max_threads());
    
    setlocale(LC_ALL, "RU");
    _mkdir("dataOpenMP");
    _mkdir("dataOpenMP/matrix");

    ofstream resultFile("dataOpenMP/ResultExperimentOpenMP.txt");

    vector<int> sizes = { 10, 25 ,50, 100, 250, 500, 600, 700 ,800, 900, 1000, 1200, 1400, 1600, 1800 };

    for (int size : sizes) {

        auto A = generateMatrix(size);
        auto B = generateMatrix(size);
        auto fileNameForA = "matrix_" + to_string(size) + "_A" + ".txt";
        auto fileNameForB = "matrix_" + to_string(size) + "_B" + ".txt";
        writeMatrixToFile(A, "dataOpenMP/matrix/" + fileNameForA);
        writeMatrixToFile(B, "dataOpenMP/matrix/" + fileNameForB);

        auto start = high_resolution_clock::now();
        cout << "Выполняются расчеты для матриц размером" << size << "X" << size << endl;
        auto C = multiplyMatrices(A, B);
        auto stop = high_resolution_clock::now();

        writeMatrixToFile(C, "dataOpenMP/matrix/matrixRes_" + to_string(size) + ".txt");

        auto duration = duration_cast<milliseconds>(stop - start);
        resultFile << "For matrix size" << size << "x" << size << " time -  " << duration.count() << " ms" << endl;
        cout << "Для матрицы размером" << size << "x" << size << " расчесты выполнены " << endl;
    }

    resultFile.close();
    return 0;
}
