#include "Overload.h"
#include <iostream>
using namespace std;

dVector operator-(const dVector &lhs, const dVector &rhs) {
    // Ensure both vectors are of the same length
    if (lhs.size() != rhs.size()) {
        throw std::invalid_argument("Vectors must be of the same length to add.");
    }

    // Initialize the result vector
    dVector result(lhs.size());
    
    // Sum corresponding elements
    for (size_t i = 0; i < lhs.size(); i++) {
        result[i] = lhs[i] - rhs[i];
    }

    return result;
}

dVector operator+(const dVector &lhs, const dVector &rhs) {
    
	double size = min(lhs.size(), rhs.size());
    // Initialize the result vector
    dVector result(size);
    
    // Sum corresponding elements
    for (size_t i = 0; i < size; i++) {
        result[i] = lhs[i] + rhs[i];
    }

    return result;
}


dVector operator/(const dVector &lhs, const double &num) {
	
	// Initialize the result vector
    dVector result(lhs.size());
	for (size_t i = 0; i < lhs.size(); i++){
		result[i] = lhs[i] / num;
	}
	
	return result;
}



