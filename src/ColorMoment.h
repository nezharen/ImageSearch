#ifndef COLORMOMENT_H
#define COLORMOMENT_H

#include <QString>
#include <QColor>
#include <QImage>

// 9 Dimension of color moment features using HSV model
// parameters:
// result - the features of the image
// filename - the name of target image
void ColorMoment(double* result, QString filename);

// Similarity of two features
// parameters:
// m1, m2 - 2 features
// return value: the similarity of two features. Smaller value indicates better similarity.
double Similarity_Moment(double* m1, double* m2);

#endif
