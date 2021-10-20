#ifndef TOP_BAR_AND_STACKED_WIDGET_H
#define TOP_BAR_AND_STACKED_WIDGET_H

#include <QtWidgets>
#include <QNetworkAccessManager>
#include "signup_widget.h"

class TopBarAndStackedWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TopBarAndStackedWidget(QWidget *parent = nullptr);
private slots:
    void onExitButtonClicked();
    void onClickGet();
    void managerFinished(QNetworkReply *reply);
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
    SignUpWidget *signUpPage;

    /* Buttons SETTINGS/USER/EXIT */
    QPushButton *topBarButtons[3];

    /* Backend connection components */
    QNetworkAccessManager *manager;
    QNetworkReply *reply;

    void createLayouts();
    void createStackedWidget();
    void createButtons();

};

#endif // TOP_BAR_AND_STACKED_WIDGET_H
