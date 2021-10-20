#ifndef SIGNUP_WIDGET_H
#define SIGNUP_WIDGET_H

#include <QtWidgets>

class SignUpWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SignUpWidget(QWidget *parent = nullptr);
private:
    QVBoxLayout *widgetLayout;

    /* Upper area of widget */
    QWidget *signUpWidget;
    QVBoxLayout *signUpLayout;
    QLineEdit *userInputFields[3];
    QLabel *textHelpers[3];
    QPushButton *signUpButton;
    QStringList fieldsTexts = {"USERNAME", "PASSWORD", "STARTING BALANCE[$]"};

    /* Lower are of widget */
    QWidget *signInWidget;
    QLabel *alreadyText;
    QPushButton *signInButton;


    void layoutCreate();
    void signInCreate();
    void signUpCreate();
    void fieldsCreate();
    void buttonsCreate();
};

#endif // SIGNUP_WIDGET_H
