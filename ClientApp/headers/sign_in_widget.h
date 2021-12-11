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
    /* Upper area of widget */
    QWidget *signInBox;
    QVBoxLayout *signInBoxLayout;
    QLineEdit *userInputFields[2];
    QLabel *inputLabels[2];
    QStringList inputLabelsTexts = { "USERNAME", "PASSWORD" };
    QPushButton *signInButton;

    /* Lower are of widget */
    QWidget *signUpBox;
    QLabel *tipLabel;
    QPushButton *signUpButton;
    QPushButton *showPasswordButton;

    /* Client/server response label */
    QLabel *errorLabel;

    /* API connection components */
    QNetworkAccessManager *signInManager;
    QNetworkReply *signInReply;


    void layoutCreate();
    void signUpBoxCreate();
    void signInBoxCreate();
    void inputFieldsCreate();
    void labelsCreate();
    void buttonsCreate();
    void updateErrorLabel(QString message, bool error);

private slots:
    void onSignInButtonClicked();
    void onSignUpButtonClicked();
    void signInFinished();
    void showPassword();
    void hidePassword();
};

#endif // SIGN_IN_WIDGET_H
