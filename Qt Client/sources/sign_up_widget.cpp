#include "../headers/sign_up_widget.h"
#include "../headers/top_bar_and_stacked_widget.h"
#include <QNetworkReply>

SignUpWidget::SignUpWidget(QWidget *parent) : AuthenticationWidget(parent)
{
    SignUpWidget::signInBoxCreate();
    SignUpWidget::signUpBoxCreate();
    SignUpWidget::layoutCreate();
    SignUpWidget::inputFieldsCreate();
    SignUpWidget::labelsCreate();
    buttonsCreate();

    connect(signInButton, &QPushButton::clicked, this, &SignUpWidget::onSignInButtonClicked);
    connect(signUpButton, &QPushButton::clicked, this, &SignUpWidget::onSignUpButtonClicked);
    connect(showPasswordButton, &QPushButton::pressed, this, &SignUpWidget::showPassword);
    connect(showPasswordButton, &QPushButton::released, this, &SignUpWidget::hidePassword);
}

void SignUpWidget::signInBoxCreate()
{
    signInBox = new QWidget(this);
    signInBox->setGeometry(307, 350, 585, 190);
    signInBox->setStyleSheet("background-color: #4C3099;"
                             "border-radius: 10px;"
                             "border: 3px solid #000000");
}

void SignUpWidget::signUpBoxCreate()
{
    signUpBox = new QWidget(this);
    signUpBox->setGeometry(275, 85, 650, 380);
    signUpBox->setStyleSheet("background-color: #00FFA3;"
                             "border-radius: 10px;"
                             "border: 3px solid #000000");
}

void SignUpWidget::inputFieldsCreate()
{
    for (int i = 0; i < 3; ++i){
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
    userInputFields[2]->setValidator(new QIntValidator(0, INT_MAX, userInputFields[2]));

    for (int i = 0; i < 3; ++i)
        signUpBoxLayout->addWidget(userInputFields[i]);
}

void SignUpWidget::layoutCreate()
{
    signUpBoxLayout = new QVBoxLayout(signUpBox);
    signUpBoxLayout->setSpacing(28);
    signUpBoxLayout->setContentsMargins(125, 54, 125, 102);
}

void SignUpWidget::labelsCreate()
{
    inputLabelsText.append("STARTING BALANCE[$]");

    redirectLabel = new QLabel(this);
    redirectLabel->setText("Already have an account?");
    redirectLabel->setStyleSheet("color: #FFFFFF;"
                                 "font: bold;"
                                 "background-color: rgba(0,0,0,0%);"
                                 "font-size: 20px;"
                                 "border: 0px");
    redirectLabel->setGeometry(396, 484, 250, 40);

    QString inputLabelsStylesheet = "color: #000000;"
                                    "font: bold;"
                                    "background-color: rgba(0,0,0,0%);"
                                    "font-size: 13px";
    for (int i = 0; i < 3; ++i) {
        userInputLabels[i] = new QLabel(this);
        userInputLabels[i]->setText(inputLabelsText[i]);
        userInputLabels[i]->setStyleSheet(inputLabelsStylesheet);
    }
    userInputLabels[0]->setGeometry(429, 117, 79, 17);
    userInputLabels[1]->setGeometry(429, 201, 79, 17);
    userInputLabels[2]->setGeometry(429, 285, 146, 17);

    messageLabel = new QLabel(this);
    messageLabel->setAlignment(Qt::AlignCenter);
    messageLabel->setGeometry(300, 558, 600, 56);
    messageLabel->setVisible(false);
}

void SignUpWidget::buttonsCreate()
{
    AuthenticationWidget::buttonsCreate();

    QString signUpStyleSheet = "QPushButton {"
                                "background-color: #4C3099;"
                                "border-radius: 28px;"
                                "border: 2px solid #000000;"
                                "font: bold;"
                                "color: #FFFFFF;}"
                                "QPushButton:pressed {"
                                "background-color: #332066;}";
    signUpButton->setStyleSheet(signUpStyleSheet);
    signUpButton->setGeometry(500, 391, 200, 56);

    QString signInStylesheet = "QPushButton {"
                               "background-color: #00FFA3;"
                               "border-radius: 15px;"
                               "border: 2px solid #000000;"
                               "font: bold;"
                               "color: #000000;}"
                               "QPushButton:pressed {"
                               "background-color: #00BD79;}";
    signInButton->setStyleSheet(signInStylesheet);
    signInButton->setGeometry(677, 481, 151, 43);

    createAndAddShadowEffect(signUpButton);
    createAndAddShadowEffect(signInButton);

    createShowPasswordButton();
    showPasswordButton->setGeometry(744, 235, 32, 32);
}

void SignUpWidget::onSignInButtonClicked()
{
    emit signInClicked();
    for (int i = 0; i < 3; ++i)
        userInputFields[i]->clear();
    messageLabel->setVisible(false);
}

void SignUpWidget::onSignUpButtonClicked()
{
    if (userInputFields[0]->text().isEmpty() ||
        userInputFields[1]->text().isEmpty() ||
        userInputFields[2]->text().isEmpty()) {

        updateMessageLabel("Missing one or more fields!", true);
        /* Hide error message after 3 seconds */
        QTimer::singleShot(3000, this, [this]{
            messageLabel->setVisible(false);
        });
        messageLabel->setVisible(true);
    }
    else {
        /* Password requirements:
         * 8-16 characters long
         * minimum 1 number
         * minimun 1 uppercase letter */
        QString password = userInputFields[1]->text();
        QRegularExpression re("^(?=.*[a-z])(?=.*[A-Z])(?=.*\\d)[a-zA-Z\\d]{8,}$");
        QRegularExpressionMatch match = re.match(password);
        if (match.hasMatch() == false) {
            QTimer::singleShot(3000, this, [this]{
                messageLabel->setVisible(false);
                messageLabel->setGeometry(300, 558, 600, 56);
            });
            updateMessageLabel("Password must contain 8-16 characters, 1 uppercase and 1 number!", true);
            messageLabel->setGeometry(250, 558, 700, 56);
            messageLabel->setVisible(true);
            return;
        }

        updateMessageLabel("Processing...", false);
        messageLabel->setVisible(true);

        QNetworkRequest registerRequest;
        registerRequest.setUrl(QUrl("http://127.0.0.1:5000/register"));
        registerRequest.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("application/x-www-form-urlencoded"));

        QUrlQuery query;
        query.addQueryItem("username", userInputFields[0]->text());
        query.addQueryItem("password", userInputFields[1]->text());
        query.addQueryItem("balance", userInputFields[2]->text());

        QByteArray postData;
        postData = query.toString(QUrl::FullyEncoded).toUtf8();

        serverReply = this->connectionManager->post(registerRequest, postData);
        connect(serverReply, &QNetworkReply::finished,
                this, &SignUpWidget::signUpFinished);
    }
}

void SignUpWidget::signUpFinished()
{
    if (serverReply->error()) {
            QTimer::singleShot(5000, this, [this]{
                messageLabel->setVisible(false);
            });
            if (serverReply->error() == QNetworkReply::ContentConflictError) {
                updateMessageLabel("Username is already taken! Try another!", true);
            }
            else {
                updateMessageLabel("Server error! Try again later!", true);
            }
        }
        else {
            QTimer::singleShot(3000, this, [this]{
                messageLabel->setVisible(false);
                onSignInButtonClicked();
            });
            updateMessageLabel("Success! Redirecting...", false);
        }
        serverReply->deleteLater();
}

void SignUpWidget::showPassword()
{
    userInputFields[1]->setEchoMode(QLineEdit::Normal);
    AuthenticationWidget::showPassword();
}

void SignUpWidget::hidePassword()
{
    userInputFields[1]->setEchoMode(QLineEdit::Password);
    AuthenticationWidget::hidePassword();
}
