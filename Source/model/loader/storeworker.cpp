#include "model/loader/storeworker.hpp"

#include <iostream>

StoreWorker::StoreWorker(Model *model): mModel(model) { }

void StoreWorker::run() {
    try {
        if (mModel->saveHistory()) {
            emit progressDone();
        } else {
            emit progressError("Failed");
        }
    } catch(...){
        emit progressError("Exception!!");
    }
}
