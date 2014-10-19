#include "trayicon.h"

#include <QApplication>
#include <QMenu>

#include "settingsdialog.h"


TrayIcon::TrayIcon(QObject *parent) :
    QSystemTrayIcon(parent)
{
    setupMenu();
}

void TrayIcon::setupMenu()
{
    QMenu *menu = new QMenu;
    menu->addAction("Settings", this, SLOT(showSettings()));
    menu->addSeparator();
    menu->addAction("Quit", qApp, SLOT(quit()), QKeySequence("ctrl+q"));

    setContextMenu(menu);
}

void TrayIcon::showSettings()
{
    SettingsDialog *settingsDialog = new SettingsDialog;
    settingsDialog->exec();
}

