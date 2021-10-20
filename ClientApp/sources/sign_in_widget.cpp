#include "../headers/sign_in_widget.h"

SignInWidget::SignInWidget(QWidget *parent) : QWidget(parent)
{
    /* Set up widget size */
    this->setFixedSize(1200, 640);
    this->setStyleSheet("background-color: #FAFAFA");

    /* Create all the widget' compontents */
    signUpCreate();
    signInCreate();
    layoutCreate();
    fieldsCreate();
    buttonsCreate();

    /* Add them to layout */
    widgetLayout->addWidget(signUpWidget);

    for (int i = 0; i < 2; ++i)
        signInLayout->addWidget(userInputFields[i]);
}

void SignInWidget::onSignUpButtonClicked()
{
    emit signUpClicked();
}

void SignInWidget::layoutCreate()
{
    widgetLayout = new QVBoxLayout(this);
    widgetLayout->setSpacing(0);
    widgetLayout->setContentsMargins(275, 85, 275, 175);

    signInLayout = new QVBoxLayout(signUpWidget);
    signInLayout->setSpacing(28);
    signInLayout->setContentsMargins(125, 78, 125, 162);
}

void SignInWidget::signUpCreate()
{
    signInWidget = new QWidget(this);
    signInWidget->setGeometry(307, 350, 585, 190);
    signInWidget->setStyleSheet("background-color: #00FFA3;"
                                "border-radius: 10px;"
                                "border: 3px solid #000000");
}

void SignInWidget::signInCreate()
{
    signUpWidget = new QWidget(this);
    signUpWidget->setFixedSize(650, 380);
    signUpWidget->setStyleSheet("background-color: #4C3099;"
                                "border-radius: 10px;"
                                "border: 3px solid #000000");
}

void SignInWidget::fieldsCreate()
{
    alreadyText = new QLabel(this);
    alreadyText->setText("Don't have an account?");
    alreadyText->setStyleSheet("color: #000000;"
                               "font: bold;"
                               "background-color: rgba(0,0,0,0%);"
                               "font-size: 20px;"
                               "border: 0px");
    alreadyText->setGeometry(396, 479, 250, 40);

    /* User input fields */
    for (int i = 0; i < 2; ++i){
        userInputFields[i] = new QLineEdit(this);
        userInputFields[i]->setFixedSize(400, 56);
        userInputFields[i]->setStyleSheet("background-color: #FFFFFF;"
                                          "border-radius: 28px;"
                                          "border: 2px solid #000000;"
                                          "font: 20px");
        userInputFields[i]->setTextMargins(25, 0, 0, 0);
        userInputFields[i]->setMaxLength(18);
    }
    userInputFields[1]->setEchoMode(QLineEdit::Password);

    /* Info texts */
    for (int i = 0; i < 2; ++i){
        textHelpers[i] = new QLabel(this);
        textHelpers[i]->setText(fieldsTexts[i]);
        textHelpers[i]->setStyleSheet("color: #FFFFFF;"
                                      "font: bold;"
                                      "background-color: rgba(0,0,0,0%);"
                                      "font-size: 13px");
    }
    textHelpers[0]->setGeometry(429, 141, 79, 17);
    textHelpers[1]->setGeometry(429, 225, 79, 17);
}

void SignInWidget::buttonsCreate()
{
    signUpButton = new QPushButton("SIGN UP", this);
    signUpButton->setStyleSheet("background-color: #4C3099;"
                                "border-radius: 15px;"
                                "border: 2px solid #000000;"
                                "font: bold;"
                                "color: #000000");
    signUpButton->setGeometry(677, 481, 151, 43);

    signInButton = new QPushButton("SIGN IN", this);
    signInButton->setFixedSize(200, 56);
    signInButton->setStyleSheet("background-color: #00FFA3;"
                                "border-radius: 28px;"
                                "border: 2px solid #000000;"
                                "font: bold;"
                                "color: #000000");
    signInButton->setGeometry(500, 331, 200, 56);

    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect(signUpButton);
    shadow->setBlurRadius(5);
    /* 0.1 * 255 ~ 26 */
    shadow->setColor(QColor(0,0,0,26));
    shadow->setOffset(4, 4);

    signUpButton->setGraphicsEffect(shadow);
    signInButton->setGraphicsEffect(shadow);

    connect(signUpButton, SIGNAL(clicked()), SLOT(onSignUpButtonClicked()));
}
