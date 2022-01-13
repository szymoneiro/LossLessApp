#include "../headers/home_page_widget.h"
#include "../headers/sign_in_widget.h"
#include "../headers/top_bar_and_stacked_widget.h"
#include "../headers/main_widget.h"
#include <QNetworkReply>

HomePageWidget::HomePageWidget(QWidget *parent) : ScrollWidget(parent)
{
    scrollAreaCreate();

    connect(sideBarWidget, SIGNAL(homePageClicked()),
            this, SLOT(onCryptocurrenciesTabClick()));

    connect(tabButtons[0], &QPushButton::clicked,
            this, &HomePageWidget::onCryptocurrenciesTabClick);
    connect(tabButtons[1], &QPushButton::clicked,
            this, &HomePageWidget::onCurrenciesTabClick);
    connect(tabButtons[2], &QPushButton::clicked,
            this, &HomePageWidget::onStocksTabClick);
}

void HomePageWidget::scrollAreaCreate()
{
    scrollAreaWidgetLayout->setSpacing(12);
    scrollAreaWidgetLayout->setContentsMargins(72, 45, 136, 44);

    scrollAreaWidget->setLayout(scrollAreaWidgetLayout);
    scrollAreaWidget->setStyleSheet("background-color: #4C3099;"
                                    "border: 0px");

    QGraphicsDropShadowEffect *scrollAreaShadow = new QGraphicsDropShadowEffect(scrollAreaWidget);
    scrollAreaShadow->setBlurRadius(2);
    scrollAreaShadow->setColor(QColor(0, 0, 0, 64));
    scrollAreaShadow->setOffset(2, 5);
    scrollArea->setGraphicsEffect(scrollAreaShadow);

    const int label_width = 198;
    const int label_height = 30;
    const int label_spacing = 32;
    for (int i = 0; i < 4; ++i) {
        scrollAreaLabels[i] = new QLabel(scrollLabelsText[i], scrollAreaWidget);
        scrollAreaLabels[i]->setStyleSheet("background-color: none;"
                                           "font-size: 14px;"
                                           "color: #FFFFFF;"
                                           "border: 0px");
        scrollAreaLabels[i]->setFont(QFont("Lato"));
        scrollAreaLabels[i]->setGeometry(100 + i * (label_spacing + label_width),
                                         8,
                                         label_width,
                                         label_height);
        scrollAreaLabels[i]->setAlignment(Qt::AlignCenter);
    }

    scrollArea->setGeometry(20, 81, 1160, 511);
}

void HomePageWidget::obtainUserRecords(recordType type)
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
    QVector<QJsonObject> records;
    QVector<QString> recordsIds;
    while (recordsReplyJson[index] != QJsonValue::Undefined) {
        records.append(recordsReplyJson[index].toObject());
        int record_id = recordsReplyJson[index].toObject()["record_id"].toInt();

        QString record_id_string = QString::number(record_id);
        if (!recordsIds.contains(record_id_string)) {
            recordsIds.append(record_id_string);
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

    QMap<QString, QString> idNameMap;
    QMap<QString, double> idCurrentPriceMap;
    QVectorIterator<QString> ids(recordsIds);
    QJsonDocument recordsValuesJson = QJsonDocument::fromJson(recordsValuesReply->readAll());
    QJsonArray recordsArray = recordsValuesJson.array();
    while (ids.hasNext()) {
        for (int i = 0; i < recordsArray.size(); ++i) {
            if (ids.peekNext().toInt() == recordsArray.at(i)["id"].toInt()) {
                idNameMap[ids.peekNext()] = recordsArray.at(i)["name"].toString();
                idCurrentPriceMap[ids.next()] = recordsArray.at(i)["value"].toDouble();
                break;
            }
        }
    }
        recordsValuesReply->deleteLater();

    QVectorIterator<QJsonObject> j(records);
    while (j.hasNext()) {
        QWidget *currentRecord = new QWidget(this);
        QHBoxLayout *currentRecordLayout = new QHBoxLayout(currentRecord);
        currentRecordLayout->setSpacing(32);
        currentRecordLayout->setContentsMargins(32, 10, 32, 10);

        currentRecord->setStyleSheet("background-color: #C4C4C4;"
                                     "border-radius: 10px;"
                                     "border: 2px solid #000000");
        currentRecord->setFixedSize(952, 50);
        QGraphicsDropShadowEffect *currentRecordShadow = new QGraphicsDropShadowEffect(currentRecord);
        currentRecordShadow->setBlurRadius(2);
        currentRecordShadow->setColor(QColor(0, 0, 0, 64));
        currentRecordShadow->setOffset(2, 5);
        currentRecord->setGraphicsEffect(currentRecordShadow);

        /* 1. Record name
         * 2. Record quantity
         * 3. Record Buy price
         * 4. Record current_price */

        QString recordValueStylesheet = "background-color: #FFFFFF;"
                                        "border-radius: 10px;"
                                        "border: 2px solid #000000;"
                                        "font-size: 14px;"
                                        "font-weight: bold";

        QString buyRecordStylesheet = "background-color: #00FFA3;"
                                      "border-radius: 10px;"
                                      "border: 2px solid #000000;"
                                      "font-size: 14px;"
                                      "font-weight: bold";

        QString sellRecordStylesheet = "background-color: #FF004C;"
                                       "border-radius: 10px;"
                                       "border: 2px solid #000000;"
                                       "font-size: 14px;"
                                       "font-weight: bold";

        QString currentRecordID = QString::number(j.peekNext()["record_id"].toDouble());

        QLabel *recordName = new QLabel(idNameMap[currentRecordID], currentRecord);
        recordName->setStyleSheet(recordValueStylesheet);
        recordName->setFont(QFont("Lato"));
        recordName->setAlignment(Qt::AlignCenter);

        double quantity = j.peekNext()["quantity"].toDouble();
        QLabel *recordQuantity = new QLabel(QString::number(quantity, 'g', 12), currentRecord);
        if (quantity > 0.0) {
            recordQuantity->setStyleSheet(buyRecordStylesheet);
        }
        else {
           recordQuantity->setStyleSheet(sellRecordStylesheet);
        }

        recordQuantity->setFont(QFont("Lato"));
        recordQuantity->setAlignment(Qt::AlignCenter);

        QLabel *recordBuyPrice = new QLabel(QString::number(j.next()["priceg"].toDouble(), 'g', 12), currentRecord);
        recordBuyPrice->setStyleSheet(recordValueStylesheet);
        recordBuyPrice->setFont(QFont("Lato"));
        recordBuyPrice->setAlignment(Qt::AlignCenter);

        QLabel *recordCurrentPrice = new QLabel(QString::number(idCurrentPriceMap[currentRecordID], 'g', 12), currentRecord);
        recordCurrentPrice->setStyleSheet(recordValueStylesheet);
        recordCurrentPrice->setFont(QFont("Lato"));
        recordCurrentPrice->setAlignment(Qt::AlignCenter);

        scrollAreaWidgetLayout->addWidget(currentRecord);

        currentRecordLayout->addWidget(recordName);
        currentRecordLayout->addWidget(recordQuantity);
        currentRecordLayout->addWidget(recordBuyPrice);
        currentRecordLayout->addWidget(recordCurrentPrice);
    }
    recordsReply->deleteLater();
}

void HomePageWidget::onUserLogin()
{
    obtainUserRecords(recordType::cryptoRecord);
}

void HomePageWidget::onCryptocurrenciesTabClick()
{
    ScrollWidget::onCryptocurrenciesTabClick();
    scrollAreaLabels[0]->setText("Cryptocurrency name");
    obtainUserRecords(recordType::cryptoRecord);
}

void HomePageWidget::onCurrenciesTabClick()
{
    ScrollWidget::onCurrenciesTabClick();
    scrollAreaLabels[0]->setText("Currency name");
    obtainUserRecords(recordType::currencyRecord);
}

void HomePageWidget::onStocksTabClick()
{
    ScrollWidget::onStocksTabClick();
    scrollAreaLabels[0]->setText("Stock name");
    obtainUserRecords(recordType::stockRecord);
}
