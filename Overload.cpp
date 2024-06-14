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

// dVector calculateCAARt(const dVector &lhs){
// 	// Initialize the result vector
//     dVector result(lhs.size());
//     double sum = 0;
//     // Sum corresponding elements
//     for (size_t i = 0; i < lhs.size(); i++) {
//         sum += lhs[i];
//         result[i] = sum;
//     }

//     return result;
// }

dVector operator/(const dVector &lhs, const double &num) {
	
	// Initialize the result vector
    dVector result(lhs.size());
	for (size_t i = 0; i < lhs.size(); i++){
		result[i] = lhs[i] / num;
	}
	
	return result;
}

// void displayVector(const dVector vec){
// 	for(size_t i =0; i < vec.size(); i++){
// 		cout << vec[i] << " ";
// 	}
// 	cout << endl;
// }

// double calculateMean(const vector<double>&vec){
//     // Calculate sum then divide by size for mean
//     double mean = accumulate(vec.begin(),vec.end(),0.0) / vec.size();
//     return mean;
// } 

// double calculateStdDev(const vector<double>& vec) {
//     double mean = accumulate(vec.begin(), vec.end(), 0.0) / vec.size();
//     // Calculate the squared differences from the mean
//     double sq_sum = inner_product(vec.begin(), vec.end(), vec.begin(), 0.0, [&](double sum, double val) {
//                                           double diff = val - mean;
//                                           return sum + diff * diff;
//                                             },
//                                       [](double a, double b) { return a + b; });

//     // Calculate the variance and then the standard deviation
//     double variance = sq_sum / vec.size();
//     double std_dev = sqrt(variance);

//     return std_dev;
// }

// vector<double> calculateStdDev(const Matrix& matrix){
// 	vector<double> stdVec;
// 	size_t numDates;
// 	for(size_t k=0; k < matrix.size(); k++){
// 		if(matrix[k].size() < numDates){
// 			numDates = matrix[k].size();
// 		}
// 	}
	
// 	for(size_t j = 0; j < numDates; j++){
// 		vector<double> vec;
// 		for(size_t i =0; i < matrix.size(); i++){
// 			vec.push_back(matrix[i][j]);
// 		}
// 		stdVec.push_back(calculateStdDev(vec));
// 	}
		
// 	return stdVec;
// }

// vector<double> calculateMean(const Matrix& matrix){
// 	size_t size = matrix.size();
// 	dVector vec = matrix[0];
// 	for(size_t i=1; i < size; i++){
// 		vec = vec + matrix[0];
// 	}
// 	vec = vec/size;
// 	return vec;
// }

