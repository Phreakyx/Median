#include "medianfilter.h"
#include <stdio.h>
#include <QDebug>

MedianFilter* MedianFilter::mfStatic = NULL;

MedianFilter::MedianFilter(QObject *parent) : QObject(parent)
{
    mfStatic = this;
    image = NULL;
    extensionImage = NULL;
    resImage = NULL;
    futureWatcher = new QFutureWatcher<void>(this);
    progressDialog = new QProgressDialog(NULL);
    connect(futureWatcher, SIGNAL(progressValueChanged(int)), progressDialog, SLOT(setValue(int)));
    connect(futureWatcher, SIGNAL(progressRangeChanged(int,int)), progressDialog, SLOT(setRange(int,int)));
    connect(progressDialog, SIGNAL(canceled()), this, SLOT(cancelMedianFilter()));
}

MedianFilter::~MedianFilter()
{
    if (futureWatcher->isRunning())
    {
        futureWatcher->cancel();
        futureWatcher->waitForFinished();
    }

    if(extensionImage)
        delete [] extensionImage;
}

void MedianFilter::cancelMedianFilter()
{
    if (futureWatcher->isRunning())
    {
        futureWatcher->cancel();
        futureWatcher->waitForFinished();
    }
}

void getResPixelValueST(IndexPair &p)
{
    MedianFilter::mfStatic->getResPixelValue(p);
}

void MedianFilter::getResPixelValue(IndexPair &p)
{
    int k = 0, workingWidth = originImagewidth +2*radius;
    int* window = new int[windowSize];

    for (int i = p.i - radius; i < p.i + radius+1; i++)
    {
        for (int j = p.j - radius; j < p.j + radius + 1; j++)
            window[k++] = this->extensionImage[i * (workingWidth) + j];
    }

    for (int j = 0; j <= windowSizeHalf; j++)
    {
        int min = j;
        for (int l = j + 1; l < windowSize; l++)
            if (window[l] < window[min])
                min = l;
        const int el = window[j];
        window[j] = window[min];
        window[min] = el;
    }

    resImage[(p.i - radius) * originImagewidth + (p.j - radius)] = window[windowSizeHalf];
	delete[] window;
}

void MedianFilter::applyMedianFilter(int *i, int *res, const int imageHeight, const int imageWidth, const int r)
{ 
    if (!i || imageHeight < 1 || imageWidth < 1)
        return;

    if (futureWatcher->isRunning())
    {
        futureWatcher->cancel();
        futureWatcher->waitForFinished();
    }

    this->originImagewidth = imageWidth;
    this->radius = r;
    image = i;

    int extWidth = imageWidth + 2 * radius, extHeight = imageHeight + 2 * radius;
    this->windowSize = (2 * radius + 1) * (2 * radius + 1);
    this->windowSizeHalf = windowSize / 2;

    extensionImage = new int[extWidth * extHeight];
    if (!extensionImage)
        return;
    resImage = res;

    for (int i = 0; i < imageHeight; i++)
    {
        memcpy(extensionImage + extWidth * (i + radius) + radius, image + imageWidth * i, imageWidth * sizeof(int));
        memcpy(extensionImage + extWidth * (i + radius), image + (imageWidth * i), radius * sizeof(int));
        memcpy(extensionImage + (extWidth * (i + radius + 1) - radius), image + imageWidth * (i + 1) - radius, radius * sizeof(int));
    }

    for(int i = 0; i < radius; i++)
    {
        memcpy(extensionImage + i * extWidth, extensionImage + extWidth * (i + imageHeight - 1), extWidth * sizeof(int));
        memcpy(extensionImage + extWidth * (imageHeight + radius + i), extensionImage + extWidth * (imageHeight + radius - i - 1), extWidth * sizeof(int));
    }

    QList<IndexPair> indexes;
    for (int i = radius; i < imageHeight + radius; i++)
    {   for (int j = radius; j < imageWidth + radius; j++)
        {
            IndexPair p(i,j);
            indexes<<p;
        }
    }

    futureWatcher->setFuture(QtConcurrent::map(indexes, getResPixelValueST));
    progressDialog->exec();
    futureWatcher->waitForFinished();

}