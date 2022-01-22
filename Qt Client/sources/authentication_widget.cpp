#include "../headers/authentication_widget.h"
#include "../headers/top_bar_and_stacked_widget.h"

AuthenticationWidget::AuthenticationWidget(QWidget *parent) : QWidget(parent)
{
    this->setFixedSize(1200, 640);
    this->setStyleSheet("background-color: #FAFAFA");

    connectionManager = qobject_cast<TopBarAndStackedWidget*>(parent)->getNetworkManager();
}

void AuthenticationWidget::buttonsCreate()
{
    signInButton = new QPushButton("SIGN IN", this);
    signUpButton = new QPushButton("SIGN UP", this);
}

void AuthenticationWidget::showPassword()
{
    QDir iconsDir;
    iconsDir.setPath("../Qt Client/icons");
    QString icon_path = iconsDir.absolutePath() + "/" + "hide.svg";
    showPasswordButton->setIcon(QIcon(icon_path));
    showPasswordButton->setIconSize(QSize(32, 32));
}

void AuthenticationWidget::hidePassword()
{
    QDir iconsDir;
    iconsDir.setPath("../Qt Client/icons");
    QString icon_path = iconsDir.absolutePath() + "/" + "show.svg";
    showPasswordButton->setIcon(QIcon(icon_path));
    showPasswordButton->setIconSize(QSize(32, 32));
}

void AuthenticationWidget::updateMessageLabel(QString message, bool isError)
{
    messageLabel->setText(message);
    if (isError) {
        messageLabel->setStyleSheet("background-color: #FF0000;"
                                  "font: bold;"
                                  "color: #FFFFFF;"
                                  "font-size: 20px;"
                                  "border: 2px solid #000000;"
                                  "border-radius: 10px");
    }
    else {
        messageLabel->setStyleSheet("background-color: #00FFA3;"
                                  "font: bold;"
                                  "color: #FFFFFF;"
                                  "font-size: 20px;"
                                  "border: 2px solid #000000;"
                                  "border-radius: 10px");
    }
}

void AuthenticationWidget::createAndAddShadowEffect(QWidget *widget)
{
    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect(widget);
    shadow->setBlurRadius(5);
    /* 0.1 * 255 ~ 26 */
    shadow->setColor(QColor(0,0,0,26));
    shadow->setOffset(4, 4);

    widget->setGraphicsEffect(shadow);
}

void AuthenticationWidget::createShowPasswordButton()
{
    showPasswordButton = new QPushButton(this);
    QDir iconsDir;
    iconsDir.setPath("../Qt Client/icons");
    QString icon_path = iconsDir.absolutePath() + "/" + "show.svg";
    showPasswordButton->setIcon(QIcon(icon_path));
    showPasswordButton->setIconSize(QSize(32, 32));
    showPasswordButton->setStyleSheet("background: transparent");
}
