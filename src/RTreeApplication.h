#ifndef RTREEAPPLICATION_H
#define RTREEAPPLICATION_H

#include "RTree.h"
#include <cstring>

#define RTREEAPPLICATION_TEMPLATE template<int NUMDIMS>
#define RTREEAPPLICATION_QUAL RTreeApplication<NUMDIMS>

#define PGSIZE 4096
#define FILESIZE 3072000

template<int NUMDIMS>
class RTreeApplication
{
public:

    RTree<char*, double, NUMDIMS, double, (PGSIZE - 8) / (8 + 16 * NUMDIMS), (PGSIZE - 8) / (8 + 16 * NUMDIMS) / 2> rtree;
	char* m_featureList;
	char* m_imageList;
	int m_total;
	char** m_nameList;
    int m_totalNodes;

	double m_query[NUMDIMS];

	int m_k;
	char** m_resultList;
	double* m_distList;

	RTreeApplication();
	~RTreeApplication();

	void mb_destroy();

	void mb_setDataFile(const char* a_featureList, const char* a_imageList);
	bool mb_createRTree();

	int mb_searchQuery(const double a_query[NUMDIMS], const int k, int& a_searchCount);
};

RTREEAPPLICATION_TEMPLATE
	RTREEAPPLICATION_QUAL::RTreeApplication()
{
	m_featureList = NULL;
	m_imageList = NULL;
	m_total = 0;
	m_nameList = NULL;
    m_totalNodes = 0;
    m_resultList = NULL;
    m_distList = NULL;
}


RTREEAPPLICATION_TEMPLATE
	RTREEAPPLICATION_QUAL::~RTreeApplication()
{
	mb_destroy();
}


RTREEAPPLICATION_TEMPLATE
	void RTREEAPPLICATION_QUAL::mb_destroy()
{
	rtree.RemoveAll();

	if (m_featureList)
	{
		delete []m_featureList;
		m_featureList = NULL;
	}
	if (m_imageList)
	{
		delete []m_imageList;
		m_imageList = NULL;
	}
	m_total = 0;
	if (m_nameList)
	{
		for (int i = 0; i < m_total; ++i)
			delete [](m_nameList[i]);
		delete []m_nameList;
		m_nameList = NULL;
	}
    m_totalNodes = 0;

	if (m_resultList)
	{
		delete []m_resultList;
		m_resultList = NULL;
	}
	if (m_distList)
	{
		delete []m_distList;
		m_distList = NULL;
	}
	m_k = 0;
}


RTREEAPPLICATION_TEMPLATE
	void RTREEAPPLICATION_QUAL::mb_setDataFile(const char* a_featureList, const char* a_imageList)
{
    mb_destroy();

	m_featureList = new char[strlen(a_featureList)+1];
	strcpy(m_featureList, a_featureList);
	m_featureList[strlen(a_featureList)] = 0;

	m_imageList = new char[strlen(a_imageList)+1];
	strcpy(m_imageList, a_imageList);
	m_imageList[strlen(a_imageList)] = 0;
}


RTREEAPPLICATION_TEMPLATE
	bool RTREEAPPLICATION_QUAL::mb_createRTree()
{
	rtree.RemoveAll();

	char* buf = new char[FILESIZE];
	memset(buf, 0, sizeof(char) * FILESIZE);
	char *buf_ptr = buf;

	RTFileStream stream;
	if (!stream.OpenRead(m_imageList))
		return false;
	stream.ReadArray(buf, FILESIZE);
	stream.Close();

	m_total = strtol(buf_ptr, &buf_ptr, 0);
    m_nameList = new char*[m_total];
	while (*buf_ptr == '\n' || *buf_ptr == '\r')
		buf_ptr++;

	int k;
	for (int i = 0; i < m_total; ++i)
	{
		k = 0;
		while (*(buf_ptr + k) != '\r' && *(buf_ptr + k) != '\n' && *(buf_ptr + k) != 0)
			k++;
		m_nameList[i] = new char[k + 1];
		m_nameList[i][k] = 0;
		for (int j = 0; j < k; ++j, ++buf_ptr)
			m_nameList[i][j] = *buf_ptr;
		while (i < m_total - 1 && (*buf_ptr == '\n' || *buf_ptr == '\r'))
			buf_ptr++;
	}

	buf_ptr = buf;
	if (!stream.OpenRead(m_featureList))
		return false;
	stream.ReadArray(buf, FILESIZE);
	stream.Close();

	double a_min[NUMDIMS], a_max[NUMDIMS];
	for (int i = 0; i < m_total; ++i)
	{
		for (int j = 0; j < NUMDIMS; ++j)
		{
			a_min[j] = strtod(buf_ptr, &buf_ptr);
			a_max[j] = a_min[j];
		}

		rtree.Insert(a_min, a_max, m_nameList[i]);
	}

    m_totalNodes = rtree.CountNodes();

	delete []buf;
	return true;
}


RTREEAPPLICATION_TEMPLATE
	int RTREEAPPLICATION_QUAL::mb_searchQuery(const double a_query[NUMDIMS], const int k, int& a_searchCount)
{
	for (int i = 0; i < NUMDIMS; ++i)
		m_query[i] = a_query[i];
	m_k = (m_total > k ? k : m_total);
	m_resultList = new char*[m_k];
	m_distList = new double[m_k];

	return rtree.KNearestNeighborsSearch(m_query, m_query, m_k, m_resultList, m_distList, a_searchCount);
}


#undef RTREEAPPLICATION_TEMPLATE
#undef RTREEAPPLICATION_QUAL

#endif
