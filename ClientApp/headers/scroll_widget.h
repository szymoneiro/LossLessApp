#ifndef SCROLL_WIDGET_H
#define SCROLL_WIDGET_H

#include <QtWidgets>
#include <QNetworkAccessManager>
#include "logo_and_side_bar_widget.h"

class ScrollWidget : public QWidget
{
    Q_OBJECT
protected:
    enum class recordType {
        cryptoRecord = 1,
        currencyRecord = 2,
        stockRecord = 3
    };

    QScrollArea *scrollArea;
    QWidget *scrollAreaWidget;
    QVBoxLayout *scrollAreaWidgetLayout;

    QPushButton *tabButtons[3];
    QStringList tabButtonsNames = {
        "Cryptocurrencies",
        "Currencies",
        "Stocks"
    };

    LogoAndSideBarWidget *sideBarWidget;

    QNetworkAccessManager *connectionManager;
    QString *accessToken;

protected slots:
    void onCryptocurrenciesTabClick();
    void onCurrenciesTabClick();
    void onStocksTabClick();

public:
    explicit ScrollWidget(QWidget *parent = nullptr);
    void scrollAreaCreate();
    void createTabButtons();
    void clearScrollLayout();
    void setActiveButton(recordType type);
};
#endif // SCROLL_WIDGET_H
