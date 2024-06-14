#ifndef CALCULATIONS_H
#define CALCULATIONS_H

#include <vector>
#include <iostream>
#include <numeric>
#include <cmath>
#include <algorithm>
#include "Overload.h"

using namespace std;

void calculateLogReturns(vector<double> prices, vector<double>& logReturns);
dVector calculateCAARt(const dVector &lhs);
double calculateMean(const vector<double>& vec);
double calculateStdDev(const vector<double>& vec);
vector<double> calculateMean(const vector<vector<double>>& matrix);
vector<double> calculateStdDev(const vector<vector<double>>& matrix);
void displayVector(const dVector vec);


#endif // CALCULATIONS_H
