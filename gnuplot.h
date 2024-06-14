#ifndef GNUPLOT_H
#define GNUPLOT_H

#include <vector>
#include <string>

typedef std::vector<std::vector<double>> Matrix;

class Gnuplot {
public:
    Gnuplot(const Matrix &data);

    void PlotToFile(const std::string &filename) const;
    // void PlotToScreenAndFile(const std::string &filename) const;

private:
    const Matrix &data;
    int size;
};

#endif // GNUPLOT_H
