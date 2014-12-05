#include <cmath>
#include "ColorCorrelogram.h"

void ColorCorrelogram(double* result, int NumDim, int dist, QString filename)
{
    int i, j;
    QImage image;
    QRgb rgb;
    int* sum = new int [NumDim];
    int gray1, gray2;

    for (i = 0; i < NumDim; i++)
    {
        result[i] = 0;
        sum[i] = 0;
    }

    image.load(filename);
    for (i = 0; i < image.width(); i++)
        for (j = 0; j < image.height(); j++)
        {
            rgb = image.pixel(i, j);
            gray1 = qGray(qRed(rgb), qGreen(rgb), qBlue(rgb));
            for (int k = 0; k <= dist; k++)
            {
                if (((i - k) >= 0) && ((i - k) < image.width()))
                {
                    if (((j + k - dist) >= 0) && (j + k - dist) < image.height())
                    {
                        sum[gray1 * NumDim / 256]++;
                        rgb = image.pixel(i - k, j + k - dist);
                        gray2 = qGray(qRed(rgb), qGreen(rgb), qBlue(rgb));
                        if (gray1 == gray2)
                            result[gray1 * NumDim / 256] += 1;
                    }
                    if (((j + dist - k) >= 0) && (j + dist - k) < image.height())
                    {
                        sum[gray1 * NumDim / 256]++;
                        rgb = image.pixel(i - k, j + dist - k);
                        gray2 = qGray(qRed(rgb), qGreen(rgb), qBlue(rgb));
                        if (gray1 == gray2)
                            result[gray1 * NumDim / 256] += 1;
                    }
                }
                if (k == 0) continue;
                if (((i + k) >= 0) && ((i + k) < image.width()))
                {
                    if (((j + k - dist) >= 0) && (j + k - dist) < image.height())
                    {
                        sum[gray1 * NumDim / 256]++;
                        rgb = image.pixel(i + k, j + k - dist);
                        gray2 = qGray(qRed(rgb), qGreen(rgb), qBlue(rgb));
                        if (gray1 == gray2)
                            result[gray1 * NumDim / 256] += 1;
                    }
                    if (((j + dist - k) >= 0) && (j + dist - k) < image.height())
                    {
                        sum[gray1 * NumDim / 256]++;
                        rgb = image.pixel(i + k, j + dist - k);
                        gray2 = qGray(qRed(rgb), qGreen(rgb), qBlue(rgb));
                        if (gray1 == gray2)
                            result[gray1 * NumDim / 256] += 1;
                    }
                }
            }
        }
    for (i = 0; i < NumDim; i++)
        if (sum[i] > 0)
            result[i] = result[i] * 10000 / sum[i];
    delete [] sum;
}

double Similarity_Correlogram(double* h1, double* h2, int NumDim)
{
    double result = 0;
    for (int i = 0; i < NumDim; i++)
        if (result < fabs(h1[i] - h2[i]))
            result = fabs(h1[i] - h2[i]);
    return result;
}
