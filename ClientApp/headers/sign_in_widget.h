#ifndef SIGN_IN_WIDGET_H
#define SIGN_IN_WIDGET_H

#include <QtWidgets>

class SignInWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SignInWidget(QWidget *parent = nullptr);

signals:
    void signUpClicked();

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


    void layoutCreate();
    void signUpCreate();
    void signInCreate();
    void fieldsCreate();
    void buttonsCreate();

private slots:
        void onSignUpButtonClicked();
};

#endif // SIGN_IN_WIDGET_H
