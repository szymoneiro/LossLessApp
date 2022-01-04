#ifndef AUTHENTICATION_WIDGET_H
#define AUTHENTICATION_WIDGET_H

#include <QtWidgets>
#include <QNetworkAccessManager>

class AuthenticationWidget : public QWidget
{
    Q_OBJECT
protected:
    QWidget *signInBox;
    QWidget *signUpBox;

    QStringList inputLabelsText = {
        "USERNAME",
        "PASSWORD"
    };

    QPushButton *signInButton;
    QPushButton *signUpButton;
    QPushButton *showPasswordButton;

    QLabel *messageLabel;
    QLabel *redirectLabel;

    QNetworkAccessManager *connectionManager;
    QNetworkReply *serverReply;

protected slots:
    virtual void onSignInButtonClicked() = 0;
    virtual void onSignUpButtonClicked() = 0;

    void showPassword();
    void hidePassword();

public:
    explicit AuthenticationWidget(QWidget *parent = nullptr);

    virtual void signUpBoxCreate() = 0;
    virtual void signInBoxCreate() = 0;
    virtual void layoutCreate() = 0;
    virtual void inputFieldsCreate() = 0;
    virtual void labelsCreate() = 0;
    void buttonsCreate();

    void updateMessageLabel(QString message, bool isError);
    void createAndAddShadowEffect(QWidget *widget);
    void createShowPasswordButton();
};

#endif // AUTHENTICATION_WIDGET_H
