#ifndef PERIODICCAMERACAPTURE_H
#define PERIODICCAMERACAPTURE_H

#include <QObject>

class PeriodicCameraCapturePrivate;

class PeriodicCameraCapture : public QObject
{
    Q_OBJECT
public:
    explicit PeriodicCameraCapture(QObject *parent = 0);

    void setTimeout(quint32 timeout);
    quint32 timeout();

signals:
    void newImageAvailable(const QString &filename);

public slots:
    void start();

private slots:
    void captureImage();
    void imageSaved(int id, const QString &filename);

private:
    PeriodicCameraCapturePrivate *d;
};

#endif // PERIODICCAMERACAPTURE_H
