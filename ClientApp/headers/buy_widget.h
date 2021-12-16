#ifndef BUY_WIDGET_H
#define BUY_WIDGET_H

#include <QtWidgets>
#include <QNetworkAccessManager>

class BuyWidget : public QWidget
{
    Q_OBJECT
public:
    explicit BuyWidget(QWidget *parent = nullptr);
private:
    enum class recordType {
        cryptoRecord = 1,
        currencyRecord = 2,
        stockRecord = 3
    };

    QScrollArea *scrollArea;
    QWidget *scrollWidget;
    QVBoxLayout *scrollLayout;

    QLabel *scrollAreaLabels[3];
    QStringList scrollLabelsText = {
        "Name",
        "Buy price",
        "Quantity"
    };
    QPushButton *tabButtons[3];
    QStringList buttonsNames = {
        "Cryptocurrencies",
        "Currencies",
        "Stocks"
    };
    QString *accessToken;


    QNetworkAccessManager *connectionManager;

    void scrollAreaCreate();
    void buttonsCreate();
    void obtainServerRecords(recordType type);

private slots:
    void buyCrypto();
};

#endif // BUY_WIDGET_H
