#include "headers/main_widget.h"
#include "headers/home_page_widget.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication application(argc, argv);
    MainWidget lossLessApp;
    lossLessApp.show();
    return application.exec();
}
