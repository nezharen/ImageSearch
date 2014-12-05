#include <cmath>
#include "ColorHistogram.h"

void ColorHistogram(double* result, int d1, int d2, int d3, QString filename)
{
    int i, j;
    QImage image;
    QRgb rgb;
    QColor color;
    for (i = 0; i < d1 * d2 * d3; i++)
        result[i] = 0;
    image.load(filename);
    for (i = 0; i < image.width(); i++)
        for (j = 0; j < image.height(); j++)
        {
            rgb = image.pixel(i, j);
            color = QColor::fromRgba(rgb);
            result[color.hsvHue() / (360 / d1) * d2 * d3 + color.hsvSaturation() / (256 / d2) * d3 + color.value() / (256 / d3)]++;
        }
    for (i = 1; i < d1 * d2 * d3; i++)
        result[i] += result[i - 1];
}

int Similarity_Histogram(int* h1, int* h2, int NumDim)
{
    int result = 0;
    for (int i = 0; i < NumDim; i++)
        if (result < abs(h1[i] - h2[i]))
            result = abs(h1[i] - h2[i]);
    return result;
}
