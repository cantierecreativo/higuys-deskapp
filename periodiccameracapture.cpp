#include "periodiccameracapture.h"
#include <QCamera>
#include <QCameraImageCapture>
#include <QTimer>
#include <QDir>

static const quint32 DEFAULT_TIMEOUT = 20000;

class PeriodicCameraCapturePrivate
{
public:
    PeriodicCameraCapturePrivate() : timeout(DEFAULT_TIMEOUT) {}

    quint32 timeout;
    QCamera *camera;
    QCameraImageCapture *imageCapture;
    QTimer *timer;

    void setupCamera();
};

void PeriodicCameraCapturePrivate::setupCamera()
{
    camera = new QCamera;
    imageCapture = new QCameraImageCapture(camera);
    imageCapture->setCaptureDestination(QCameraImageCapture::CaptureToFile);

    camera->setCaptureMode(QCamera::CaptureStillImage);
}


PeriodicCameraCapture::PeriodicCameraCapture(QObject *parent) :
    QObject(parent), d(new PeriodicCameraCapturePrivate)
{
    d->setupCamera();
    connect(d->imageCapture, SIGNAL(imageSaved(int,QString)), this, SLOT(imageSaved(int,QString)));

    d->timer = new QTimer(this);
    connect(d->timer, SIGNAL(timeout()), this, SLOT(captureImage()));
}

void PeriodicCameraCapture::setTimeout(quint32 timeout)
{
    d->timeout = timeout;
    d->timer->setInterval(timeout);
}

quint32 PeriodicCameraCapture::timeout()
{
    return d->timeout;
}

void PeriodicCameraCapture::start()
{
    d->camera->start();
    d->timer->start();
}

void PeriodicCameraCapture::imageSaved(int /* id */, const QString &filename)
{
    emit newImageAvailable(filename);
}

void PeriodicCameraCapture::captureImage()
{
    qDebug() << "Capturing Image";
    d->camera->searchAndLock();
    d->imageCapture->capture(QDir::temp().filePath("higuys.jpg"));
    d->camera->unlock();
    qDebug() << "Done";
}
