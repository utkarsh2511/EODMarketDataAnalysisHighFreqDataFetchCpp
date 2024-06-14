#include <iostream>
#include <iomanip>
#include "Calculation.h"
using namespace std;

void calculateLogReturns(vector<double> prices, vector<double>& logReturns){
	int size = prices.size() - 1;

	for(int i = 0; i <size; i++){
		logReturns.push_back(log(prices[i+1]) - log(prices[i]));
	}
	
}


dVector calculateCAARt(const dVector &lhs){
	// Initialize the result vector
    dVector result(lhs.size());
    double sum = 0;
    // Sum corresponding elements
    for (size_t i = 0; i < lhs.size(); i++) {
        sum += lhs[i];
        result[i] = sum;
    }

    return result;
}


double calculateMean(const vector<double>&vec){
    // Calculate sum then divide by size for mean
    double mean = accumulate(vec.begin(),vec.end(),0.0) / vec.size();
    return mean;
} 

double calculateStdDev(const vector<double>& vec) {
    double mean = accumulate(vec.begin(), vec.end(), 0.0) / vec.size();
    // Calculate the squared differences from the mean
    double sq_sum = inner_product(vec.begin(), vec.end(), vec.begin(), 0.0, [&](double sum, double val) {
                                           double diff = val - mean;
                                           return sum + diff * diff;
                                            },
                                       [](double a, double b) { return a + b; });

    // Calculate the variance and then the standard deviation
    double variance = sq_sum / vec.size();
    double std_dev = sqrt(variance);

    return std_dev;
}


vector<double> calculateMean(const Matrix& matrix){
	size_t size = matrix.size();
	dVector vec = matrix[0];
	for(size_t i=1; i < size; i++){
		vec = vec + matrix[i];
	}
	vec = vec/size;
	return vec;
}

vector<double> calculateStdDev(const Matrix& matrix){
	vector<double> stdVec;
	size_t numDates;
	for(size_t k=0; k < matrix.size(); k++){
		if(matrix[k].size() < numDates){
			numDates = matrix[k].size();
		}
	}
	
	for(size_t j = 0; j < numDates; j++){
		vector<double> vec;
		for(size_t i =0; i < matrix.size(); i++){
			vec.push_back(matrix[i][j]);
		}
		stdVec.push_back(calculateStdDev(vec));
	}
		
	return stdVec;
}

void displayVector(const dVector vec){
	for(size_t i =0; i < vec.size(); i++){
		cout << setw(15) << vec[i] << " ";
		if ((i + 1) % 8 == 0) {
            cout << endl;
	}
}
	cout << endl;
}
