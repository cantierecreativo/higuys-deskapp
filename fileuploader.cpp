#include "fileuploader.h"
#include "fileupload.h"

class FileUploaderPrivate
{
public:
    QString accessToken;
};


FileUploader::FileUploader(const QString &accessToken, QObject *parent) :
    QObject(parent), d(new FileUploaderPrivate)
{
    d->accessToken = accessToken;
}

void FileUploader::uploadFile(const QString &filename)
{
    FileUpload *upload = new FileUpload(filename, this);
    upload->setAccessToken(d->accessToken);
    connect(upload, SIGNAL(done()), upload, SLOT(deleteLater()));

    upload->start();
}
