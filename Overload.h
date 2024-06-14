#ifndef OVERLOAD_H
#define OVERLOAD_H

#include <vector>
#include <algorithm>
#include <stdexcept>
#include <cmath>
#include <numeric>

typedef std::vector<double> dVector;
typedef std::vector<std::vector<double>> Matrix;

dVector operator-(const dVector &lhs, const dVector &rhs);
dVector operator+(const dVector &lhs, const dVector &rhs);
dVector calculateCAARt(const dVector &lhs);
dVector operator/(const dVector &lhs, const double &num);
void displayVector(const dVector vec);
double calculateMean(const std::vector<double>&vec);
double calculateStdDev(const std::vector<double>& vec);
std::vector<double> calculateStdDev(const Matrix& matrix);
std::vector<double> calculateMean(const Matrix& matrix);

#endif // OVERLOAD_H
