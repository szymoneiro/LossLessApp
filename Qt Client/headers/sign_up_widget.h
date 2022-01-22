#ifndef SIGN_UP_WIDGET_H
#define SIGN_UP_WIDGET_H

#include "authentication_widget.h"

class SignUpWidget : public AuthenticationWidget
{
    Q_OBJECT
private:
    QVBoxLayout *signUpBoxLayout;
    QLineEdit *userInputFields[3];
    QLabel *userInputLabels[3];

signals:
    void signInClicked();

private slots:
    void onSignInButtonClicked() override;
    void onSignUpButtonClicked() override;

    void signUpFinished();

    void showPassword();
    void hidePassword();

public:
    explicit SignUpWidget(QWidget *parent = nullptr);

    void signInBoxCreate() override;
    void signUpBoxCreate() override;
    void layoutCreate() override;
    void inputFieldsCreate() override;
    void labelsCreate() override;
    void buttonsCreate();
};

#endif // SIGN_UP_WIDGET_H
