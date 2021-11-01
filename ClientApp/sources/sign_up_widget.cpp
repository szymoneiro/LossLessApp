#include "../headers/sign_up_widget.h"
#include <QNetworkReply>
#include <QJsonDocument>

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

    connect(signUpButton, SIGNAL(clicked()), SLOT(onSignUpButtonClicked()));
    connect(signInButton, SIGNAL(clicked()), SLOT(onSignInButtonClicked()));
    connect(showPasswordButton, SIGNAL(pressed()), SLOT(showPassword()));
    connect(showPasswordButton, SIGNAL(released()), SLOT(hidePassword()));

    /* Add them to layout */
    widgetLayout->addWidget(signUpWidget);

    for (int i = 0; i < 3; ++i)
        signUpLayout->addWidget(userInputFields[i]);

    networkManager = new QNetworkAccessManager(this);
    connect(networkManager, SIGNAL(finished(QNetworkReply*)),
            SLOT(signUpFinished(QNetworkReply*)));
}

void SignUpWidget::onSignInButtonClicked()
{
    emit signInClicked();
    for (int i = 0; i < 3; ++i)
        userInputFields[i]->clear();
    errorLabel->setVisible(false);
}

void SignUpWidget::onSignUpButtonClicked()
{
    if (userInputFields[0]->text().isEmpty() ||
        userInputFields[1]->text().isEmpty() ||
        userInputFields[2]->text().isEmpty()) {

        updateErrorLabel("Missing one or more fields!", true);
        /* Hide error message after 3 seconds */
        QTimer::singleShot(3000, this, [this]{
            errorLabel->setVisible(false);
        });
        errorLabel->setVisible(true);
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
                errorLabel->setVisible(false);
                errorLabel->setGeometry(300, 558, 600, 56);
            });
            updateErrorLabel("Password must contain 8-16 characters, 1 uppercase and 1 number!", true);
            errorLabel->setGeometry(250, 558, 700, 56);
            errorLabel->setVisible(true);
            return;
        }

        updateErrorLabel("Processing...", false);
        errorLabel->setVisible(true);

        QNetworkRequest registerRequest;
        registerRequest.setUrl(QUrl("http://127.0.0.1:5000/register"));
        registerRequest.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("application/x-www-form-urlencoded"));

        QUrlQuery query;
        query.addQueryItem("username", userInputFields[0]->text());
        query.addQueryItem("password", userInputFields[1]->text());
        query.addQueryItem("balance", userInputFields[2]->text());

        QByteArray postData;
        postData = query.toString(QUrl::FullyEncoded).toUtf8();

        networkManager->post(registerRequest, postData);
    }
}

void SignUpWidget::signUpFinished(QNetworkReply *reply)
{
    if (reply->error()) {
        QTimer::singleShot(5000, this, [this]{
            errorLabel->setVisible(false);
        });
        if (reply->error() == QNetworkReply::ContentConflictError) {
            updateErrorLabel("Username is already taken! Try another!", true);
        }
        else {
            updateErrorLabel("Server error! Try again later!", true);
        }
    }
    else {
        QTimer::singleShot(3000, this, [this]{
            errorLabel->setVisible(false);
            onSignInButtonClicked();
        });
        updateErrorLabel("Success! Redirecting...", false);
    }
}

void SignUpWidget::showPassword()
{
    userInputFields[1]->setEchoMode(QLineEdit::Normal);
    QDir iconsDir;
    iconsDir.setPath("../ClientApp/icons");
    QString icon_path = iconsDir.absolutePath() + "/" + "hide_icon.png";
    showPasswordButton->setIcon(QIcon(icon_path));
    showPasswordButton->setIconSize(QSize(32, 32));
}

void SignUpWidget::hidePassword()
{
    userInputFields[1]->setEchoMode(QLineEdit::Password);
    QDir iconsDir;
    iconsDir.setPath("../ClientApp/icons");
    QString icon_path = iconsDir.absolutePath() + "/" + "show_icon.png";
    showPasswordButton->setIcon(QIcon(icon_path));
    showPasswordButton->setIconSize(QSize(32, 32));
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

    errorLabel = new QLabel(this);
    errorLabel->setAlignment(Qt::AlignCenter);
    errorLabel->setGeometry(300, 558, 600, 56);
    errorLabel->setVisible(false);
}

void SignUpWidget::buttonsCreate()
{
    signInButton = new QPushButton("SIGN IN", this);
    signInButton->setGeometry(677, 481, 151, 43);
    QString signInStylesheet = "QPushButton {"
                               "background-color: #00FFA3;"
                               "border-radius: 15px;"
                               "border: 2px solid #000000;"
                               "font: bold;"
                               "color: #000000;}"
                               "QPushButton:pressed {"
                               "background-color: #00BD79;}";
    signInButton->setStyleSheet(signInStylesheet);


    signUpButton = new QPushButton("SIGN UP", this);
    signUpButton->setFixedSize(200, 56);
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

    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect(signUpButton);
    shadow->setBlurRadius(5);
    /* 0.1 * 255 ~ 26 */
    shadow->setColor(QColor(0,0,0,26));
    shadow->setOffset(4, 4);

    signInButton->setGraphicsEffect(shadow);
    signUpButton->setGraphicsEffect(shadow);

    showPasswordButton = new QPushButton(this);
    QDir iconsDir;
    iconsDir.setPath("../ClientApp/icons");
    QString icon_path = iconsDir.absolutePath() + "/" + "show_icon.png";
    showPasswordButton->setIcon(QIcon(icon_path));
    showPasswordButton->setIconSize(QSize(32, 32));
    showPasswordButton->setStyleSheet("background: transparent");
    showPasswordButton->setGeometry(744, 235, 32, 32);
}

void SignUpWidget::updateErrorLabel(QString message, bool error)
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
