#include <random>
#include <vector>
#include <chrono>
#include <iostream>
#include <bitset>

#ifdef _WIN32
#include <intrin.h>
#else
#include <x86intrin.h>
#endif

std::vector<std::vector<bool>> generateRandomNonogramMatrix1By1(int rows, int cols) {
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine generator(seed);
    std::bernoulli_distribution distribution(0.5);  

    // Create the matrix from the binary representation
    std::vector<std::vector<bool>> matrix(rows, std::vector<bool>(cols, false));
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            matrix[i][j] = distribution(generator);
        }
    }

    return matrix;
}

// This approach generates random 32 bit integers to populate the matrix
std::vector<std::vector<bool>> generateRandomNonogramMatrix32each(int rows, int cols) {
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::mt19937 rng(seed);
    std::uint32_t data;

    // Create the matrix from the binary representation
    std::vector<std::vector<bool>> matrix(rows, std::vector<bool>(cols, false));
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if( !((j + (i*j)) % 32)){data = rng();}
            matrix[i][j] = data & 1;
            data >>= 1;
        }
    }

    return matrix;
}

// THis approach generates random 64 bit integers to populate the matrix
std::vector<std::vector<bool>> generateRandomNonogramMatrix64each(int rows, int cols) {
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::mt19937_64 rng(seed);
    std::uint64_t data;

    // Create the matrix from the binary representation
    std::vector<std::vector<bool>> matrix(rows, std::vector<bool>(cols, false));
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if( !((j + (i*j)) % 64)){data = rng();}
            matrix[i][j] = data & 1;
            data >>= 1;
        }
    }

    return matrix;
}

// This approach generates random 64 bit integers to populate the matrix
std::vector<std::vector<bool>> generateRandomNonogramMatrix64eachNoModulo(int rows, int cols) {
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::mt19937_64 rng(seed);
    std::uint64_t data;
    
    // Create the matrix from the binary representation
    unsigned int bitcount = 0;
    std::vector<std::vector<bool>> matrix(rows, std::vector<bool>(cols, false));
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if(!bitcount){ 
                data = rng();
                bitcount = 64;
            }
            matrix[i][j] = data & 1;
            data >>= 1;
            bitcount--;
        }
    }

    return matrix;
}

#define ROWNUM 10000
#define COLNUM 10000

int main() {
    // Measure elapsed time
    auto start_time = std::chrono::high_resolution_clock::now();
    unsigned long long start_cycles = __rdtsc();
    // timing actual function
    generateRandomNonogramMatrix1By1(ROWNUM, COLNUM);

    unsigned long long end_cycles = __rdtsc();
    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed_seconds = end_time - start_time;
    std::cout << "BERNOULLI ONE BY ONE\nElapsed time: " << elapsed_seconds.count() << " seconds" << std::endl;
    
    unsigned long long cycle_count = end_cycles - start_cycles;
    std::cout << "Clock cycles: " << cycle_count << std::endl;

    ///

    start_time = std::chrono::high_resolution_clock::now();
    start_cycles = __rdtsc();
    // timing actual function
    generateRandomNonogramMatrix32each(ROWNUM, COLNUM);

    end_cycles = __rdtsc();
    end_time = std::chrono::high_resolution_clock::now();
    elapsed_seconds = end_time - start_time;
    std::cout << "32BitEach\nElapsed time: " << elapsed_seconds.count() << " seconds" << std::endl;
    
    cycle_count = end_cycles - start_cycles;
    std::cout << "Clock cycles: " << cycle_count << std::endl;

    ///

    start_time = std::chrono::high_resolution_clock::now();
    start_cycles = __rdtsc();
    // timing actual function
    generateRandomNonogramMatrix64each(ROWNUM, COLNUM);

    end_cycles = __rdtsc();
    end_time = std::chrono::high_resolution_clock::now();
    elapsed_seconds = end_time - start_time;
    std::cout << "64BitEach\nElapsed time: " << elapsed_seconds.count() << " seconds" << std::endl;
    
    cycle_count = end_cycles - start_cycles;
    std::cout << "Clock cycles: " << cycle_count << std::endl;

    ///

    start_time = std::chrono::high_resolution_clock::now();
    start_cycles = __rdtsc();
    // timing actual function
    generateRandomNonogramMatrix64eachNoModulo(ROWNUM, COLNUM);

    end_cycles = __rdtsc();
    end_time = std::chrono::high_resolution_clock::now();
    elapsed_seconds = end_time - start_time;
    std::cout << "64BitEach No Modulo\nElapsed time: " << elapsed_seconds.count() << " seconds" << std::endl;
    
    cycle_count = end_cycles - start_cycles;
    std::cout << "Clock cycles: " << cycle_count << std::endl;

    
    return 0;
}