#include "settingsdialog.h"

#include <QFormLayout>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QDebug>
#include <QSettings>
#include <QLabel>
#include <QPushButton>
#include <QRegExpValidator>

class SettingsDialogPrivate
{
public:
    QSettings settings;
    QLineEdit *tokenEdit;
    QPushButton *loginButton;

    void setupUI(SettingsDialog *dialog);
};

void SettingsDialogPrivate::setupUI(SettingsDialog *dialog)
{
    QVBoxLayout *layout = new QVBoxLayout(dialog);

    QLabel *logoLabel = new QLabel;
    logoLabel->setAlignment(Qt::AlignCenter | Qt::AlignHCenter);
    logoLabel->setPixmap(QPixmap(":/images/logo-higuys.png"));
    layout->addWidget(logoLabel);

    tokenEdit = new QLineEdit;
    tokenEdit->setPlaceholderText("Access Token");
    tokenEdit->setText(settings.value("account/token", QString("")).toString());
    layout->addWidget(tokenEdit);

    QRegExpValidator *tokenValidator = new QRegExpValidator(QRegExp("^[a-f0-9]{64}$"));
    tokenEdit->setValidator(tokenValidator);

    loginButton = new QPushButton("Login");
    layout->addWidget(loginButton);
}

SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent), d(new SettingsDialogPrivate)
{
    d->setupUI(this);

    connect(d->loginButton, SIGNAL(clicked()), this, SLOT(accept()));
    connect(d->tokenEdit, SIGNAL(textChanged(QString)), this, SLOT(validateAccessToken(QString)));

    validateAccessToken(d->tokenEdit->text());
}

void SettingsDialog::accept()
{
    d->settings.setValue("account/token", d->tokenEdit->text());

    QDialog::accept();
}

void SettingsDialog::validateAccessToken(const QString &accessToken)
{
    d->loginButton->setEnabled(accessToken.length() == 64);
}
