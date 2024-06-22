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


// This approach generates a random boolean value for each matrix cell
std::vector<std::vector<bool>> generateRandomNonogramMatrix1By1(int rows, int cols) {
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine generator(seed);
    std::bernoulli_distribution distribution(0.5);  

    unsigned long long int count{};

    // Create the matrix from the binary representation
    std::vector<std::vector<bool>> matrix(rows, std::vector<bool>(cols, false));
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            matrix[i][j] = distribution(generator);
            if(matrix[i][j]) {count++;}
        }
    }

    double printhis = static_cast<double>(count)/(rows*cols);
    std::cout << "Bernoulli Approach, Percent of 1:" << printhis << std::endl;

    return matrix;
}

std::vector<std::vector<bool>> generateRandomNonogramMatrix32each(int rows, int cols) {
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::mt19937 rng(seed);
    std::uint32_t data;

    unsigned long long int count{};

    // Create the matrix from the binary representation
    std::vector<std::vector<bool>> matrix(rows, std::vector<bool>(cols, false));
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if( !((j + (i*j)) % 32)){data = rng();}
            matrix[i][j] = data & 1;
            data >>= 1;
            if(matrix[i][j]) count++;
        }
    }

    double printhis = static_cast<double>(count)/(rows*cols);
    std::cout << "32bitmt19 method, Percent of 1:" << printhis << std::endl;

    return matrix;
}

// THis approach generates random 64 bit integers to populate the matrix
std::vector<std::vector<bool>> generateRandomNonogramMatrix64each(int rows, int cols) {
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::mt19937_64 rng(seed);
    std::uint64_t data;

    unsigned long long int count{};

    // Create the matrix from the binary representation
    std::vector<std::vector<bool>> matrix(rows, std::vector<bool>(cols, false));
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if( !((j + (i*j)) % 64)){data = rng();}
            matrix[i][j] = data & 1;
            data >>= 1;

            if(matrix[i][j]){ count++;}
        }
    }

    double printhis = static_cast<double>(count)/(rows*cols);
    std::cout << "64bitmt19 method, Percent of 1:" << printhis << std::endl;

    return matrix;
}

// This approach generates random 64 bit integers to populate the matrix
std::vector<std::vector<bool>> generateRandomNonogramMatrix64eachNoModulo(int rows, int cols) {
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::mt19937_64 rng(seed);
    std::uint64_t data;
    
    unsigned long long int count{};

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

            if(matrix[i][j]){count++;}
        }
    }

    double printhis = static_cast<double>(count)/(rows*cols);
    std::cout << "64bitmt19 No Modulo method, Percent of 1:" << printhis << std::endl;

    return matrix;
}

#define ROWNUM 10000
#define COLNUM 10000

int main() {

    generateRandomNonogramMatrix1By1(ROWNUM, COLNUM);

    generateRandomNonogramMatrix32each(ROWNUM, COLNUM);

    generateRandomNonogramMatrix64each(ROWNUM, COLNUM);

    generateRandomNonogramMatrix64eachNoModulo(ROWNUM, COLNUM);
    
    return 0;
}