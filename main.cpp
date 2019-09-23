#include "mainwindow.hpp"

#include <QApplication>

#include "viewtype.h"
#include "mainview.hpp"
#include "model.hpp"
#include "settings.h"

int ViewType::GLOBAL_ID = 0;

const ViewType* ViewType::MENU = new ViewType("menu");
const ViewType* ViewType::CHOICE = new ViewType("choice");
const ViewType* ViewType::INPUT = new ViewType("input");
const ViewType* ViewType::CHECK = new ViewType("check");
const ViewType* ViewType::RESULT = new ViewType("result");

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

//    QRandomGenerator *rand = new QRandomGenerator((quint8) time(nullptr));
//    Q_ASSERT(rand);

    Settings *settings = new Settings();
    Q_ASSERT(settings);

    Model *model = new Model(settings);
    Q_ASSERT(model);

    MainWindow w(model, settings);
    MainView *view = dynamic_cast<MainView*>(&w);
    Q_ASSERT(view);

    Q_ASSERT(view->presentView(nullptr) == false);
    w.show();
    return a.exec();
}
