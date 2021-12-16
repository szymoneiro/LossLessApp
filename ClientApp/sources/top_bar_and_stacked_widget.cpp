#include "../headers/top_bar_and_stacked_widget.h"
#include "../headers/main_widget.h"
#include <QNetworkReply>

Q_GLOBAL_STATIC(QNetworkAccessManager, appNetworkManager)

TopBarAndStackedWidget::TopBarAndStackedWidget(QWidget *parent) : QWidget(parent)
{
    createLayouts();
    createStackedWidget();
    createButtons();

    /* Connect functions to buttons */
    connect(topBarButtons[2], SIGNAL(clicked()), SLOT(onExitButtonClicked()));

    /* Add widgets to layouts */
    rightWidgetLayout->addWidget(topBarWidget);
    rightWidgetLayout->addWidget(stackedWidget);

    for (int i = 0; i < 3; ++i)
        topBarLayout->addWidget(topBarButtons[i]);

    connect(signUpPage, SIGNAL(signInClicked()), SLOT(setSignInPage()));

    connect(signInPage, SIGNAL(signUpClicked()), SLOT(setSignUpPage()));

    connect(signInPage, SIGNAL(userLoggedIn()), SLOT(setMainPage()));
    connect(signInPage, SIGNAL(userLoggedIn()),
            homePage, SLOT(onUserLogin()));
    connect(signInPage, SIGNAL(userLoggedIn()),
            buyPage, SLOT(onUserLogin()));

    MainWidget *parentWidget = qobject_cast<MainWidget*>(this->parent());
    QHBoxLayout *parentLayout = qobject_cast<QHBoxLayout*>(parentWidget->layout());
    sideBarWidget = qobject_cast<LogoAndSideBarWidget*>(parentLayout->itemAt(0)->widget());
    connect(sideBarWidget, SIGNAL(homePageClicked),
            this, SLOT(setHomePage));
    connect(sideBarWidget, SIGNAL(buyPageClicked),
            this, SLOT(setBuyPage));

    stackedWidget->setCurrentIndex(0);
}

QNetworkAccessManager *TopBarAndStackedWidget::getNetworkManager()
{
    return appNetworkManager;
}

void TopBarAndStackedWidget::onExitButtonClicked()
{
    QApplication::quit();
}

void TopBarAndStackedWidget::setSignUpPage()
{
    stackedWidget->setCurrentIndex(1);
}

void TopBarAndStackedWidget::setSignInPage()
{
    stackedWidget->setCurrentIndex(0);
}

void TopBarAndStackedWidget::setMainPage()
{
    stackedWidget->setCurrentIndex(2);
}

void TopBarAndStackedWidget::setHomePage()
{
    stackedWidget->setCurrentIndex(2);
}

void TopBarAndStackedWidget::setBuyPage()
{
    stackedWidget->setCurrentIndex(3);
}

void TopBarAndStackedWidget::createLayouts()
{
    rightWidgetLayout = new QVBoxLayout(this);
    rightWidgetLayout->setSpacing(0);
    rightWidgetLayout->setContentsMargins(0, 0, 0, 0);

    /* Create top bar widget to make it topBarLayouts' parent */
    topBarWidget = new QWidget(this);
    topBarWidget->setStyleSheet("background-color: #FFFFFF");

    topBarLayout = new QHBoxLayout(topBarWidget);
    topBarLayout->setSpacing(26);
    topBarLayout->setContentsMargins(954, 12, 26, 12);
}

void TopBarAndStackedWidget::createStackedWidget()
{
    stackedWidget = new QStackedWidget(this);
    stackedWidget->setFixedSize(1200, 640);
    stackedWidget->setStyleSheet("background-color: #FAFAFA");

    /* Initialize all widgets pinned to stacked widget and connect them to it. */
    signInPage = new SignInWidget(stackedWidget);
    signUpPage = new SignUpWidget(stackedWidget);
    homePage = new HomePageWidget(stackedWidget);
    buyPage = new BuyWidget(stackedWidget);

    stackedWidget->addWidget(signInPage);
    stackedWidget->addWidget(signUpPage);
    stackedWidget->addWidget(homePage);
    stackedWidget->addWidget(buyPage);
}

void TopBarAndStackedWidget::createButtons()
{
    iconsDir.setPath("../ClientApp/icons");
    for (int i = 0; i < 3; ++i) {
        topBarButtons[i] = new QPushButton(topBarWidget);
        topBarButtons[i]->setFixedSize(button_size, button_size);
        topBarButtons[i]->setStyleSheet("background-color:" + buttonColours[i] + ";"
                                          "border-radius: 15px");
        QString icon_path = iconsDir.absolutePath() + "/" + iconsNames[i];
        topBarButtons[i]->setIcon(QIcon(icon_path));
        topBarButtons[i]->setIconSize(QSize(icon_size, icon_size));
    }
}
