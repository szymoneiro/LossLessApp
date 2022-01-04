#include "../headers/sign_in_widget.h"
#include "../headers/top_bar_and_stacked_widget.h"
#include <QNetworkReply>

SignInWidget::SignInWidget(QWidget *parent) : AuthenticationWidget(parent)
{
    SignInWidget::signUpBoxCreate();
    SignInWidget::signInBoxCreate();
    SignInWidget::layoutCreate();
    SignInWidget::inputFieldsCreate();
    SignInWidget::labelsCreate();
    buttonsCreate();

    /* Connect all buttons' functions */
    connect(signInButton, &QPushButton::clicked, this, &SignInWidget::onSignInButtonClicked);
    connect(signUpButton, &QPushButton::clicked, this, &SignInWidget::onSignUpButtonClicked);
    connect(showPasswordButton, &QPushButton::pressed, this, &SignInWidget::showPassword);
    connect(showPasswordButton, &QPushButton::released, this, &SignInWidget::hidePassword);
}

void SignInWidget::signUpBoxCreate()
{
    signUpBox = new QWidget(this);
    signUpBox->setGeometry(307, 350, 585, 190);
    signUpBox->setStyleSheet("background-color: #00FFA3;"
                             "border-radius: 10px;"
                             "border: 3px solid #000000");
}

void SignInWidget::signInBoxCreate()
{
    signInBox = new QWidget(this);
    signInBox->setGeometry(275, 85, 650, 380);
    signInBox->setStyleSheet("background-color: #4C3099;"
                             "border-radius: 10px;"
                             "border: 3px solid #000000");
}

void SignInWidget::inputFieldsCreate()
{
    for (int i = 0; i < 2; ++i){
        userInputFields[i] = new QLineEdit(this);
        userInputFields[i]->setFixedSize(400, 56);
        QString inputStyleSheet = "QLineEdit {"
                                  "background-color: #FFFFFF;"
                                  "border-radius: 28px;"
                                  "border: 2px solid #000000;"
                                  "font: 20px;}"
                                  "QLineEdit:focus {"
                                  "background-color: #B0B0B0;}";
        userInputFields[i]->setStyleSheet(inputStyleSheet);
        userInputFields[i]->setTextMargins(25, 0, 0, 0);
        userInputFields[i]->setMaxLength(18);
    }
    userInputFields[1]->setEchoMode(QLineEdit::Password);

    for (int i = 0; i < 2; ++i)
        signInBoxLayout->addWidget(userInputFields[i]);
}

void SignInWidget::layoutCreate()
{
    signInBoxLayout = new QVBoxLayout(signInBox);
    signInBoxLayout->setSpacing(28);
    signInBoxLayout->setContentsMargins(125, 78, 125, 162);
}

void SignInWidget::labelsCreate()
{
    redirectLabel = new QLabel(this);
    redirectLabel->setText("Don't have an account?");
    redirectLabel->setStyleSheet("color: #000000;"
                                 "font: bold;"
                                 "background-color: rgba(0,0,0,0%);"
                                 "font-size: 20px;"
                                 "border: 0px");
    redirectLabel->setGeometry(396, 479, 250, 40);

    QString inputLabelsStylesheet = "color: #FFFFFF;"
                                    "font: bold;"
                                    "background-color: rgba(0,0,0,0%);"
                                    "font-size: 13px";
    for (int i = 0; i < 2; ++i){
        userInputLabels[i] = new QLabel(this);
        userInputLabels[i]->setText(inputLabelsText[i]);
        userInputLabels[i]->setStyleSheet(inputLabelsStylesheet);
    }
    userInputLabels[0]->setGeometry(429, 141, 79, 17);
    userInputLabels[1]->setGeometry(429, 225, 79, 17);

    messageLabel = new QLabel(this);
    messageLabel->setAlignment(Qt::AlignCenter);
    messageLabel->setGeometry(300, 558, 600, 56);
    messageLabel->setVisible(false);
}

void SignInWidget::buttonsCreate()
{
    AuthenticationWidget::buttonsCreate();

    QString signUpButtonStyleSheet = "QPushButton {"
                                "background-color: #4C3099;"
                                "border-radius: 15px;"
                                "border: 2px solid #000000;"
                                "font: bold;"
                                "color: #FFFFFF;}"
                                "QPushButton:pressed {"
                                "background-color: #332066;}";
    signUpButton->setStyleSheet(signUpButtonStyleSheet);
    signUpButton->setGeometry(677, 481, 151, 43);

    signInButton->setFixedSize(200, 56);
    QString signInButtonStylesheet = "QPushButton {"
                               "background-color: #00FFA3;"
                               "border-radius: 28px;"
                               "border: 2px solid #000000;"
                               "font: bold;"
                               "color: #000000;}"
                               "QPushButton:pressed {"
                               "background-color: #00BD79;}";
    signInButton->setStyleSheet(signInButtonStylesheet);
    signInButton->setGeometry(500, 331, 200, 56);

    createAndAddShadowEffect(signUpButton);
    createAndAddShadowEffect(signInButton);

    createShowPasswordButton();
    showPasswordButton->setGeometry(744, 259, 32, 32);
}

void SignInWidget::onSignInButtonClicked()
{
    if (userInputFields[0]->text().isEmpty() ||
        userInputFields[1]->text().isEmpty()) {

        updateMessageLabel("All fields are required!", true);
        QTimer::singleShot(3000, this, [this]{
            messageLabel->setVisible(false);
        });
        messageLabel->setVisible(true);
    }
    else {
        updateMessageLabel("Processing...", false);
        messageLabel->setVisible(true);

        QNetworkRequest loginRequest;
        loginRequest.setUrl(QUrl("http://127.0.0.1:5000/login"));
        loginRequest.setHeader(QNetworkRequest::ContentTypeHeader,
                               QVariant("application/x-www-form-urlencoded"));

        QUrlQuery query;
        query.addQueryItem("username", userInputFields[0]->text());
        query.addQueryItem("password", userInputFields[1]->text());

        QByteArray postData;
        postData = query.toString(QUrl::FullyEncoded).toUtf8();

        serverReply = this->connectionManager->post(loginRequest, postData);
        connect(serverReply, &QNetworkReply::finished,
                this, &SignInWidget::signInFinished);
    }
}

void SignInWidget::onSignUpButtonClicked()
{
    emit signUpClicked();
    for (int i = 0; i < 2; ++i)
        userInputFields[i]->clear();
    messageLabel->setVisible(false);
}

void SignInWidget::signInFinished()
{
    if (serverReply->error()) {
        QTimer::singleShot(5000, this, [this]{
            messageLabel->setVisible(false);
        });
        if (serverReply->error() == QNetworkReply::AuthenticationRequiredError) {
            updateMessageLabel("Wrong username or password! Try again!", true);
        }
        else {
            updateMessageLabel("Server error! Try again later!", true);
        }
    }
    else {
        QTimer::singleShot(3000, this, [this]{
            messageLabel->setVisible(false);
        });
        updateMessageLabel("Success! Redirecting...", false);
        QJsonDocument doc = QJsonDocument::fromJson(serverReply->readAll());
        qobject_cast<TopBarAndStackedWidget*>(parent()->parent())->x_access_token =
                doc["token"].toString();
        emit userLoggedIn();
    }
    serverReply->deleteLater();
}

void SignInWidget::showPassword()
{
    userInputFields[1]->setEchoMode(QLineEdit::Normal);
    AuthenticationWidget::showPassword();
}

void SignInWidget::hidePassword()
{
    userInputFields[1]->setEchoMode(QLineEdit::Password);
    AuthenticationWidget::hidePassword();
}
