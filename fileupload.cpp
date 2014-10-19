#include "fileupload.h"

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>

class FileUploadPrivate
{
public:
    QString filename;
    QString uploadUrl;
    QByteArray accessToken;
    QNetworkAccessManager *manager;
};

FileUpload::FileUpload(const QString &filename, QObject *parent) :
    QObject(parent), d(new FileUploadPrivate)
{
    d->filename = filename;
    d->manager = new QNetworkAccessManager(this);
}

void FileUpload::setAccessToken(const QString &accessToken)
{
    d->accessToken = accessToken.toLocal8Bit();
}

QString FileUpload::accessToken() const
{
    return QString(d->accessToken);
}

void FileUpload::start()
{
    qDebug() << "Requesting upload url";

    QNetworkRequest uploadUrlRequest(QUrl("http://www.higuys.io/api/wall/upload-requests"));
    uploadUrlRequest.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    uploadUrlRequest.setRawHeader("accept", "application/json");
    uploadUrlRequest.setRawHeader("authorization", d->accessToken);

    QNetworkReply *uploadUrlReply = d->manager->post(uploadUrlRequest, QByteArray());
    connect(uploadUrlReply, SIGNAL(finished()), this, SLOT(uploadPolicyReceived()));
}

void FileUpload::uploadPolicyReceived()
{
    QNetworkReply *uploadUrlReply = qobject_cast<QNetworkReply *>(sender());
    if (uploadUrlReply->error() != QNetworkReply::NoError) {
        onError(uploadUrlReply);
        return;
    }

    QJsonDocument jsonDocument = QJsonDocument::fromJson(uploadUrlReply->readAll(), 0);
    uploadUrlReply->deleteLater();

    Q_ASSERT_X(jsonDocument.isObject(), "FileUpload::uploadPolicyReceived()", "Received json is not an object");

    QVariantMap body = jsonDocument.object().toVariantMap();
    d->uploadUrl = body.value("upload_url").toString();

    qDebug() << "Received upload url" << d->uploadUrl;

    QFile imageFile(d->filename);
    if (!imageFile.open(QIODevice::ReadOnly)) {
        qDebug() << "Error opening" << d->filename;
        return;
    }

    qDebug() << "Uploading image on s3";

    QNetworkRequest uploadImageRequest(QUrl(d->uploadUrl));
    uploadImageRequest.setHeader(QNetworkRequest::ContentTypeHeader, "image/jpeg");

    QNetworkReply *uploadImageReply = d->manager->put(uploadImageRequest, imageFile.readAll());
    connect(uploadImageReply, SIGNAL(finished()), this, SLOT(imageUploaded()));
}

void FileUpload::imageUploaded()
{
    QNetworkReply *uploadImageReply = qobject_cast<QNetworkReply *>(sender());
    if (uploadImageReply->error() != QNetworkReply::NoError) {
        onError(uploadImageReply);
        return;
    }
    qDebug() << "Image uploaded";
    uploadImageReply->deleteLater();

    QJsonObject body;
    body.insert("s3_url", QJsonValue(d->uploadUrl));

    QJsonDocument *jsonDocument = new QJsonDocument(body);

    qDebug() << "Sending upload notify";
    QNetworkRequest uploadedImageNotifyRequest(QUrl("http://www.higuys.io/api/wall/photos"));
    uploadedImageNotifyRequest.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    uploadedImageNotifyRequest.setRawHeader("accept", "application/json");
    uploadedImageNotifyRequest.setRawHeader("authorization", d->accessToken);

    QNetworkReply *uploadedImageNotifyReply = d->manager->post(uploadedImageNotifyRequest, jsonDocument->toJson());
    connect(uploadedImageNotifyReply, SIGNAL(finished()), this, SLOT(uploadNotifySent()));
}

void FileUpload::uploadNotifySent()
{
    QNetworkReply *uploadedImageNotifyReply = qobject_cast<QNetworkReply *>(sender());
    if (uploadedImageNotifyReply->error() != QNetworkReply::NoError) {
        onError(uploadedImageNotifyReply);
        return;
    }
    qDebug() << "Upload notify sent";

    uploadedImageNotifyReply->deleteLater();

    emit done();
}

void FileUpload::onError(QNetworkReply *reply)
{
    qDebug() << reply->errorString();
    emit error(reply->errorString());
    reply->deleteLater();
}
