#ifndef TRAYICON_H
#define TRAYICON_H

#include <QSystemTrayIcon>

class TrayIcon : public QSystemTrayIcon
{
    Q_OBJECT
public:
    explicit TrayIcon(QObject *parent = 0);

signals:

public slots:

private slots:
    void showSettings();

private:
    void setupMenu();
};

#endif // TRAYICON_H
