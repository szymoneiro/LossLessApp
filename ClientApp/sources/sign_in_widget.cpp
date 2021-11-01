#include "../headers/sign_in_widget.h"
#include <QNetworkReply>
#include "../headers/top_bar_and_stacked_widget.h"

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

    connect(signUpButton, SIGNAL(clicked()), SLOT(onSignUpButtonClicked()));
    connect(signInButton, SIGNAL(clicked()), SLOT(onSignInButtonClicked()));
    connect(showPasswordButton, SIGNAL(pressed()), SLOT(showPassword()));
    connect(showPasswordButton, SIGNAL(released()), SLOT(hidePassword()));


    /* Add them to layout */
    widgetLayout->addWidget(signUpWidget);

    for (int i = 0; i < 2; ++i)
        signInLayout->addWidget(userInputFields[i]);

    networkManager = new QNetworkAccessManager(this);
    connect(networkManager, SIGNAL(finished(QNetworkReply*)),
            SLOT(signInFinished(QNetworkReply*)));
}

void SignInWidget::onSignUpButtonClicked()
{
    emit signUpClicked();
    for (int i = 0; i < 2; ++i)
        userInputFields[i]->clear();
    errorLabel->setVisible(false);
}

void SignInWidget::signInFinished(QNetworkReply *reply)
{
    if (reply->error()) {
        QTimer::singleShot(5000, this, [this]{
            errorLabel->setVisible(false);
        });
        if (reply->error() == QNetworkReply::AuthenticationRequiredError) {
            updateErrorLabel("Wrong username or password! Try again!", true);
        }
        else {
            updateErrorLabel("Server error! Try again later!", true);
        }
    }
    else {
        QTimer::singleShot(3000, this, [this]{
            errorLabel->setVisible(false);
        });
        updateErrorLabel("Success! Redirecting...", false);
        QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
        /* TopBarAndStackedWidget -> StackedWidget -> SignInWidget */
        dynamic_cast<TopBarAndStackedWidget*>(parent()->parent())->x_access_token = doc["token"].toString();
        emit userLoggedIn();
    }
}

void SignInWidget::showPassword()
{
    userInputFields[1]->setEchoMode(QLineEdit::Normal);
    QDir iconsDir;
    iconsDir.setPath("../ClientApp/icons");
    QString icon_path = iconsDir.absolutePath() + "/" + "hide_icon.png";
    showPasswordButton->setIcon(QIcon(icon_path));
    showPasswordButton->setIconSize(QSize(32, 32));
}

void SignInWidget::hidePassword()
{
    userInputFields[1]->setEchoMode(QLineEdit::Password);
    QDir iconsDir;
    iconsDir.setPath("../ClientApp/icons");
    QString icon_path = iconsDir.absolutePath() + "/" + "show_icon.png";
    showPasswordButton->setIcon(QIcon(icon_path));
    showPasswordButton->setIconSize(QSize(32, 32));
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

    errorLabel = new QLabel(this);
    errorLabel->setAlignment(Qt::AlignCenter);
    errorLabel->setGeometry(300, 558, 600, 56);
    errorLabel->setVisible(false);
}

void SignInWidget::buttonsCreate()
{
    signUpButton = new QPushButton("SIGN UP", this);
    QString signUpStyleSheet = "QPushButton {"
                                "background-color: #4C3099;"
                                "border-radius: 15px;"
                                "border: 2px solid #000000;"
                                "font: bold;"
                                "color: #FFFFFF;}"
                                "QPushButton:pressed {"
                                "background-color: #332066;}";
    signUpButton->setStyleSheet(signUpStyleSheet);
    signUpButton->setGeometry(677, 481, 151, 43);

    signInButton = new QPushButton("SIGN IN", this);
    signInButton->setFixedSize(200, 56);
    QString signInStylesheet = "QPushButton {"
                               "background-color: #00FFA3;"
                               "border-radius: 28px;"
                               "border: 2px solid #000000;"
                               "font: bold;"
                               "color: #000000;}"
                               "QPushButton:pressed {"
                               "background-color: #00BD79;}";
    signInButton->setStyleSheet(signInStylesheet);
    signInButton->setGeometry(500, 331, 200, 56);

    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect(signUpButton);
    shadow->setBlurRadius(5);
    /* 0.1 * 255 ~ 26 */
    shadow->setColor(QColor(0,0,0,26));
    shadow->setOffset(4, 4);

    signUpButton->setGraphicsEffect(shadow);
    signInButton->setGraphicsEffect(shadow);

    showPasswordButton = new QPushButton(this);
    QDir iconsDir;
    iconsDir.setPath("../ClientApp/icons");
    QString icon_path = iconsDir.absolutePath() + "/" + "show_icon.png";
    showPasswordButton->setIcon(QIcon(icon_path));
    showPasswordButton->setIconSize(QSize(32, 32));
    showPasswordButton->setStyleSheet("background: transparent");
    showPasswordButton->setGeometry(744, 259, 32, 32);
}

void SignInWidget::updateErrorLabel(QString message, bool error)
{
    errorLabel->setText(message);
    if (error) {
        errorLabel->setStyleSheet("background-color: #FF0000;"
                                  "font: bold;"
                                  "color: #FFFFFF;"
                                  "font-size: 20px;"
                                  "border: 2px solid #000000;"
                                  "border-radius: 10px");
    }
    else {
        errorLabel->setStyleSheet("background-color: #00FFA3;"
                                  "font: bold;"
                                  "color: #FFFFFF;"
                                  "font-size: 20px;"
                                  "border: 2px solid #000000;"
                                  "border-radius: 10px");
    }
}

void SignInWidget::onSignInButtonClicked()
{
    if (userInputFields[0]->text().isEmpty() ||
        userInputFields[1]->text().isEmpty()) {

        updateErrorLabel("All fields are required!", true);
        QTimer::singleShot(3000, this, [this]{
            errorLabel->setVisible(false);
        });
        errorLabel->setVisible(true);
    }
    else {
        updateErrorLabel("Processing...", false);
        errorLabel->setVisible(true);

        QNetworkRequest loginRequest;
        loginRequest.setUrl(QUrl("http://127.0.0.1:5000/login"));
        loginRequest.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("application/x-www-form-urlencoded"));

        QUrlQuery query;
        query.addQueryItem("username", userInputFields[0]->text());
        query.addQueryItem("password", userInputFields[1]->text());

        QByteArray postData;
        postData = query.toString(QUrl::FullyEncoded).toUtf8();

        networkManager->post(loginRequest, postData);
    }
}
