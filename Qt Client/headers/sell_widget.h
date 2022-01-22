#ifndef SELLWIDGET_H
#define SELLWIDGET_H

#include "scroll_widget.h"

class SellWidget : public ScrollWidget
{
    Q_OBJECT
private:
    QLabel *scrollAreaLabels[3];
    QStringList scrollLabelsText = {
        "Name",
        "Sell price",
        "Quantity"
    };

    QLabel *accountBalance;

private slots:
    void onCryptocurrenciesTabClick();
    void onCurrenciesTabClick();
    void onStocksTabClick();

    void sellCrypto();
    void sellCurrency();
    void sellStock();

    void onSellButtonClicked();

public:
    explicit SellWidget(QWidget *parent = nullptr);
    void scrollAreaCreate();
    void obtainUserPurchases(recordType type);
    void sellRecord(recordType type);
};

#endif // SELLWIDGET_H
