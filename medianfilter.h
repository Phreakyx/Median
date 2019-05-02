#ifndef MEDIANFILTER_H
#define MEDIANFILTER_H

#include <QObject>
#include <QtConcurrent>
#include <QProgressDialog>

struct IndexPair{
    int i, j;
    IndexPair(int a, int b)
    {
        i = a; j = b;
    }
};

class MedianFilter : public QObject
{
    Q_OBJECT
public:
    static MedianFilter *mfStatic;
    explicit MedianFilter(QObject *parent = 0);
    ~MedianFilter();
    void applyMedianFilter(int *i, int *res, const int imageHeight, const int imageWidth, const int r);
    void getResPixelValue(IndexPair &p);
public slots:
    void cancelMedianFilter();
private:
	int *image, *extensionImage, *resImage;
    int originImagewidth, radius, windowSize, windowSizeHalf;
    QFutureWatcher<void> *futureWatcher;
    QProgressDialog *progressDialog;    
};

#endif // MEDIANFILTER_H
