#include "../headers/home_page_widget.h"
#include "../headers/sign_in_widget.h"
#include "../headers/top_bar_and_stacked_widget.h"
#include <QNetworkReply>

HomePageWidget::HomePageWidget(QWidget *parent) : QWidget(parent)
{
    this->setFixedSize(1200, 640);
    this->setStyleSheet("background-color: #FAFAFA");
    connectionManager = new QNetworkAccessManager(this);

    buttonsCreate();
    scrollAreaCreate();
}

void HomePageWidget::buttonsCreate()
{
    const int button_width = 150;
    const int button_height = 46;
    const int spacing = 16;
    QStringList colours = {
        "00FFA3;",
        "C8C8C8;",
        "C8C8C8;"
    };

    for (int i = 0; i < 3; ++i) {
        tabButtons[i] = new QPushButton(buttonsNames[i], this);
        tabButtons[i]->setStyleSheet("background-color: #" + colours[i] +
                                     "border-radius: 10px;"
                                     "border: 3px solid #000000;"
                                     "font-weight: bold;"
                                     "font-size: 14px");
        tabButtons[i]->setGeometry(28 + i * (button_width + spacing),
                                   48,
                                   button_width,
                                   button_height);
        tabButtons[i]->setFont(QFont("Lato"));
    }
}

void HomePageWidget::scrollAreaCreate()
{
    scrollArea = new QScrollArea(this);
    scrollWidget = new QWidget(scrollArea);
    scrollLayout = new QVBoxLayout(scrollWidget);

    scrollLayout->setSpacing(12);
    scrollLayout->setContentsMargins(72, 45, 136, 44);

    scrollWidget->setLayout(scrollLayout);
    scrollWidget->setStyleSheet("background-color: #4C3099");

    const int label_width = 198;
    const int label_height = 30;
    const int label_spacing = 32;
    for (int i = 0; i < 4; ++i) {
        scrollAreaLabels[i] = new QLabel(scrollLabelsText[i], scrollWidget);
        scrollAreaLabels[i]->setStyleSheet("background-color: none;"
                                           "font-size: 14px;"
                                           "color: #FFFFFF");
        scrollAreaLabels[i]->setFont(QFont("Lato"));
        scrollAreaLabels[i]->setGeometry(100 + i * (label_spacing + label_width),
                                         8,
                                         label_width, label_height);
        scrollAreaLabels[i]->setAlignment(Qt::AlignCenter);
    }

    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(scrollWidget);
    scrollArea->setGeometry(20, 81, 1160, 511);
    scrollArea->setStyleSheet("border-radius: 10px;"
                              "border: 3px solid #000000");
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

    /* We travel from HomePageWidget->StackedWidget->TopBarAndStackedWidget */
    QString accessToken = qobject_cast<TopBarAndStackedWidget*>(parent()->parent())->x_access_token;
    recordsRequest.setRawHeader(QByteArray("x-access-token"), accessToken.toUtf8());
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
    /* Warning below is known issue:
     * https://forum.qt.io/topic/123718/use-vector-instead-of-qlist/2 */
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

    QMap<QString, QString> idNameMap;
    QMap<QString, double> idCurrentPriceMap;

    /* 1. Send get request
     * 2. Wait for response
     * 3. Add both response values to QMap
     * 4. Create records and append them to dashboard */
    QNetworkRequest singleRecordRequest;
    QString baseUrl;
    if (type == recordType::cryptoRecord) {
        baseUrl = "http://127.0.0.1:5000/cryptocurrencies/";
    }
    else if (type == recordType::currencyRecord) {
         baseUrl = "http://127.0.0.1:5000/currencies/";
    }
    else {
         baseUrl = "http://127.0.0.1:5000/stocks/";
    }

    singleRecordRequest.setRawHeader(QByteArray("x-access-token"), accessToken.toUtf8());
    singleRecordRequest.setHeader(QNetworkRequest::ContentTypeHeader,
                                  QVariant("application/x-www-form-urlencoded"));
    QVectorIterator<QString> i(recordsIds);
    while (i.hasNext()){
       singleRecordRequest.setUrl(QUrl(baseUrl + i.peekNext()));
       QNetworkReply *singleRecordReply = this->connectionManager->get(singleRecordRequest);

       /* Wait for the server response */
       QEventLoop singleConnectionLoop;
       connect(singleRecordReply, &QNetworkReply::finished,
               &singleConnectionLoop, &QEventLoop::quit);
       singleConnectionLoop.exec();

       /* In case we get an error from the server */
       if (singleRecordReply->error()) {
           if (singleRecordReply->error() == QNetworkReply::ConnectionRefusedError) {
               qDebug() << "There you should implement displaying server/invalid token error!";
           }
           else {
               QJsonDocument recordsReplyError = QJsonDocument::fromJson(
                           singleRecordReply->readAll());
               qDebug() << recordsReplyError["message"].toString();
               singleRecordReply->deleteLater();
           }
           return;
       }

       QJsonDocument singleRecordReplyJson = QJsonDocument::fromJson(singleRecordReply->readAll());
       idNameMap[i.peekNext()] = singleRecordReplyJson["name"].toString();
       idCurrentPriceMap[i.next()] = singleRecordReplyJson["value"].toDouble();

       singleRecordReply->deleteLater();
    }

    QVectorIterator<QJsonObject> j(records);
    while (j.hasNext()) {
        QWidget *currentRecord = new QWidget(this);
        QHBoxLayout *currentRecordLayout = new QHBoxLayout(this);
        currentRecordLayout->setSpacing(32);
        currentRecordLayout->setContentsMargins(32, 10, 32, 10);

        currentRecord->setLayout(currentRecordLayout);
        currentRecord->setStyleSheet("background-color: #C4C4C4;"
                                     "border-radius: 10px;"
                                     "border: 2px solid #000000");
        currentRecord->setFixedSize(952, 50);

        /* 1. Record name
         * 2. Record quantity
         * 3. Record Buy price
         * 4. Record current_price */

        QString recordValueStylesheet = "background-color: #FFFFFF;"
                                        "border-radius: 10px;"
                                        "border: 2px solid #000000;"
                                        "font-size: 14px;"
                                        "font-weight: bold";

        QString currentRecordID = QString::number(j.peekNext()["record_id"].toDouble());

        QLabel *recordName = new QLabel(idNameMap[currentRecordID], currentRecord);
        recordName->setStyleSheet(recordValueStylesheet);
        recordName->setFont(QFont("Lato"));
        recordName->setAlignment(Qt::AlignCenter);

        QLabel *recordQuantity = new QLabel(QString::number(j.peekNext()["quantity"].toDouble(), 'g', 12), currentRecord);
        recordQuantity->setStyleSheet(recordValueStylesheet);
        recordQuantity->setFont(QFont("Lato"));
        recordQuantity->setAlignment(Qt::AlignCenter);

        QLabel *recordBuyPrice = new QLabel(QString::number(j.next()["buy_price"].toDouble(), 'g', 12), currentRecord);
        recordBuyPrice->setStyleSheet(recordValueStylesheet);
        recordBuyPrice->setFont(QFont("Lato"));
        recordBuyPrice->setAlignment(Qt::AlignCenter);

        QLabel *recordCurrentPrice = new QLabel(QString::number(idCurrentPriceMap[currentRecordID], 'g', 12), currentRecord);
        recordCurrentPrice->setStyleSheet(recordValueStylesheet);
        recordCurrentPrice->setFont(QFont("Lato"));
        recordCurrentPrice->setAlignment(Qt::AlignCenter);

        scrollLayout->addWidget(currentRecord);

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
    //obtainUserRecords(recordType::currencyRecord);
    //obtainUserRecords(recordType::stockRecord);
}
