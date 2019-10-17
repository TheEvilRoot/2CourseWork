#include "model/loader/storeworker.hpp"

#include <iostream>

StoreWorker::StoreWorker(Model *model, const ViewType *nextView): mModel(model), mNextView(nextView) { }

void StoreWorker::run() {
    try {
        if (mModel->saveHistory()) {
            emit progressDone(mNextView);
        } else {
            emit progressError("Failed");
        }
    } catch(...){
        emit progressError("Exception!!");
    }
}
