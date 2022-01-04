#ifndef SIGN_IN_WIDGET_H
#define SIGN_IN_WIDGET_H

#include "authentication_widget.h"

class SignInWidget : public AuthenticationWidget
{
    Q_OBJECT
private:
    QVBoxLayout *signInBoxLayout;
    QLineEdit *userInputFields[2];
    QLabel *userInputLabels[2];

signals:
    void signUpClicked();
    void userLoggedIn();

private slots:
    void onSignInButtonClicked() override;
    void onSignUpButtonClicked() override;

    void signInFinished();

    void showPassword();
    void hidePassword();

public:
    explicit SignInWidget(QWidget *parent = nullptr);

    void signUpBoxCreate() override;
    void signInBoxCreate() override;
    void layoutCreate() override;
    void inputFieldsCreate() override;
    void labelsCreate() override;
    void buttonsCreate();
};

#endif // SIGN_IN_WIDGET_H
