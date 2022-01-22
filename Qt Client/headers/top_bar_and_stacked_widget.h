#ifndef TOP_BAR_AND_STACKED_WIDGET_H
#define TOP_BAR_AND_STACKED_WIDGET_H

#include <QtWidgets>
#include <QNetworkAccessManager>
#include "sign_up_widget.h"
#include "sign_in_widget.h"
#include "home_page_widget.h"
#include "buy_widget.h"
#include "sell_widget.h"
#include "logo_and_side_bar_widget.h"

class TopBarAndStackedWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TopBarAndStackedWidget(QWidget *parent = nullptr);
    QString x_access_token = "EXAMPLE-TOKEN";
    static QNetworkAccessManager* getNetworkManager();
    QLabel *accountBalance;

private slots:
    void onExitButtonClicked();
    void setSignUpPage();
    void setSignInPage();
    void setHomePage();
    void setBuyPage();
    void setSellPage();

private:
    QDir iconsDir;
    QStringList iconsNames = {
        "settings.svg",
        "profile.svg",
        "exit.svg"
    };
    QStringList buttonColours = {
        "#4C3099",
        "#00FFA3",
        "#FF004C"
    };
    enum { button_size = 56, icon_size = 30 };

    /* Layouts */
    QVBoxLayout *rightWidgetLayout;
    QHBoxLayout *topBarLayout;

    /* Top bar with 3 buttons and application body */
    QWidget *topBarWidget;
    QStackedWidget *stackedWidget;

    /* stackedWidget's widgets */
    SignInWidget *signInPage;
    SignUpWidget *signUpPage;
    HomePageWidget *homePage;
    BuyWidget *buyPage;
    SellWidget *sellPage;

    /* Buttons SETTINGS/USER/EXIT */
    QPushButton *topBarButtons[3];

    /* Sidebar object to connect to navigation */
    LogoAndSideBarWidget *sideBarWidget;

    void createLayouts();
    void createStackedWidget();
    void createButtons();
    void createBalanceLabel();
    void createShadows();
};

#endif // TOP_BAR_AND_STACKED_WIDGET_H
