#ifndef COLORHISTOGRAM_H
#define COLORHISTOGRAM_H

#include <QString>
#include <QColor>
#include <QImage>

// Cumulative Color Histogram using HSV model
// parameters:
// result - the features of the image
// d1 - number of bins of Hue
// d2 - number of bins of Saturation
// d3 - number of bins of value
// filename - the name of target image
void ColorHistogram(double* result, int d1, int d2, int d3, QString filename);

// Similarity of two features
// parameters:
// h1, h2 - 2 features
// NumDim - number of dimensions
// return value: the similarity of two features. Smaller value indicates better similarity.
int Similarity_Histogram(int* h1, int* h2, int NumDim);

#endif
