#include "model/loader/storeworker.hpp"

#include <QDebug>

StoreWorker::StoreWorker(Model *model,
                         const ViewType *nextView): mModel(model), mNextView(nextView) { }

StoreWorker::~StoreWorker() {
    qDebug() << "StoreWorker destruction...";
}

void StoreWorker::run() {
    try {
       mModel->saveHistory();
       emit progressDone(mNextView);
    } catch(QString &msg){
        emit progressError(msg);
    }
}
