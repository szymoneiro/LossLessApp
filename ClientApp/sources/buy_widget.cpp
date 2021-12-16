#include "../headers/buy_widget.h"
#include "../headers/top_bar_and_stacked_widget.h"
#include "../headers/main_widget.h"
#include <QNetworkReply>

BuyWidget::BuyWidget(QWidget *parent) : QWidget(parent)
{
    this->setFixedSize(1200, 640);
    this->setStyleSheet("background-color: #FAFAFA");
    connectionManager = qobject_cast<TopBarAndStackedWidget*>(parent->parent())->getNetworkManager();
    accessToken = &qobject_cast<TopBarAndStackedWidget*>(this->parent()->parent())->x_access_token;

    buttonsCreate();
    scrollAreaCreate();

    /* Create connection between buying widget and sidebar which emit signal to switch between widgets. */
    MainWidget *parentWidget = qobject_cast<MainWidget*>(this->parent()->parent()->parent());
    QHBoxLayout *parentLayout = qobject_cast<QHBoxLayout*>(parentWidget->layout());
    sidebarWidget = qobject_cast<LogoAndSideBarWidget*>(parentLayout->itemAt(0)->widget());

    connect(sidebarWidget, SIGNAL(buyPageClicked()),
            this, SLOT(buyButtonClicked()));

    connect(tabButtons[0], &QPushButton::clicked,
            this, &BuyWidget::onCryptocurrenciesTabClick);
    connect(tabButtons[1], &QPushButton::clicked,
            this, &BuyWidget::onCurrenciesTabClick);
    connect(tabButtons[2], &QPushButton::clicked,
            this, &BuyWidget::onStocksTabClick);
}

void BuyWidget::scrollAreaCreate()
{
    scrollArea = new QScrollArea(this);
    scrollWidget = new QWidget(scrollArea);
    scrollLayout = new QVBoxLayout(scrollWidget);

    scrollLayout->setSpacing(8);
    scrollLayout->setContentsMargins(12, 61, 24, 24);

    scrollWidget->setLayout(scrollLayout);
    scrollWidget->setStyleSheet("background-color: #4C3099");

    const int label_width = 101;
    const int label_height = 28;
    const int label_spacing = 256;
    for (int i = 0; i < 3; ++i) {
        scrollAreaLabels[i] = new QLabel(scrollLabelsText[i], scrollWidget);
        scrollAreaLabels[i]->setStyleSheet("background-color: none;"
                                           "font-size: 24px;"
                                           "color: #FFFFFF;"
                                           "font-weight: bold");
        scrollAreaLabels[i]->setFont(QFont("Lato"));
        scrollAreaLabels[i]->setGeometry(140 + i * (label_spacing + label_width),
                                         17,
                                         label_width, label_height);
        scrollAreaLabels[i]->setAlignment(Qt::AlignCenter);
    }

    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(scrollWidget);
    scrollArea->setGeometry(22, 81, 1155, 511);
    scrollArea->setStyleSheet("border-radius: 10px;"
                              "border: 3px solid #000000");
}

void BuyWidget::buttonsCreate()
{
    const int button_width = 150;
    const int button_height = 46;
    const int spacing = 16;

    QStringList buttonsColours = {
        "00FFA3;",
        "C8C8C8;",
        "C8C8C8;"
    };

    for (int i = 0; i < 3; ++i) {
        tabButtons[i] = new QPushButton(buttonsNames[i], this);
        tabButtons[i]->setStyleSheet("background-color: #" + buttonsColours[i] +
                                     "border-radius: 10px;"
                                     "border: 3px solid #000000;"
                                     "font-weight: bold;"
                                     "font-size: 14px");
        tabButtons[i]->setGeometry(46 + i * (button_width + spacing),
                                   48,
                                   button_width,
                                   button_height);
        tabButtons[i]->setFont(QFont("Lato"));
    }
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
        recordQuantity->setValidator(new QDoubleValidator(0, double(1'000'000'000), 17, recordQuantity));

        QPushButton *buyButton = new QPushButton(currentRecord);
        buyButton->setFixedSize(36, 36);
        buyButton->setStyleSheet(recordStyleSheet);
        QDir iconDir;
        iconDir.setPath("../ClientApp/icons");
        QString icon_path = iconDir.absolutePath() + "/" + "shop_icon";
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

        scrollLayout->addWidget(currentRecord);

        currentRecordLayout->addWidget(recordName);
        currentRecordLayout->addWidget(recordPrice);
        currentRecordLayout->addWidget(recordQuantity);
        currentRecordLayout->addWidget(buyButton);
    }

    recordsReply->deleteLater();
}

void BuyWidget::buyRecord(recordType type)
{
    QWidget *recordParentWidget = qobject_cast<QWidget*>(QObject::sender()->parent());
    QHBoxLayout *recordParentLayout = qobject_cast<QHBoxLayout*>(recordParentWidget->layout());

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

    /* We travel from BuyWidget->StackedWidget->TopBarAndStackedWidget */
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
    query.addQueryItem("buy_price", QString::number(buyPrice, 'g', 12));
    query.addQueryItem("quantity", QString::number(buyQuantity, 'g', 12));

    QByteArray postData;
    postData = query.toString(QUrl::FullyEncoded).toUtf8();

    QMessageBox confirmTransactionBox;
    confirmTransactionBox.setText("Are you sure you want to buy " +
                                  QString::number(buyQuantity, 'g', 12) +
                                  " " + name +
                                  " for " + QString::number(buyPrice, 'g', 12) + "$ each?");
    /* Check if buying ammount is bigger than 0! */
    /* For example buying 0.0001 TRON is free! */
    confirmTransactionBox.setInformativeText("That will cost you: " + QString::number(buyQuantity * buyPrice, 'f', 2) + "$");
    confirmTransactionBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    confirmTransactionBox.setDefaultButton(QMessageBox::Yes);
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
}

void BuyWidget::setActiveButton(recordType type)
{
    QStringList colours;
    if (type == recordType::cryptoRecord) {
        colours = {
            "00FFA3;",
            "C8C8C8;",
            "C8C8C8;"
        };
    }
    else if (type == recordType::currencyRecord) {
        colours = {
            "C8C8C8;",
            "00FFA3;",
            "C8C8C8;"
        };
    }
    else {
        colours = {
            "C8C8C8;",
            "C8C8C8;",
            "00FFA3;"
        };
    }
    for (int i = 0; i < 3; ++i) {
        tabButtons[i]->setStyleSheet("background-color: #" + colours[i] +
                                     "border-radius: 10px;"
                                     "border: 3px solid #000000;"
                                     "font-weight: bold;"
                                     "font-size: 14px");
    }
}

void BuyWidget::clearScrollLayout()
{
    QLayoutItem *child;
    QVBoxLayout *layoutToClear = this->scrollLayout;
    while ((child = layoutToClear->takeAt(0)) != 0) {
        delete child->widget();
        delete child;
    }
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

void BuyWidget::buyButtonClicked()
{
    clearScrollLayout();
    setActiveButton(recordType::cryptoRecord);
    obtainServerRecords(recordType::cryptoRecord);
}

void BuyWidget::onCryptocurrenciesTabClick()
{
    clearScrollLayout();
    setActiveButton(recordType::cryptoRecord);
    obtainServerRecords(recordType::cryptoRecord);
}

void BuyWidget::onCurrenciesTabClick()
{
    clearScrollLayout();
    setActiveButton(recordType::currencyRecord);
    obtainServerRecords(recordType::currencyRecord);
}

void BuyWidget::onStocksTabClick()
{
    clearScrollLayout();
    setActiveButton(recordType::stockRecord);
    obtainServerRecords(recordType::stockRecord);
}

