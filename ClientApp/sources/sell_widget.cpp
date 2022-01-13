#include "../headers/buy_widget.h"
#include "../headers/top_bar_and_stacked_widget.h"
#include "../headers/main_widget.h"
#include <QNetworkReply>

SellWidget::SellWidget(QWidget *parent) : ScrollWidget(parent)
{
    accountBalance = qobject_cast<TopBarAndStackedWidget*>(this->parent()->parent())->accountBalance;

    scrollAreaCreate();

    connect(sideBarWidget, SIGNAL(sellPageClicked()),
            this, SLOT(onSellButtonClicked()));

    connect(tabButtons[0], &QPushButton::clicked,
            this, &SellWidget::onCryptocurrenciesTabClick);
    connect(tabButtons[1], &QPushButton::clicked,
            this, &SellWidget::onCurrenciesTabClick);
    connect(tabButtons[2], &QPushButton::clicked,
            this, &SellWidget::onStocksTabClick);
}

void SellWidget::scrollAreaCreate()
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
                                         label_width, label_height);
        scrollAreaLabels[i]->setAlignment(Qt::AlignCenter);
    }

    scrollArea->setGeometry(22, 81, 1155, 511);
}

void SellWidget::obtainUserPurchases(recordType type)
{
    QNetworkRequest recordsRequest;
    if (type == recordType::cryptoRecord) {
        recordsRequest.setUrl(QUrl("http://127.0.0.1:5000/cryptocurrencies/records"));
    }
    else if (type == recordType::currencyRecord) {
        recordsRequest.setUrl(QUrl("http://127.0.0.1:5000/currencies/records"));
    }
    else {
        recordsRequest.setUrl(QUrl("http://127.0.0.1:5000/stocks/records"));
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
    int index = 0;
    QVector<int> uniqueRecordsIds;
    QMap<int, double> recordsQuantities;
    while (recordsReplyJson[index] != QJsonValue::Undefined) {
        int record_id = recordsReplyJson[index].toObject()["record_id"].toInt();
        recordsQuantities[record_id] = recordsQuantities[record_id] + recordsReplyJson[index].toObject()["quantity"].toDouble();
        if (!uniqueRecordsIds.contains(record_id)) {
            uniqueRecordsIds.append(record_id);
        }
        index++;
    }

    QNetworkRequest recordsValuesRequest;
    if (type == recordType::cryptoRecord) {
        recordsValuesRequest.setUrl(QUrl("http://127.0.0.1:5000/cryptocurrencies"));
    }
    else if (type == recordType::currencyRecord) {
         recordsValuesRequest.setUrl(QUrl("http://127.0.0.1:5000/currencies"));
    }
    else {
         recordsValuesRequest.setUrl(QUrl("http://127.0.0.1:5000/stocks"));
    }

    recordsValuesRequest.setRawHeader(QByteArray("x-access-token"), accessToken->toUtf8());
    recordsValuesRequest.setHeader(QNetworkRequest::ContentTypeHeader,
                                  QVariant("application/x-www-form-urlencoded"));
    QNetworkReply *recordsValuesReply = this->connectionManager->get(recordsValuesRequest);

    /* Wait for the server response */
    QEventLoop singleConnectionLoop;
    connect(recordsValuesReply, &QNetworkReply::finished,
            &singleConnectionLoop, &QEventLoop::quit);
    singleConnectionLoop.exec();

    /* In case we get an error from the server */
    if (recordsValuesReply->error()) {
        if (recordsValuesReply->error() == QNetworkReply::ConnectionRefusedError) {
            qDebug() << "There you should implement displaying server/invalid token error!";
        }
        else {
            QJsonDocument recordsReplyError = QJsonDocument::fromJson(
                        recordsValuesReply->readAll());
            qDebug() << recordsReplyError["message"].toString();
            recordsValuesReply->deleteLater();
        }
        return;
    }

    QJsonDocument recordsValuesJson = QJsonDocument::fromJson(recordsValuesReply->readAll());
    QJsonArray recordsArray = recordsValuesJson.array();

    for (int i = 0; i < uniqueRecordsIds.size(); ++i) {

        /* Check if current record quantity is equal 0,
         * it's a case when user made some transactions, but now he has 0 quantity of certain record
         * so there's no point of displaying that record. */

        if (recordsQuantities[uniqueRecordsIds[i]] == 0.0) {
            continue;
        }

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

        double currentSellPrice = 0;
        QString currentRecordName = "BITICOIN_XD";
        for (int j = 0; j < recordsArray.size(); ++j) {
            if (recordsArray.at(j)["id"].toInt() == uniqueRecordsIds[i]) {
                currentSellPrice = recordsArray.at(j)["value"].toDouble();
                currentRecordName = recordsArray.at(j)["name"].toString();
                break;
            }
        }

        QLabel *recordName = new QLabel(currentRecordName, currentRecord);
        recordName->setFixedSize(357, 48);
        recordName->setStyleSheet("background-color: #EAEAEA;"
                                  "border: 3px solid #A7A7A7;"
                                  "font-size: 18px;"
                                  "font-weight: bold");
        recordName->setFont(QFont("Lato"));
        recordName->setAlignment(Qt::AlignCenter);


        QLabel *recordPrice = new QLabel(QString::number(currentSellPrice, 'g', 12), currentRecord);
        recordPrice->setFixedSize(357, 48);
        recordPrice->setStyleSheet(recordStyleSheet);
        recordPrice->setFont(QFont("Lato"));
        recordPrice->setAlignment(Qt::AlignCenter);

        QLineEdit *recordQuantity = new QLineEdit(QString::number(recordsQuantities[uniqueRecordsIds[i]], 'g', 12), currentRecord);
        recordQuantity->setFixedSize(357, 48);
        recordQuantity->setStyleSheet(recordStyleSheet);
        recordQuantity->setFont(QFont("Lato"));
        recordQuantity->setAlignment(Qt::AlignCenter);

        QDoubleValidator *inputValidator = new QDoubleValidator(0, recordsQuantities[uniqueRecordsIds[i]], 12, recordQuantity);
        inputValidator->setNotation(QDoubleValidator::StandardNotation);
        recordQuantity->setValidator(inputValidator);

        QPushButton *sellButton = new QPushButton(currentRecord);
        sellButton->setFixedSize(36, 36);
        sellButton->setStyleSheet(recordStyleSheet);
        QDir iconDir;
        iconDir.setPath("../ClientApp/icons");
        QString icon_path = iconDir.absolutePath() + "/" + "basket_icon";
        sellButton->setIcon(QIcon(icon_path));
        sellButton->setIconSize(QSize(22, 22));

        if (type == recordType::cryptoRecord) {
            connect(sellButton, &QPushButton::clicked,
                    this, &SellWidget::sellCrypto);
        }
        else if (type == recordType::currencyRecord) {
            connect(sellButton, &QPushButton::clicked,
                    this, &SellWidget::sellCurrency);
        }
        else {
            connect(sellButton, &QPushButton::clicked,
                    this, &SellWidget::sellStock);
        }

        scrollAreaWidgetLayout->addWidget(currentRecord);

        currentRecordLayout->addWidget(recordName);
        currentRecordLayout->addWidget(recordPrice);
        currentRecordLayout->addWidget(recordQuantity);
        currentRecordLayout->addWidget(sellButton);
    }

    recordsReply->deleteLater();
    recordsValuesReply->deleteLater();
}

void SellWidget::sellRecord(recordType type)
{
    QWidget *recordParentWidget = qobject_cast<QWidget*>(QObject::sender()->parent());
    QHBoxLayout *recordParentLayout = qobject_cast<QHBoxLayout*>(recordParentWidget->layout());

    QLabel *recordName = qobject_cast<QLabel*>(recordParentLayout->itemAt(0)->widget());
    QLabel *recordPrice = qobject_cast<QLabel*>(recordParentLayout->itemAt(1)->widget());
    QLineEdit *recordQuantity = qobject_cast<QLineEdit*>(recordParentLayout->itemAt(2)->widget());
    const QDoubleValidator *recordValidator = qobject_cast<const QDoubleValidator*>(recordQuantity->validator());

    int position;
    QString recordQuantityText = recordQuantity->text();
    if (recordValidator->validate(recordQuantityText, position) == QValidator::Intermediate || QValidator::Invalid) {
        QMessageBox wrongQuantityBox;
        wrongQuantityBox.setText("Plese insert correct quantity! Maximum quanity for sell is: " + QString::number(recordValidator->top(), 'g', 12));
        wrongQuantityBox.setStandardButtons(QMessageBox::Ok);
        wrongQuantityBox.setDefaultButton(QMessageBox::Ok);
        wrongQuantityBox.setIcon(QMessageBox::Critical);
        wrongQuantityBox.exec();
        return;
    }
    /* Get record ID by its name */
    QNetworkRequest recordsUrl;
    if (type == recordType::cryptoRecord) {
        recordsUrl.setUrl(QUrl("http://127.0.0.1:5000/cryptocurrencies"));
    }
    else if (type == recordType::currencyRecord) {
        recordsUrl.setUrl(QUrl("http://127.0.0.1:5000/currencies"));
    }
    else {
        recordsUrl.setUrl(QUrl("http://127.0.0.1:5000/stocks"));
    }

    recordsUrl.setRawHeader(QByteArray("x-access-token"), accessToken->toUtf8());
    recordsUrl.setHeader(QNetworkRequest::ContentTypeHeader,
                      QVariant("application/x-www-form-urlencoded"));

    QNetworkReply *recordsReply = this->connectionManager->get(recordsUrl);

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
    QString recordNameString = recordName->text();
    int recordId = 0;

    QJsonDocument recordsJson = QJsonDocument::fromJson(recordsReply->readAll());
    QJsonArray recordsArray = recordsJson.array();
    for (int i = 0; i < recordsArray.size(); ++i) {
        if (recordsArray.at(i)["name"].toString() == recordNameString) {
            recordId = recordsArray.at(i)["id"].toInt();
            break;
        }
    }

    recordsReply->deleteLater();

    /* Sell record */

    QNetworkRequest sellRequestUrl;
    if (type == recordType::cryptoRecord) {
        sellRequestUrl.setUrl(QUrl("http://127.0.0.1:5000/cryptocurrencies/sell"));
    }
    else if (type == recordType::currencyRecord) {
        sellRequestUrl.setUrl(QUrl("http://127.0.0.1:5000/currencies/sell"));
    }
    else {
        sellRequestUrl.setUrl(QUrl("http://127.0.0.1:5000/stocks/sell"));
    }

    sellRequestUrl.setRawHeader(QByteArray("x-access-token"), accessToken->toUtf8());
    sellRequestUrl.setHeader(QNetworkRequest::ContentTypeHeader,
                      QVariant("application/x-www-form-urlencoded"));

    QString recordPriceText = recordPrice->text();
    QUrlQuery query;
    query.addQueryItem("record_id", QString::number(recordId));
    query.addQueryItem("price", recordPriceText);
    query.addQueryItem("quantity", recordQuantityText);

    QByteArray postData;
    postData = query.toString(QUrl::FullyEncoded).toUtf8();

    double transactionCost = recordQuantityText.toDouble() * recordPriceText.toDouble();
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
    confirmTransactionBox.setText("Are you sure you want to sell " + recordQuantityText +
                                  " " + recordNameString +
                                  " for " + recordPriceText + "$ each?");

    confirmTransactionBox.setInformativeText("Your balance will increase by: " +
                                             QString::number(recordQuantityText.toDouble() * recordPriceText.toDouble(), 'f', 2) + "$");
    confirmTransactionBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    confirmTransactionBox.setDefaultButton(QMessageBox::Yes);
    confirmTransactionBox.setIcon(QMessageBox::Question);
    int userDecision = confirmTransactionBox.exec();

    if (userDecision == QMessageBox::No) {
        return;
    }

    QNetworkReply *sellReply = this->connectionManager->post(sellRequestUrl, postData);

    /* Wait for the server response */
    connect(sellReply, &QNetworkReply::finished,
            &connectionLoop, &QEventLoop::quit);
    connectionLoop.exec();

    /* In case we get an error from the server */
    if (sellReply->error()) {
        if (sellReply->error() == QNetworkReply::ConnectionRefusedError) {
            qDebug() << "There you should implement displaying server/invalid token error!";
        }
        else {
            QJsonDocument recordsReplyError = QJsonDocument::fromJson(
                        sellReply->readAll());
            qDebug() << recordsReplyError["message"].toString();
            sellReply->deleteLater();
        }
        return;
    }

    qDebug() << sellReply->readAll();
    sellReply->deleteLater();

    /* Update balance label */
    QString balanceWithoutDolarSign = accountBalance->text().left(accountBalance->text().length() - 2);
    accountBalance->setText(QString::number(balanceWithoutDolarSign.toDouble() + transactionCost, 'f', 2) + " $");


    if (type == recordType::cryptoRecord) {
        onCryptocurrenciesTabClick();
    }
    else if (type == recordType::currencyRecord) {
        onCurrenciesTabClick();
    }
    else {
        onStocksTabClick();
    }
}

void SellWidget::sellCrypto()
{
    sellRecord(recordType::cryptoRecord);
}

void SellWidget::sellCurrency()
{
    sellRecord(recordType::currencyRecord);
}

void SellWidget::sellStock()
{
    sellRecord(recordType::stockRecord);
}

void SellWidget::onSellButtonClicked()
{
    clearScrollLayout();
    setActiveButton(recordType::cryptoRecord);
    obtainUserPurchases(recordType::cryptoRecord);
}

void SellWidget::onCryptocurrenciesTabClick()
{
    ScrollWidget::onCryptocurrenciesTabClick();
    obtainUserPurchases(recordType::cryptoRecord);
}

void SellWidget::onCurrenciesTabClick()
{
    ScrollWidget::onCurrenciesTabClick();
    obtainUserPurchases(recordType::currencyRecord);
}

void SellWidget::onStocksTabClick()
{
    ScrollWidget::onStocksTabClick();
    obtainUserPurchases(recordType::stockRecord);
}

