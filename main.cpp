#include <QApplication>
#include <QIcon>
#include <QString>
#include <QMenu>
#include <QAction>
#include <QKeySequence>
#include <QSettings>

#include "periodiccameracapture.h"
#include "fileuploader.h"
#include "settingsdialog.h"
#include "trayicon.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setQuitOnLastWindowClosed(false);

    QCoreApplication::setOrganizationName("HiGuys");
    QCoreApplication::setOrganizationDomain("cantierecreativo.net");
    QCoreApplication::setApplicationName("HiGuys");

    TrayIcon *tray = new TrayIcon;
    tray->setIcon(QIcon(":icons/higuys-osx-mini-icon.png"));
    tray->setToolTip("Hi Guys!");
    tray->setVisible(true);

    QSettings settings;
    QString accessToken = settings.value("account/token", "").toString();
    if (accessToken.isEmpty()) {
        SettingsDialog *settingsDialog = new SettingsDialog;
        if (settingsDialog->exec() == QDialog::Rejected) {
            return -1;
        }
        accessToken = settings.value("account/token", "").toString();
    }

    PeriodicCameraCapture *capture = new PeriodicCameraCapture;
    capture->setTimeout(20000);

    FileUploader *uploader = new FileUploader(accessToken);
    QObject::connect(capture, SIGNAL(newImageAvailable(QString)), uploader, SLOT(uploadFile(QString)));

    capture->start();

    return a.exec();
}
