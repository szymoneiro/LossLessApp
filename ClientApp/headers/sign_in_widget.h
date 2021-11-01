#ifndef SIGN_IN_WIDGET_H
#define SIGN_IN_WIDGET_H

#include <QtWidgets>
#include <QNetworkAccessManager>

class SignInWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SignInWidget(QWidget *parent = nullptr);

signals:
    void signUpClicked();
    void userLoggedIn();

private:
    QVBoxLayout *widgetLayout;

    /* Upper area of widget */
    QWidget *signUpWidget;
    QVBoxLayout *signInLayout;
    QLineEdit *userInputFields[2];
    QLabel *textHelpers[2];
    QPushButton *signUpButton;
    QStringList fieldsTexts = { "USERNAME", "PASSWORD" };

    /* Lower are of widget */
    QWidget *signInWidget;
    QLabel *alreadyText;
    QPushButton *signInButton;
    QLabel *errorLabel;
    QPushButton *showPasswordButton;

    /* API connection components */
    QNetworkAccessManager *networkManager;
    QNetworkReply *reply;


    void layoutCreate();
    void signUpCreate();
    void signInCreate();
    void fieldsCreate();
    void buttonsCreate();
    void updateErrorLabel(QString message, bool error);

private slots:
    void onSignInButtonClicked();
    void onSignUpButtonClicked();
    void signInFinished(QNetworkReply *reply);
    void showPassword();
    void hidePassword();
};

#endif // SIGN_IN_WIDGET_H
