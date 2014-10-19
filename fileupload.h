#ifndef FILEUPLOAD_H
#define FILEUPLOAD_H

#include <QObject>

class FileUploadPrivate;
class QNetworkReply;

class FileUpload : public QObject
{
    Q_OBJECT
public:
    explicit FileUpload(const QString &filename, QObject *parent = 0);

    void setAccessToken(const QString &accessToken);
    QString accessToken() const;

signals:
    void done();
    void error(const QString &error);

public slots:
    void start();

private slots:
    void uploadPolicyReceived();
    void imageUploaded();
    void uploadNotifySent();

private:
    void onError(QNetworkReply *reply);

    FileUploadPrivate *d;
};

#endif // FILEUPLOAD_H
