#ifndef SIGN_UP_WIDGET_H
#define SIGN_UP_WIDGET_H

#include <QtWidgets>
#include <QNetworkAccessManager>

class SignUpWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SignUpWidget(QWidget *parent = nullptr);

signals:
    void signInClicked();
private:
    QVBoxLayout *widgetLayout;

    /* Upper area of widget */
    QWidget *signUpWidget;
    QVBoxLayout *signUpLayout;
    QLineEdit *userInputFields[3];
    QLabel *textHelpers[3];
    QPushButton *signUpButton;
    QStringList fieldsTexts = {"USERNAME", "PASSWORD", "STARTING BALANCE[$]"};

    /* Lower area of widget */
    QWidget *signInWidget;
    QLabel *alreadyText;
    QLabel *errorLabel;
    QPushButton *signInButton;
    QPushButton *showPasswordButton;

    /* API connection components */
    QNetworkAccessManager *networkManager;
    QNetworkReply *reply;

    void layoutCreate();
    void signInCreate();
    void signUpCreate();
    void fieldsCreate();
    void buttonsCreate();
    void updateErrorLabel(QString message, bool error);

private slots:
    void onSignInButtonClicked();
    void onSignUpButtonClicked();
    void signUpFinished(QNetworkReply *reply);
    void showPassword();
    void hidePassword();
};

#endif // SIGN_UP_WIDGET_H
