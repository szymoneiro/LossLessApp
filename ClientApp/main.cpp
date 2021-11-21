#include "headers/main_widget.h"
#include "headers/home_page_widget.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //MainWidget w;
    HomePageWidget w;
    w.show();
    return a.exec();
}
