#include <iostream>
#include <chrono>
#include <iomanip>

#include "matrix_utils.h"
#include "multiply.h"

int main() {
    int n = 500;

    matrix a = create_matrix(n);
    matrix b = create_matrix(n);

    fill_matrix_random(a);
    fill_matrix_random(b);

    auto start_classic = std::chrono::high_resolution_clock::now();
    matrix c1 = mul_classic(a, b);
    auto end_classic = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> time_classic = end_classic - start_classic;

    auto start_transpose = std::chrono::high_resolution_clock::now();
    matrix c2 = mul_transpose(a, b);
    auto end_transpose = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> time_transpose = end_transpose - start_transpose;

    auto start_buffered = std::chrono::high_resolution_clock::now();
    matrix c3 = mul_buffered(a, b, 1);
    auto end_buffered = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> time_buffered = end_buffered - start_buffered;

    auto start_blocked = std::chrono::high_resolution_clock::now();
    matrix c4 = mul_blocked(a, b, 64, 1);
    auto end_blocked = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> time_blocked = end_blocked - start_blocked;

    bool equal_1 = are_equal(c1, c2);
    bool equal_2 = are_equal(c1, c3);
    bool equal_3 = are_equal(c1, c4);

    double gflops_classic = calc_gflops(n, time_classic.count());
    double gflops_transpose = calc_gflops(n, time_transpose.count());
    double gflops_buffered = calc_gflops(n, time_buffered.count());
    double gflops_blocked = calc_gflops(n, time_blocked.count());

    std::cout << std::fixed << std::setprecision(6);

    std::cout << "n = " << n << std::endl;
    std::cout << "mul_classic(a, b) = mul_transpose(a, b) ? " << (equal_1 ? "yes" : "no") << std::endl;
    std::cout << "mul_classic(a, b) = mul_buffered(a, b) ? " << (equal_2 ? "yes" : "no") << std::endl;
    std::cout << "mul_classic(a, b) = mul_blocked(a, b) ? " << (equal_3 ? "yes" : "no") << std::endl;
    std::cout << std::endl;

    std::cout << "classic:" << std::endl;
    std::cout << "time = " << time_classic.count() << " sec" << std::endl;
    std::cout << "performance = " << gflops_classic << " GFLOP/s" << std::endl;
    std::cout << std::endl;

    std::cout << "transpose:" << std::endl;
    std::cout << "time = " << time_transpose.count() << " sec" << std::endl;
    std::cout << "performance = " << gflops_transpose << " GFLOP/s" << std::endl;
    std::cout << std::endl;

    std::cout << "buffered:" << std::endl;
    std::cout << "time = " << time_buffered.count() << " sec" << std::endl;
    std::cout << "performance = " << gflops_buffered << " GFLOP/s" << std::endl;
    std::cout << std::endl;

    std::cout << "blocked:" << std::endl;
    std::cout << "time = " << time_blocked.count() << " sec" << std::endl;
    std::cout << "performance = " << gflops_blocked << " GFLOP/s" << std::endl;

    std::cout << std::endl;
    std::cout << "blocked experiment (varying S):" << std::endl;

    for (int S = 8; S <= 128; S *= 2) {
        auto start = std::chrono::high_resolution_clock::now();
        matrix c = mul_blocked(a, b, S, 1);
        auto end = std::chrono::high_resolution_clock::now();

        std::chrono::duration<double> time = end - start;
        double gflops = calc_gflops(n, time.count());

        std::cout << "S = " << std::setw(3) << S << " | time = " << std::setw(8) << time.count() << " sec | perf = " << gflops << " GFLOP/s" << std::endl;
    }
    return 0;

    //Сравнение Debug / Release
    /*
    int n = 512;

    matrix a = create_matrix(n);
    matrix b = create_matrix(n);
    fill_matrix_random(a);
    fill_matrix_random(b);

    auto start = std::chrono::high_resolution_clock::now();
    matrix c = mul_classic(a, b);
    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> t = end - start;
    double gflops = calc_gflops(n, t.count());

    std::cout << std::fixed << std::setprecision(6);
    std::cout << "N = " << n << std::endl;
    std::cout << "Time = " << t.count() << " sec" << std::endl;
    std::cout << "Performance = " << gflops << " GFLOP/s" << std::endl;

    return 0;
    */

}
