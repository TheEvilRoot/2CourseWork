#include "view/main/mainwindow.hpp"
#include "view/viewtype.h"
#include "view/mainview.hpp"
#include "model/model.hpp"
#include "model/settings.h"

#include <QApplication>
#include <iostream>

int ViewType::GLOBAL_ID = 0;

const ViewType* ViewType::MENU = new ViewType("menu");
const ViewType* ViewType::CHOICE = new ViewType("choice");
const ViewType* ViewType::INPUT = new ViewType("input");
const ViewType* ViewType::CHECK = new ViewType("check");
const ViewType* ViewType::RESULT = new ViewType("result");

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    Settings *settings = new Settings();
    Q_ASSERT(settings);

    QRandomGenerator *random = new QRandomGenerator((int) time(nullptr));

    Model *model = new Model(settings, random);
    Q_ASSERT(model);

    MainWindow w(model, settings);
    MainView *view = dynamic_cast<MainView*>(&w);
    Q_ASSERT(view);

    Q_ASSERT(view->presentView(nullptr) == false);
    w.show();
    return a.exec();
}
