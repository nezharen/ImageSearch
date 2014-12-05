#ifndef RTREEAPPTEST_H
#define RTREEAPPTEST_H

#include "RTreeApplication.h"
#include <cstring>
#include <iostream>
#include <fstream>
#include "ColorCorrelogram.h"
#include "ColorHistogram.h"
#include "ColorMoment.h"
#include <QString>
#include <QStringList>
using namespace std;

#define RTREEAPPTEST_TEMPLATE template<int NUMDIMS>
#define RTREEAPPTEST_QUAL RTreeAppTest<NUMDIMS>

#define LONGESTPATH 256

template<int NUMDIMS>
class RTreeAppTest
{
public:
	RTreeApplication<NUMDIMS> m_app;
    int m_feature;
	char m_featureList[LONGESTPATH];
	char m_imageList[LONGESTPATH];
	char m_resultLog[LONGESTPATH];

	RTreeAppTest();
	~RTreeAppTest();

    bool mb_createRTree(int a_imageNum, int a_feature);
    double mb_singleQuery(QString q_queryImage, QStringList& q_queryResult, int& a_searchCount, int k = 20);
    double mb_batchQuery(const int a_batch, QString a_resultLog, double& a_searchCount, int k = 20);

    void mb_extractFeature(char* a_queryImage, double* a_query);
    double mb_singleQuery(char* a_queryImage, int& a_searchCount, int k = 20);
    double mb_batchQuery(const int a_batch, char* a_resultLog, double& a_searchCount, int k = 20);
};


RTREEAPPTEST_TEMPLATE
	RTREEAPPTEST_QUAL::RTreeAppTest()
{
    m_feature = -1;
	memset(m_featureList, 0, sizeof(char) * LONGESTPATH);
	memset(m_imageList, 0, sizeof(char) * LONGESTPATH);
	memset(m_resultLog, 0, sizeof(char) * LONGESTPATH);
}


RTREEAPPTEST_TEMPLATE
	RTREEAPPTEST_QUAL::~RTreeAppTest()
{

}


RTREEAPPTEST_TEMPLATE
    bool RTREEAPPTEST_QUAL::mb_createRTree(int a_imageNum, int a_feature)
{
    m_feature = a_feature;

    // FeatureList File Name: featureList_[imageNumber]_[featureID]_[dimensions]
    sprintf(m_featureList, "./list/featureList_%d_%d_%d.txt", a_imageNum, a_feature, NUMDIMS);
    // ImageList File Name: imageList_[imageNumber]
    sprintf(m_imageList, "./list/imageList_%d.txt", a_imageNum);
    m_app.mb_setDataFile(m_featureList, m_imageList);
    return m_app.mb_createRTree();
}


RTREEAPPTEST_TEMPLATE
    double RTREEAPPTEST_QUAL::mb_singleQuery(QString q_queryImage, QStringList& q_queryResult, int& a_searchCount, int k)
{
    char *ch;
    QByteArray ba = q_queryImage.toLocal8Bit();
    ch = ba.data();
    double r = mb_singleQuery(ch, a_searchCount, k);

    q_queryResult.clear();
    for (int i = 0; i < k; ++i)
        q_queryResult.append(QString(m_app.m_resultList[i]));

    return r;
}


RTREEAPPTEST_TEMPLATE
    void RTREEAPPTEST_QUAL::mb_extractFeature(char* a_queryImage, double* a_query)
{
    switch (m_feature)
    {
    case 0:
        ColorMoment(a_query, QString(a_queryImage));
        break;

    case 1:
        int d1, d2, d3;
        switch (NUMDIMS)
        {
        case 12:
            d1 = 3; d2 = 2; d3 = 2;
            break;

        case 20:
            d1 = 5; d2 = 2; d3 = 2;
            break;

        case 32:
            d1 = 8; d2 = 2; d3 = 2;
            break;
        }

        ColorHistogram(a_query, d1, d2, d3, QString(a_queryImage));
        break;

    case 2:
        ColorCorrelogram(a_query, NUMDIMS, 10, QString(a_queryImage));
        break;
    }
}


RTREEAPPTEST_TEMPLATE
    double RTREEAPPTEST_QUAL::mb_singleQuery(char* a_queryImage, int& a_searchCount, int k)
{
    while (k > m_app.m_total)   k /= 2;
    char b_queryImage[LONGESTPATH];
    int p = strlen(a_queryImage);
    while ((a_queryImage[p] != '/') && (a_queryImage[p] != '\\'))
        p--;
    p++;
    for (int i = 0; ; i++)
    {
        b_queryImage[i] = a_queryImage[p + i];
        if (b_queryImage[i] == '\0')
            break;
    }
    char c_queryImage[LONGESTPATH];
    strcpy(c_queryImage, "./image/");
    strcat(c_queryImage, b_queryImage);

    double* a_query = new double[NUMDIMS];
    mb_extractFeature(c_queryImage, a_query);
    
    a_searchCount = 0;
    m_app.mb_searchQuery(a_query, k, a_searchCount);
    int correct = 0;
    for (int j = 0; j < k; ++j)
    {
        int l;
        for (l = 0; l < 9; ++l)
            if (m_app.m_resultList[j][l] != b_queryImage[l])	break;
        if (l == 9)	correct++;
    }
    delete []a_query;

    return (double)correct / (double)k;
}


RTREEAPPTEST_TEMPLATE
    double RTREEAPPTEST_QUAL::mb_batchQuery(const int a_batch, QString q_resultLog, double& a_searchCount, int k)
{
    string tmp = q_resultLog.toStdString();
    return mb_batchQuery(a_batch, a_searchCount, tmp.c_str(), k);
}


RTREEAPPTEST_TEMPLATE
    double RTREEAPPTEST_QUAL::mb_batchQuery(const int a_batch, char* a_resultLog, double& a_searchCount, int k)
{
	strcpy(m_resultLog, a_resultLog);

	char* buf = new char[FILESIZE];
	char* buf_ptr = buf;
	RTFileStream stream;
	if (!stream.OpenRead(m_featureList))
        return -1;
	stream.ReadArray(buf, FILESIZE);
	stream.Close();

	while (k > m_app.m_total)
        k /= 2;

	double query[NUMDIMS];
	int correct, totcorrect = 0, searchCount = 0, totsearchCount = 0;
	ofstream outFile(m_resultLog, ios::out);

    int frequency = m_app.m_total / a_batch;
    for (int i = 0; i < frequency * a_batch; ++i)
	{
		for (int j = 0; j < NUMDIMS; ++j)
			query[j] = strtod(buf_ptr, &buf_ptr);

        if (i % frequency != 0) continue;

		m_app.mb_searchQuery(query, k, searchCount);
		correct = 1;
		for (int j = 1; j < k; ++j)
		{
			int l;
			for (l = 0; l < 9; ++l)
				if (m_app.m_resultList[j][l] != m_app.m_resultList[0][l])	break;
			if (l == 9)	correct++;
		}
		totcorrect += correct;
		totsearchCount += searchCount;

		outFile << m_app.m_nameList[i];
		for (int p = 0; p < 25 - strlen(m_app.m_nameList[i]); ++p)
			outFile << ' ';
        outFile << (double)correct * 100.0 / (double)k << "%(" << searchCount << ')' << endl;
	}
    double r = (double)totcorrect / (double)(a_batch) / (double)k;
    a_searchCount = (double)totsearchCount / (double)(a_batch);

    outFile << "Total Correctness:       " << r * 100.0 << "%(" <<  a_searchCount << ')' << endl;

	delete []buf;
	outFile.close();

    return r;
}


#undef RTREEAPPTEST_TEMPLATE
#undef RTREEAPPTEST_QUAL

#endif
