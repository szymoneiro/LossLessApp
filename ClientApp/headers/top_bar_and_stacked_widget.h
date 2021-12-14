#ifndef TOP_BAR_AND_STACKED_WIDGET_H
#define TOP_BAR_AND_STACKED_WIDGET_H

#include <QtWidgets>
#include <QNetworkAccessManager>
#include "sign_up_widget.h"
#include "sign_in_widget.h"
#include "home_page_widget.h"
#include "buy_widget.h"

class TopBarAndStackedWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TopBarAndStackedWidget(QWidget *parent = nullptr);
    QString x_access_token = "EXAMPLE-TOKEN-jdksaldkjsalkda";
    static QNetworkAccessManager* getNetworkManager();

private slots:
    void onExitButtonClicked();
    void setSignUpPage();
    void setSignInPage();
    void setMainPage();

private:
    QDir iconsDir;
    QStringList iconsNames = {
        "settings_icon.png",
        "profile_icon.png",
        "exit_icon.png"
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

    /* Buttons SETTINGS/USER/EXIT */
    QPushButton *topBarButtons[3];

    void createLayouts();
    void createStackedWidget();
    void createButtons();

};

#endif // TOP_BAR_AND_STACKED_WIDGET_H
