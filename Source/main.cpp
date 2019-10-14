#include "view/main/mainwindow.hpp"
#include "view/viewtype.h"
#include "view/mainview.hpp"
#include "model/model.hpp"
#include "model/settings.h"
#include "model/data/CEFR.hpp"

#include <QApplication>
#include <iostream>

int ViewType::GLOBAL_ID = 0;
std::vector<CEFR* > CEFR::list;

const ViewType* ViewType::MENU = new ViewType("menu");
const ViewType* ViewType::CHOICE = new ViewType("choice");
const ViewType* ViewType::INPUT = new ViewType("input");
const ViewType* ViewType::CHECK = new ViewType("check");
const ViewType* ViewType::RESULT = new ViewType("result");
const ViewType* ViewType::HISTORY = new ViewType("history");

int CEFR::GLOBAL_ID = 0;

const CEFR* CEFR::NOTHING = new CEFR("Need more practice.");
const CEFR* CEFR::A1 = new CEFR("A1");
const CEFR* CEFR::A2 = new CEFR("A2");
const CEFR* CEFR::B1 = new CEFR("B1");
const CEFR* CEFR::B2 = new CEFR("B2");
const CEFR* CEFR::C1 = new CEFR("C1");
const CEFR* CEFR::C2 = new CEFR("C2");

int main(int argc, char *argv[]) {
    QApplication application(argc, argv);

    Settings *settings = new Settings();
    Q_ASSERT(settings);

    QRandomGenerator *random = new QRandomGenerator((int) time(nullptr));
    Q_ASSERT(random);

    Model *model = new Model(settings, random);
    Q_ASSERT(model);

    MainWindow window(&application, model, settings);
    MainView *view = dynamic_cast<MainView*>(&window);
    Q_ASSERT(view);

    Q_ASSERT(view->presentView(nullptr) == false);
    window.show();
    return application.exec();
}
