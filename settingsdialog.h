#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>

class SettingsDialogPrivate;


class SettingsDialog : public QDialog
{
    Q_OBJECT
public:
    explicit SettingsDialog(QWidget *parent = 0);

signals:

public slots:
    virtual void accept();

private slots:
    void validateAccessToken(const QString &accessToken);

private:
    SettingsDialogPrivate *d;
};

#endif // SETTINGSDIALOG_H
