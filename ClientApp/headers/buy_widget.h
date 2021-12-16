#ifndef BUY_WIDGET_H
#define BUY_WIDGET_H

#include <QtWidgets>
#include <QNetworkAccessManager>
#include "../headers/logo_and_side_bar_widget.h"
#include "../headers/sign_in_widget.h"

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

    LogoAndSideBarWidget *sidebarWidget;

    SignInWidget *signInWidget;
    QLabel *accountBalance;

    QNetworkAccessManager *connectionManager;
    QString *accessToken;

    void scrollAreaCreate();
    void buttonsCreate();
    void obtainServerRecords(recordType type);
    void buyRecord(recordType type);
    void setActiveButton(recordType type);
    void clearScrollLayout();

private slots:
    void buyCrypto();
    void buyCurrency();
    void buyStock();
    void buyButtonClicked();
    void onCryptocurrenciesTabClick();
    void onCurrenciesTabClick();
    void onStocksTabClick();
    void setUserBalance();
};

#endif // BUY_WIDGET_H
