#include "../headers/logo_and_side_bar_widget.h"

LogoAndSideBarWidget::LogoAndSideBarWidget(QWidget *parent) : QWidget(parent)
{
    createSideBar();
    createLayouts();
    createLogoBar();
    createButtons();

    iconWidgetLayout->addWidget(logoWidget);
    iconWidgetLayout->addWidget(sideBarWidget);

    sideBarWidget->setLayout(sideBarWidgetLayout);
    for (int i = 0; i < 4; ++i)
        sideBarWidgetLayout->addWidget(sideBarButtons[i]);
}

void LogoAndSideBarWidget::createLayouts()
{
    iconWidgetLayout = new QVBoxLayout(this);
    iconWidgetLayout->setSpacing(0);
    iconWidgetLayout->setContentsMargins(0, 0, 0, 0);

    sideBarWidgetLayout = new QVBoxLayout(sideBarWidget);
    sideBarWidgetLayout->setSpacing(8);
    sideBarWidgetLayout->setContentsMargins(12, 16, 12, 376);
}

void LogoAndSideBarWidget::createLogoBar()
{
    logoWidget = new QLabel(this);
    logoWidget->setText("LOGO");
    logoWidget->setStyleSheet("background-color: #00FFA3;"
                            "color: #FFFFFF;"
                            "font: bold;");
    logoWidget->setAlignment(Qt::AlignCenter);
    logoWidget->setFixedSize(80, 80);
}

void LogoAndSideBarWidget::createSideBar()
{
    sideBarWidget = new QWidget(this);
    sideBarWidget->setStyleSheet("background-color: #4C3099");
}

void LogoAndSideBarWidget::createButtons()
{
    /* Set path to button icons */
    iconsDir.setPath("../ClientApp/icons");

    for (int i = 0; i < 4; ++i) {
        sideBarButtons[i] = new QPushButton(sideBarWidget);
        sideBarButtons[i]->setFixedSize(button_size, button_size);
        sideBarButtons[i]->setStyleSheet("background-color: white;"
                                  "border-radius: 15px");
        QString icon_path = iconsDir.absolutePath() + "/" + iconsNames[i];
        sideBarButtons[i]->setIcon(QIcon(icon_path));
        sideBarButtons[i]->setIconSize(QSize(icon_size, icon_size));
    }
}
