#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QDesktopServices>
#include <QMessageBox>
#include <QDesktopWidget>
#include <QDebug>

#define MAXSIDE 500

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{   
    ui->setupUi(this);

    ui->labelImage->setMaximumSize(QSize(MAXSIDE,MAXSIDE));
    ui->labelMedianFilterResult->setMaximumSize(QSize(MAXSIDE,MAXSIDE));
    connect(ui->btnOpenDialog, SIGNAL(clicked(bool)), this, SLOT(openDialog()));
    connect(ui->btnApplyMedianFilter, SIGNAL(clicked(bool)), this, SLOT(applyMedianFilter()));
}

MainWindow::~MainWindow()
{    
    delete ui;
}

void MainWindow::openDialog()
{
    QFileDialog dialog(this);
    dialog.setNameFilter(tr("Images (*.png *.xpm *.jpg *.bmp *.jpeg *.xbm *.ppm)"));
    dialog.setViewMode(QFileDialog::Detail);
    QString fileName = QFileDialog::getOpenFileName(this,tr("Open Images"),
                                                    QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation), tr("Image Files (*.png *.jpg *.bmp)"));
    if (!fileName.isEmpty())
    {
        QImage image(fileName);
        this->image = image;
        ui->labelImage->setPixmap(QPixmap::fromImage(image).scaled(MAXSIDE, MAXSIDE, Qt::KeepAspectRatio));
    }
}

void MainWindow::applyMedianFilter()
{    
    int filterRadius = ui->lineEdit->text().toInt();
    if (filterRadius > 30 || filterRadius <= 0)
    {
        QMessageBox::warning(this, tr("Incorrect radius"), tr("Please choose radius between 1 and 30"), QMessageBox::Ok);
        return;
    }

    if(!ui->labelImage->pixmap())
    {
        QMessageBox::warning(this, tr("No image"), tr("Please select image."), QMessageBox::Ok);
        return;
    }

    int imageHeight = image.height(), imageWidth = image.width();

    if (filterRadius > imageHeight / 2 || filterRadius > imageWidth / 2)
    {
        QMessageBox::warning(this, tr("Incorrect radius"), tr("The chosen filter radius is too big for this image!"), QMessageBox::Ok);
        return;
    }

    MedianFilter medianFilter;
	int* resImageBits;
    resImageBits = new int[imageHeight * imageWidth];
    if (!resImageBits)
        return;

    medianFilter.applyMedianFilter((int*)image.bits(), resImageBits, imageHeight, imageWidth, filterRadius);
    if(resImageBits)
    {
        QImage destImage((uchar*)resImageBits, imageWidth, imageHeight, image.format());
        QPixmap pixRes;
        pixRes.convertFromImage(destImage);
        ui->labelMedianFilterResult->setPixmap(pixRes.scaled(MAXSIDE, MAXSIDE, Qt::KeepAspectRatio));
    }
    else
        qDebug()<<"median filter failed";
    delete [] resImageBits;
}

