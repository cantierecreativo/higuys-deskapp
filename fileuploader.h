#ifndef FILEUPLOADER_H
#define FILEUPLOADER_H

#include <QObject>

class FileUploaderPrivate;

class FileUploader : public QObject
{
    Q_OBJECT
public:
    explicit FileUploader(const QString &accessToken, QObject *parent = 0);

signals:

public slots:
    void uploadFile(const QString &filename);

private:
    FileUploaderPrivate *d;
};

#endif // FILEUPLOADER_H
