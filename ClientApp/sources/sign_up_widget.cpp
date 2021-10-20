#include "../headers/sign_up_widget.h"

SignUpWidget::SignUpWidget(QWidget *parent) : QWidget(parent)
{
    /* Set up widget size */
    this->setFixedSize(1200, 640);
    this->setStyleSheet("background-color: #FAFAFA");

    /* Create all the widget' compontents */
    signInCreate();
    signUpCreate();
    layoutCreate();
    fieldsCreate();
    buttonsCreate();

    /* Add them to layout */
    widgetLayout->addWidget(signUpWidget);

    for (int i = 0; i < 3; ++i)
        signUpLayout->addWidget(userInputFields[i]);
}

void SignUpWidget::onSignInButtonClicked()
{
    emit signInClicked();
}

void SignUpWidget::layoutCreate()
{
    widgetLayout = new QVBoxLayout(this);
    widgetLayout->setSpacing(0);
    widgetLayout->setContentsMargins(275, 85, 275, 175);

    signUpLayout = new QVBoxLayout(signUpWidget);
    signUpLayout->setSpacing(28);
    signUpLayout->setContentsMargins(0, 54, 0, 102);
    signUpLayout->setAlignment(Qt::AlignHCenter);
}

void SignUpWidget::signInCreate()
{
    signInWidget = new QWidget(this);
    signInWidget->setGeometry(307, 350, 585, 190);
    signInWidget->setStyleSheet("background-color: #4C3099;"
                                "border-radius: 10px;"
                                "border: 3px solid #000000");
}

void SignUpWidget::signUpCreate()
{
    signUpWidget = new QWidget(this);
    signUpWidget->setFixedSize(650, 380);
    signUpWidget->setStyleSheet("background-color: #00FFA3;"
                                "border-radius: 10px;"
                                "border: 3px solid #000000");
}

void SignUpWidget::fieldsCreate()
{
    alreadyText = new QLabel(this);
    alreadyText->setText("Already have an account?");
    alreadyText->setStyleSheet("color: #FFFFFF;"
                               "font: bold;"
                               "background-color: rgba(0,0,0,0%);"
                               "font-size: 20px;"
                               "border: 0px");
    alreadyText->setGeometry(396, 484, 250, 40);

    /* User input fields */
    for (int i = 0; i < 3; ++i){
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
    userInputFields[2]->setInputMask("999999999999999999");

    /* Info texts */
    for (int i = 0; i < 3; ++i){
        textHelpers[i] = new QLabel(this);
        textHelpers[i]->setText(fieldsTexts[i]);
        textHelpers[i]->setStyleSheet("color: #000000;"
                                      "font: bold;"
                                      "background-color: rgba(0,0,0,0%);"
                                      "font-size: 13px");
    }
    textHelpers[0]->setGeometry(429, 117, 79, 17);
    textHelpers[1]->setGeometry(429, 201, 79, 17);
    textHelpers[2]->setGeometry(429, 285, 146, 17);
}

void SignUpWidget::buttonsCreate()
{
    signInButton = new QPushButton("SIGN IN", this);
    signInButton->setStyleSheet("background-color: #00FFA3;"
                                "border-radius: 15px;"
                                "border: 2px solid #000000;"
                                "font: bold;"
                                "color: #000000");
    signInButton->setGeometry(677, 481, 151, 43);

    signUpButton = new QPushButton("SIGN UP", this);
    signUpButton->setFixedSize(200, 56);
    signUpButton->setStyleSheet("background-color: #4C3099;"
                                "border-radius: 28px;"
                                "border: 2px solid #000000;"
                                "font: bold;"
                                "color: #FFFFFF");
    signUpButton->setGeometry(500, 391, 200, 56);

    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect(signUpButton);
    shadow->setBlurRadius(5);
    /* 0.1 * 255 ~ 26 */
    shadow->setColor(QColor(0,0,0,26));
    shadow->setOffset(4, 4);

    signInButton->setGraphicsEffect(shadow);
    signUpButton->setGraphicsEffect(shadow);

    connect(signInButton, SIGNAL(clicked()), SLOT(onSignInButtonClicked()));
}
