#include "gnuplot.h"
#include <iostream>
#include <cstdio>
#include <cstdlib>

Gnuplot::Gnuplot(const Matrix &data) : data(data), size(data[0].size()) {

}

void Gnuplot::PlotToFile(const std::string &filename) const {
    FILE *CAARplot = fopen("plot_commands.txt", "w");
    if (!CAARplot) {
        std::cerr << "Could not open file to write plot commands.\n";
        return;
    }

    // Lambda function to write a group of data
    auto writeGroup = [CAARplot](const std::vector<double> &values, int size) {
        for (int i = -size / 2; i < size / 2; ++i) {
            fprintf(CAARplot, "%d %lf\n", i-1, values[i + size / 2]);
        }
        fprintf(CAARplot, "e\n");
    };
    

    // Write the commands to a text file
    fprintf(CAARplot, "set terminal png size 1024,768\n");
    fprintf(CAARplot, "set output '%s'\n", filename.c_str());
    fprintf(CAARplot, "set title 'Avg CAAR for 3 Groups'\n");
    fprintf(CAARplot, "set xlabel 'Announcement Date'\n");
    fprintf(CAARplot, "set ylabel 'Avg CAAR(%%)'\n");
    fprintf(CAARplot, "set xrange [%d:%d]\n", -(size) / 2, (size) / 2);
    fprintf(CAARplot, "set grid\n");  // Add grid background
    fprintf(CAARplot, "set xtics 20\n");  // Set x-axis tick interval
    fprintf(CAARplot, "set arrow from 0,graph(0,0) to 0,graph(1,1) nohead lc rgb 'red' lw 2\n");  // Thicker red line
    fprintf(CAARplot, "plot '-' with lines lw 2 title 'Miss', '-' with lines lw 2 title 'Meet', '-' with lines lw 2 title 'Beat'\n");

    // Write the data
    writeGroup(data[0], size);  // Miss CAAR
    writeGroup(data[1], size);  // Meet CAAR
    writeGroup(data[2], size);  // Beat CAAR

    fclose(CAARplot);
    // Run Gnuplot using the created command file
    std::system("gnuplot plot_commands.txt");
}

// output to screen
// void Gnuplot::PlotToScreen() const {
//     FILE *gnuplotPipe = popen("gnuplot -persist", "w");
//     if (!gnuplotPipe) {
//         std::cerr << "Error opening Gnuplot pipe." << std::endl;
//         return;
//     }

//     // Lambda function to write a group of data
//     auto writeGroup = [gnuplotPipe](const std::vector<double> &values, int size) {
//         for (int i = -size / 2; i < size/2 ; i++) {
//             fprintf(gnuplotPipe, "%d %lf\n", i, values[i + size / 2]);
//         }
//         fprintf(gnuplotPipe, "e\n");
//     };

//     // Write the commands directly to the pipe
//     fprintf(gnuplotPipe, "set terminal x11\n");  // Use X11 terminal for displaying on screen
//     fprintf(gnuplotPipe, "set title 'Avg CAAR for 3 Groups'\n");
//     fprintf(gnuplotPipe, "set xlabel 'Announcement Date'\n");
//     fprintf(gnuplotPipe, "set ylabel 'Avg CAAR(%%)'\n");
//     fprintf(gnuplotPipe, "set grid\n");  // Add grid background
//     fprintf(gnuplotPipe, "set xtics 20\n");  // Set x-axis tick interval
//     fprintf(gnuplotPipe, "set arrow from 0,graph(0,0) to 0,graph(1,1) nohead lc rgb 'red' lw 2\n");  // Thicker red line

//     // Set output to screen
//     fprintf(gnuplotPipe, "plot '-' with lines lw 2 title 'Beat', '-' with lines lw 2 title 'Meet', '-' with lines lw 2 title 'Miss'\n");

//     // Write the data
//     writeGroup(data[0], size);  // Beat CAAR
//     writeGroup(data[1], size);  // Meet CAAR
//     writeGroup(data[2], size);  // Miss CAAR

//     fprintf(gnuplotPipe, "e\n");  // End of data

//     // Close the pipe
//     pclose(gnuplotPipe);
// }


