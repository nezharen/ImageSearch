#ifndef COLORCORRELOGRAM_H
#define COLORCORRELOGRAM_H

#include <QString>
#include <QColor>
#include <QImage>

// Cumulative Color Correlogram using HSV model
// parameters:
// result - the features of the image
// NumDim - number of dimensions
// dist - the distance token in the function, suggesting 10
// filename - the name of target image
void ColorCorrelogram(double* result, int NumDim, int dist, QString filename);

// Similarity of two features
// parameters:
// h1, h2 - 2 features
// NumDim - number of dimensions
// return value: the similarity of two features. Smaller value indicates better similarity.
double Similarity_Correlogram(double* h1, double* h2, int NumDim);

#endif
