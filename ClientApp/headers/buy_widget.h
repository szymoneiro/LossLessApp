#ifndef BUY_WIDGET_H
#define BUY_WIDGET_H

#include "scroll_widget.h"
#include "sign_in_widget.h"

class BuyWidget : public ScrollWidget
{
    Q_OBJECT
private:
    QLabel *scrollAreaLabels[3];
    QStringList scrollLabelsText = {
        "Name",
        "Buy price",
        "Quantity"
    };

    SignInWidget *signInWidget;
    QLabel *accountBalance;

private slots:
    void onCryptocurrenciesTabClick();
    void onCurrenciesTabClick();
    void onStocksTabClick();

    void buyCrypto();
    void buyCurrency();
    void buyStock();

    void onBuyButtonClicked();
    void setUserBalance();

public:
    explicit BuyWidget(QWidget *parent = nullptr);
    void scrollAreaCreate();
    void obtainServerRecords(recordType type);
    void buyRecord(recordType type);
};

#endif // BUY_WIDGET_H
