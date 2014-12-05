#include <cmath>
#include "ColorMoment.h"

void ColorMoment(double* result, QString filename)
{
    int i, j, height, width;
    for (i = 0; i < 9; i++)
        result[i] = 0.0;
    QImage image;
    QRgb rgb;
    QColor color;
    image.load(filename);
    height = image.height();
    width = image.width();
    for (i = 0; i < width; i++)
        for (j = 0; j < height; j++)
        {
            rgb = image.pixel(i, j);
            color = QColor::fromRgba(rgb);
            result[0] += (double)color.hsvHue() / height / width;
            result[3] += (double)color.hsvSaturation() / height / width;
            result[6] += (double)color.value() / height / width;
        }
    for (i = 0; i < image.width(); i++)
        for (j = 0; j < image.height(); j++)
        {
            rgb = image.pixel(i, j);
            color = QColor::fromRgba(rgb);
            result[1] += pow(color.hsvHue() - result[0], 2) / height / width;
            result[4] += pow(color.hsvSaturation() - result[3], 2) / height / width;
            result[7] += pow(color.value() - result[6], 2) / height / width;
            result[2] += pow(color.hsvHue() - result[0], 3) / height / width;
            result[5] += pow(color.hsvSaturation() - result[3], 3) / height / width;
            result[8] += pow(color.value() - result[6], 3) / height / width;
        }
    result[1] = sqrt(result[1]);
    result[2] = cbrt(result[2]);
    result[4] = sqrt(result[4]);
    result[5] = cbrt(result[5]);
    result[7] = sqrt(result[7]);
    result[8] = cbrt(result[8]);
}

double Similarity_Moment(double* m1, double* m2)
{
    double sum = 0;
    for (int i = 0; i < 9; i += 3)
        sum += abs(m1[i] - m2[i]);
    for (int i = 1; i < 9; i += 3)
        sum += 2 * abs(m1[i] - m2[i]);
    for (int i = 2; i < 9; i += 3)
        sum += abs(m1[i] - m2[i]);
    return sum;
}

