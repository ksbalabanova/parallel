#include <iostream>
#include <vector>
#include <chrono>
#include <fstream>
#include <string>

using Matrix = std::vector<std::vector<double>>;

// создание матрицы n x n, заполненной нулями
Matrix create_mat(int n) {
    return Matrix(n, std::vector<double>(n, 0.0));
}

// заполнение матрицы A
void fill_matA(Matrix& A) {
    int n = A.size();
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            A[i][j] = i + j;
        }
    }
}

// заполнение матрицы B
void fill_matB(Matrix& B) {
    int n = B.size();
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            B[i][j] = i - j;
        }
    }
}

// классическое умножение матриц
void mul_mat(const Matrix& A, const Matrix& B, Matrix& C) {
    int n = A.size();

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            double sum = 0.0;

            for (int k = 0; k < n; k++) {
                sum += A[i][k] * B[k][j];
            }

            C[i][j] = sum;
        }
    }
}

// контрольная сумма результирующей матрицы
double checksum(const Matrix& C) {
    double sum = 0.0;
    int n = C.size();

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            sum += C[i][j];
        }
    }

    return sum;
}

// поиск минимального времени
long long find_min(const std::vector<long long>& times) {
    long long min_value = times[0];

    for (int i = 1; i < times.size(); i++) {
        if (times[i] < min_value) {
            min_value = times[i];
        }
    }

    return min_value;
}

// поиск максимального времени
long long find_max(const std::vector<long long>& times) {
    long long max_value = times[0];

    for (int i = 1; i < times.size(); i++) {
        if (times[i] > max_value) {
            max_value = times[i];
        }
    }

    return max_value;
}

// поиск среднего времени
double find_avg(const std::vector<long long>& times) {
    long long sum = 0;

    for (int i = 0; i < times.size(); i++) {
        sum += times[i];
    }

    return static_cast<double>(sum) / times.size();
}

// сохранение результатов в csv-файл в папку results
void save_to_csv(const std::string& filename, const std::vector<long long>& times) {
    std::ofstream file("/Users/kseniabalabanova/CLionProjects/parallel/lab1/results/" + filename);
    file << "run,time_microseconds\n";
    for (int i = 0; i < times.size(); i++) {
        file << i + 1 << "," << times[i] << "\n";
    }
    file.close();
}

int main() {
    // Первый запуск для проверки корректности работы функции
    /*
    int testN = 3;
    Matrix tA = create_mat(testN);
    Matrix tB = create_mat(testN);
    Matrix tC = create_mat(testN);

    fill_matA(tA);
    fill_matB(tB);

    mul_mat(tA, tB, tC);

    double check = checksum(tC);

    std::cout << "тест N = " << testN << std::endl;
    std::cout << "сумма = " << check << std::endl;
    std::cout << "A:" << std::endl;
    for (int i = 0; i < testN; i++) {
        for (int j = 0; j < testN; j++) {
            std::cout << tA[i][j] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;

    std::cout << "B:" << std::endl;
    for (int i = 0; i < testN; i++) {
        for (int j = 0; j < testN; j++) {
            std::cout << tB[i][j] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
    std::cout << "C = A * B:" << std::endl;
    for (int i = 0; i < testN; i++) {
        for (int j = 0; j < testN; j++) {
            std::cout << tC[i][j] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
    std::cout << "тест N = " << testN << std::endl;
    std::cout << "сумма = " << check << std::endl;
    */

    int N = 200;
    int K = 10;

    Matrix A = create_mat(N);
    Matrix B = create_mat(N);
    Matrix C = create_mat(N);

    fill_matA(A);
    fill_matB(B);


    /*один замер времени
    auto start = std::chrono::steady_clock::now();
    mul_mat(A, B, C);
    auto stop = std::chrono::steady_clock::now();
    auto time = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    std::vector<long long> single_time;
    single_time.push_back(time.count());
    save_to_csv("single_run.csv", single_time);
    std::cout << "один запуск:" << std::endl;
    std::cout << "время = " << time.count() << " микросекунд" << std::endl;
    std::cout << "сумма = " << checksum(C) << std::endl;
    */

    /*серия измерений
    std::vector<long long> times_no_warmup;
    for (int i = 0; i < K; i++) {
        auto start = std::chrono::steady_clock::now();
        mul_mat(A, B, C);
        auto stop = std::chrono::steady_clock::now();
        auto t = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
        times_no_warmup.push_back(t.count());
    }
    save_to_csv("results_no_warmup.csv", times_no_warmup);
    std::cout << "N = " << N << std::endl;
    std::cout << "K = " << K << std::endl;
    std::cout << "сумма = " << checksum(C) << std::endl;
    std::cout << "все времена:" << std::endl;
    for (int i = 0; i < times_no_warmup.size(); i++) {
        std::cout << i + 1 << ") " << times_no_warmup[i] << std::endl;
    }
    std::cout << std::endl;
    std::cout << "min = " << find_min(times_no_warmup) << std::endl;
    std::cout << "max = " << find_max(times_no_warmup) << std::endl;
    std::cout << "avg = " << find_avg(times_no_warmup) << std::endl;
    */


    //прогрев и серия измерений

    mul_mat(A, B, C);
    std::vector<long long> times_with_warmup;
    for (int i = 0; i < K; i++) {
        auto start = std::chrono::steady_clock::now();
        mul_mat(A, B, C);
        auto stop = std::chrono::steady_clock::now();
        auto t = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
        times_with_warmup.push_back(t.count());
    }
    save_to_csv("results_with_warmup.csv", times_with_warmup);
    std::cout << "N = " << N << std::endl;
    std::cout << "K = " << K << std::endl;
    std::cout << "сумма = " << checksum(C) << std::endl;
    std::cout << "все времена после прогрева:" << std::endl;
    for (int i = 0; i < times_with_warmup.size(); i++) {
        std::cout << i + 1 << ") " << times_with_warmup[i] << std::endl;
    }
    std::cout << std::endl;
    std::cout << "min = " << find_min(times_with_warmup) << std::endl;
    std::cout << "max = " << find_max(times_with_warmup) << std::endl;
    std::cout << "avg = " << find_avg(times_with_warmup) << std::endl;
    //*/

    return 0;
}
