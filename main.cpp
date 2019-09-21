#include "mainwindow.hpp"

#include <QApplication>

#include "viewtype.h"
#include "mainview.hpp"

int ViewType::GLOBAL_ID = 0;
const ViewType* ViewType::MENU = new ViewType("menu");
const ViewType* ViewType::CHOICE = new ViewType("choice");
const ViewType* ViewType::INPUT = new ViewType("input");
const ViewType* ViewType::CHECK = new ViewType("check");

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    MainView *view = dynamic_cast<MainView*>(&w);

    Q_ASSERT(view);
    Q_ASSERT(view->presentView(nullptr) == false);

    w.show();
    return a.exec();
}
