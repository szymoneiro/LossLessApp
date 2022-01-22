#include "../headers/buy_widget.h"
#include "../headers/top_bar_and_stacked_widget.h"
#include <QNetworkReply>

BuyWidget::BuyWidget(QWidget *parent) : ScrollWidget(parent)
{
    accountBalance = qobject_cast<TopBarAndStackedWidget*>(this->parent()->parent())->accountBalance;

    scrollAreaCreate();

    connect(sideBarWidget, SIGNAL(buyPageClicked()),
            this, SLOT(onBuyButtonClicked()));

    signInWidget = qobject_cast<SignInWidget*>(this->parent()->children()[1]);
    connect(signInWidget, SIGNAL(userLoggedIn()),
            this, SLOT(setUserBalance()));

    connect(tabButtons[0], &QPushButton::clicked,
            this, &BuyWidget::onCryptocurrenciesTabClick);
    connect(tabButtons[1], &QPushButton::clicked,
            this, &BuyWidget::onCurrenciesTabClick);
    connect(tabButtons[2], &QPushButton::clicked,
            this, &BuyWidget::onStocksTabClick);
}

void BuyWidget::scrollAreaCreate()
{
    scrollAreaWidgetLayout->setSpacing(8);
    scrollAreaWidgetLayout->setContentsMargins(12, 61, 24, 24);

    const int label_width = 101;
    const int label_height = 28;
    const int label_spacing = 256;
    for (int i = 0; i < 3; ++i) {
        scrollAreaLabels[i] = new QLabel(scrollLabelsText[i], scrollAreaWidget);
        scrollAreaLabels[i]->setStyleSheet("background-color: none;"
                                           "font-size: 24px;"
                                           "color: #FFFFFF;"
                                           "font-weight: bold;"
                                           "border: 0px");
        scrollAreaLabels[i]->setFont(QFont("Lato"));
        scrollAreaLabels[i]->setGeometry(140 + i * (label_spacing + label_width),
                                         17,
                                         label_width,
                                         label_height);
        scrollAreaLabels[i]->setAlignment(Qt::AlignCenter);
    }

    scrollArea->setGeometry(22, 81, 1155, 511);
}

void BuyWidget::obtainServerRecords(recordType type)
{
    QNetworkRequest recordsRequest;
    if (type == recordType::cryptoRecord) {
        recordsRequest.setUrl(QUrl("http://127.0.0.1:5000/cryptocurrencies"));
    }
    else if (type == recordType::currencyRecord) {
        recordsRequest.setUrl(QUrl("http://127.0.0.1:5000/currencies"));
    }
    else {
        recordsRequest.setUrl(QUrl("http://127.0.0.1:5000/stocks"));
    }

    recordsRequest.setRawHeader(QByteArray("x-access-token"), accessToken->toUtf8());
    recordsRequest.setHeader(QNetworkRequest::ContentTypeHeader,
                      QVariant("application/x-www-form-urlencoded"));

    QNetworkReply *recordsReply = this->connectionManager->get(recordsRequest);

    /* Wait for the server response */
    QEventLoop connectionLoop;
    connect(recordsReply, &QNetworkReply::finished,
            &connectionLoop, &QEventLoop::quit);
    connectionLoop.exec();

    /* In case we get an error from the server */
    if (recordsReply->error()) {
        if (recordsReply->error() == QNetworkReply::ConnectionRefusedError) {
            qDebug() << "There you should implement displaying server/invalid token error!";
        }
        else {
            QJsonDocument recordsReplyError = QJsonDocument::fromJson(
                        recordsReply->readAll());
            qDebug() << recordsReplyError["message"].toString();
            recordsReply->deleteLater();
        }
        return;
    }

    QJsonDocument recordsReplyJson = QJsonDocument::fromJson(recordsReply->readAll());
    QJsonArray recordsArray = recordsReplyJson.array();

    for (int i = 0; i < recordsArray.size(); ++i) {
        QWidget *currentRecord = new QWidget(this);
        QHBoxLayout *currentRecordLayout = new QHBoxLayout(currentRecord);
        currentRecordLayout->setSpacing(0);
        currentRecordLayout->setContentsMargins(0, 0, 0, 0);

        currentRecord->setStyleSheet("background: transparent;"
                                     "border-radius: 0px;"
                                     "border: none");
        currentRecord->setFixedSize(1107, 48);

        QString recordStyleSheet = "background-color: #EAEAEA;"
                                   "border-top: 3px solid #A7A7A7;"
                                   "border-right: 3px solid #A7A7A7;"
                                   "border-bottom: 3px solid #A7A7A7;"
                                   "font-size: 18px";

        QLabel *recordName = new QLabel(recordsArray.at(i)["name"].toString(), currentRecord);
        recordName->setFixedSize(357, 48);
        recordName->setStyleSheet("background-color: #EAEAEA;"
                                  "border: 3px solid #A7A7A7;"
                                  "font-size: 18px;"
                                  "font-weight: bold");
        recordName->setFont(QFont("Lato"));
        recordName->setAlignment(Qt::AlignCenter);

        QLabel *recordPrice = new QLabel(QString::number(recordsArray.at(i)["value"].toDouble(), 'g', 12), currentRecord);
        recordPrice->setFixedSize(357, 48);
        recordPrice->setStyleSheet(recordStyleSheet);
        recordPrice->setFont(QFont("Lato"));
        recordPrice->setAlignment(Qt::AlignCenter);

        QLineEdit *recordQuantity = new QLineEdit(currentRecord);
        recordQuantity->setFixedSize(357, 48);
        recordQuantity->setStyleSheet(recordStyleSheet);
        recordQuantity->setFont(QFont("Lato"));
        recordQuantity->setAlignment(Qt::AlignCenter);
        QDoubleValidator *inputValidator = new QDoubleValidator(0, double(1'000'000'000), 17, recordQuantity);
        inputValidator->setNotation(QDoubleValidator::StandardNotation);
        recordQuantity->setValidator(inputValidator);

        QPushButton *buyButton = new QPushButton(currentRecord);
        buyButton->setFixedSize(36, 36);
        buyButton->setStyleSheet(recordStyleSheet);
        QDir iconDir;
        iconDir.setPath("../Qt Client/icons");
        QString icon_path = iconDir.absolutePath() + "/" + "shop";
        buyButton->setIcon(QIcon(icon_path));
        buyButton->setIconSize(QSize(22, 22));

        if (type == recordType::cryptoRecord) {
            connect(buyButton, &QPushButton::clicked,
                    this, &BuyWidget::buyCrypto);
        }
        else if (type == recordType::currencyRecord) {
            connect(buyButton, &QPushButton::clicked,
                    this, &BuyWidget::buyCurrency);
        }
        else {
            connect(buyButton, &QPushButton::clicked,
                    this, &BuyWidget::buyStock);
        }

        scrollAreaWidgetLayout->addWidget(currentRecord);

        currentRecordLayout->addWidget(recordName);
        currentRecordLayout->addWidget(recordPrice);
        currentRecordLayout->addWidget(recordQuantity);
        currentRecordLayout->addWidget(buyButton);
    }

    recordsReply->deleteLater();
}

void BuyWidget::buyRecord(recordType type)
{
    QNetworkRequest requestUrl;
    if (type == recordType::cryptoRecord) {
        requestUrl.setUrl(QUrl("http://127.0.0.1:5000/cryptocurrencies"));
    }
    else if (type == recordType::currencyRecord) {
        requestUrl.setUrl(QUrl("http://127.0.0.1:5000/currencies"));
    }
    else {
        requestUrl.setUrl(QUrl("http://127.0.0.1:5000/stocks"));
    }

    requestUrl.setRawHeader(QByteArray("x-access-token"), accessToken->toUtf8());
    requestUrl.setHeader(QNetworkRequest::ContentTypeHeader,
                      QVariant("application/x-www-form-urlencoded"));

    QNetworkReply *recordsReply = this->connectionManager->get(requestUrl);

    /* Wait for the server response */
    QEventLoop connectionLoop;
    connect(recordsReply, &QNetworkReply::finished,
            &connectionLoop, &QEventLoop::quit);
    connectionLoop.exec();

    /* In case we get an error from the server */
    if (recordsReply->error()) {
        if (recordsReply->error() == QNetworkReply::ConnectionRefusedError) {
            qDebug() << "There you should implement displaying server/invalid token error!";
        }
        else {
            QJsonDocument recordsReplyError = QJsonDocument::fromJson(
                        recordsReply->readAll());
            qDebug() << recordsReplyError["message"].toString();
            recordsReply->deleteLater();
        }
        return;
    }

    QWidget *recordParentWidget = qobject_cast<QWidget*>(QObject::sender()->parent());
    QHBoxLayout *recordParentLayout = qobject_cast<QHBoxLayout*>(recordParentWidget->layout());

    int recordId = 0;
    QLabel *recordName = qobject_cast<QLabel*>(recordParentLayout->itemAt(0)->widget());
    QLabel *recordPrice = qobject_cast<QLabel*>(recordParentLayout->itemAt(1)->widget());
    QLineEdit *recordQuantity = qobject_cast<QLineEdit*>(recordParentLayout->itemAt(2)->widget());

    QString name = recordName->text();
    double buyPrice = recordPrice->text().toDouble();
    double buyQuantity = recordQuantity->text().toDouble();

    QJsonDocument recordsReplyJson = QJsonDocument::fromJson(recordsReply->readAll());
    QJsonArray recordsArray = recordsReplyJson.array();
    for (int i = 0; i < recordsArray.size(); ++i) {
        if (recordsArray.at(i)["name"].toString() == name) {
            recordId = recordsArray.at(i)["id"].toDouble();
            break;
        }
    }
    recordsReply->deleteLater();

    if (type == recordType::cryptoRecord) {
        requestUrl.setUrl(QUrl("http://127.0.0.1:5000/cryptocurrencies/buy"));
    }
    else if (type == recordType::currencyRecord) {
        requestUrl.setUrl(QUrl("http://127.0.0.1:5000/currencies/buy"));
    }
    else {
        requestUrl.setUrl(QUrl("http://127.0.0.1:5000/stocks/buy"));
    }

    QUrlQuery query;
    query.addQueryItem("record_id", QString::number(recordId));
    query.addQueryItem("price", QString::number(buyPrice, 'g', 12));
    query.addQueryItem("quantity", QString::number(buyQuantity, 'g', 12));

    QByteArray postData;
    postData = query.toString(QUrl::FullyEncoded).toUtf8();

    double transactionCost = buyQuantity * buyPrice;
    QString balanceWithoutDolarSign = accountBalance->text().left(accountBalance->text().length() - 2);
    if (transactionCost > balanceWithoutDolarSign.toDouble()) {
        QMessageBox insufficientBalanceBox;
        insufficientBalanceBox.setText("Insufficient account balance: Transaction cost: " +
                                                  QString::number(transactionCost, 'f', 2) + " $");
        insufficientBalanceBox.setStandardButtons(QMessageBox::Ok);
        insufficientBalanceBox.setDefaultButton(QMessageBox::Ok);
        insufficientBalanceBox.setIcon(QMessageBox::Critical);
        insufficientBalanceBox.exec();
        return;
    }
    if (transactionCost < 0.01) {
        QMessageBox wrongQuantityBox;
        wrongQuantityBox.setText("Plese insert correct quantity! Minimum transaction value is 0.01$");
        wrongQuantityBox.setStandardButtons(QMessageBox::Ok);
        wrongQuantityBox.setDefaultButton(QMessageBox::Ok);
        wrongQuantityBox.setIcon(QMessageBox::Critical);
        wrongQuantityBox.exec();
        return;
    }

    QMessageBox confirmTransactionBox;
    confirmTransactionBox.setText("Are you sure you want to buy " +
                                  QString::number(buyQuantity, 'g', 12) +
                                  " " + name +
                                  " for " + QString::number(buyPrice, 'g', 12) + "$ each?");
    /* Check if buying ammount is bigger than 0! */
    /* For example buying 0.0001 TRON is free! */
    confirmTransactionBox.setInformativeText("That will cost you: " + QString::number(transactionCost, 'f', 2) + "$");
    confirmTransactionBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    confirmTransactionBox.setDefaultButton(QMessageBox::Yes);
    confirmTransactionBox.setIcon(QMessageBox::Question);
    int userDecision = confirmTransactionBox.exec();

    if (userDecision == QMessageBox::No) {
        return;
    }
    recordQuantity->setText("");

    QNetworkReply *buyReply = this->connectionManager->post(requestUrl, postData);

    /* Wait for the server response */
    connect(buyReply, &QNetworkReply::finished,
            &connectionLoop, &QEventLoop::quit);
    connectionLoop.exec();

    /* In case we get an error from the server */
    if (buyReply->error()) {
        if (buyReply->error() == QNetworkReply::ConnectionRefusedError) {
            qDebug() << "There you should implement displaying server/invalid token error!";
        }
        else {
            QJsonDocument recordsReplyError = QJsonDocument::fromJson(
                        buyReply->readAll());
            qDebug() << recordsReplyError["message"].toString();
            buyReply->deleteLater();
        }
        return;
    }

    qDebug() << buyReply->readAll();
    buyReply->deleteLater();

    /* Update balance label */
    accountBalance->setText(QString::number(balanceWithoutDolarSign.toDouble() - transactionCost, 'f', 2) + " $");
}

void BuyWidget::onCryptocurrenciesTabClick()
{
    ScrollWidget::onCryptocurrenciesTabClick();
    obtainServerRecords(recordType::cryptoRecord);
}

void BuyWidget::onCurrenciesTabClick()
{
    ScrollWidget::onCurrenciesTabClick();
    obtainServerRecords(recordType::currencyRecord);
}

void BuyWidget::onStocksTabClick()
{
    ScrollWidget::onStocksTabClick();
    obtainServerRecords(recordType::stockRecord);
}

void BuyWidget::buyCrypto()
{
    buyRecord(recordType::cryptoRecord);
}

void BuyWidget::buyCurrency()
{
    buyRecord(recordType::currencyRecord);
}

void BuyWidget::buyStock()
{
    buyRecord(recordType::stockRecord);
}

void BuyWidget::onBuyButtonClicked()
{
    clearScrollLayout();
    setActiveButton(recordType::cryptoRecord);
    obtainServerRecords(recordType::cryptoRecord);
}

void BuyWidget::setUserBalance()
{
    QNetworkRequest balanceUrl(QUrl("http://127.0.0.1:5000/balance"));
    balanceUrl.setRawHeader(QByteArray("x-access-token"), accessToken->toUtf8());
    balanceUrl.setHeader(QNetworkRequest::ContentTypeHeader,
                      QVariant("application/x-www-form-urlencoded"));

    QNetworkReply *balanceReply = this->connectionManager->get(balanceUrl);

    /* Wait for the server response */
    QEventLoop connectionLoop;
    connect(balanceReply, &QNetworkReply::finished,
            &connectionLoop, &QEventLoop::quit);
    connectionLoop.exec();

    /* In case we get an error from the server */
    if (balanceReply->error()) {
        if (balanceReply->error() == QNetworkReply::ConnectionRefusedError) {
            qDebug() << "There you should implement displaying server/invalid token error!";
        }
        else {
            QJsonDocument replyError = QJsonDocument::fromJson(
                        balanceReply->readAll());
            qDebug() << replyError["message"].toString();
            balanceReply->deleteLater();
        }
        return;
    }
    QJsonDocument balanceReplyJson = QJsonDocument::fromJson(balanceReply->readAll());
    accountBalance->setText(QString::number(balanceReplyJson.object()["balance"].toDouble(), 'f', 2) + " $");

    balanceReply->deleteLater();
}
