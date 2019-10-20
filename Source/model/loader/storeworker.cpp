#include "model/loader/storeworker.hpp"

#include <iostream>

StoreWorker::StoreWorker(Model *model, const ViewType *nextView): mModel(model), mNextView(nextView) { }

void StoreWorker::run() {
    try {
       if (!mModel->saveHistory()) emit progressError("Something really went wrong");
    } catch(QString &msg){
        emit progressError(msg);
        return;
    }

    emit progressDone(mNextView);
}
