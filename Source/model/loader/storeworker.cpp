#include "model/loader/storeworker.hpp"

StoreWorker::StoreWorker(Model *model,
                         const ViewType *nextView): mModel(model), mNextView(nextView) { }

void StoreWorker::run() {
    try {
       mModel->saveHistory();
       emit progressDone(mNextView);
    } catch(QString &msg){
        emit progressError(msg);
    }
}
