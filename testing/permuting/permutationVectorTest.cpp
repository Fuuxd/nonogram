#include "permutationVector.h"

int main() {
    unsigned int base = 10;  // Replace with corresponding base
    unsigned int digits = 1; // Replace with corresponding # of digits

    for(permutationVector pv(base, digits); !pv.allPermutationsIterated; pv++){
        pv.print();
        std::cout << pv.getSum() << std::endl << std::endl;
    }
    return 0;
}