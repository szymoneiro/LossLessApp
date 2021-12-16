#ifndef HOME_PAGE_WIDGET_H
#define HOME_PAGE_WIDGET_H

#include <QtWidgets>
#include <QNetworkAccessManager>
#include "../headers/logo_and_side_bar_widget.h"

class HomePageWidget : public QWidget
{
    Q_OBJECT
public:
    explicit HomePageWidget(QWidget *parent = nullptr);

private:
    enum class recordType {
        cryptoRecord = 1,
        currencyRecord = 2,
        stockRecord = 3
    };

    QPushButton *tabButtons[3];
    QStringList buttonsNames = {
        "Cryptocurrencies",
        "Currencies",
        "Stocks"
    };

    QScrollArea *scrollArea;
    QWidget *scrollWidget;
    QVBoxLayout *scrollLayout;

    QLabel *scrollAreaLabels[4];
    QStringList scrollLabelsText = {
        "Cryptocurrency name",
        "Quantity",
        "Buy price",
        "Current price"
    };
    LogoAndSideBarWidget *sidebarWidget;

    QNetworkAccessManager *connectionManager;
    QString *accessToken;

    void buttonsCreate();
    void scrollAreaCreate();
    void obtainUserRecords(recordType type);
    void clearScrollLayout();
    void setActiveButton(recordType type);
private slots:
    void onUserLogin();
    void onCryptocurrenciesTabClick();
    void onCurrenciesTabClick();
    void onStocksTabClick();
    void homePage();
};

#endif // HOME_PAGE_WIDGET_H
