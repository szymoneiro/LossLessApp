#ifndef SELLWIDGET_H
#define SELLWIDGET_H

#include <QtWidgets>
#include <QNetworkAccessManager>
#include "../headers/logo_and_side_bar_widget.h"

class SellWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SellWidget(QWidget *parent = nullptr);
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
        "Sell price",
        "Quantity"
    };
    QPushButton *tabButtons[3];
    QStringList buttonsNames = {
        "Cryptocurrencies",
        "Currencies",
        "Stocks"
    };

    LogoAndSideBarWidget *sidebarWidget;
    QLabel *accountBalance;

    QNetworkAccessManager *connectionManager;
    QString *accessToken;

    void scrollAreaCreate();
    void buttonsCreate();
    void obtainUserPurchases(recordType type);
    void sellRecord(recordType type);
    void setActiveButton(recordType type);
    void clearScrollLayout();

private slots:
    void sellCrypto();
    void sellCurrency();
    void sellStock();
    void sellButtonClicked();
    void onCryptocurrenciesTabClick();
    void onCurrenciesTabClick();
    void onStocksTabClick();
};

#endif // SELLWIDGET_H
