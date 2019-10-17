#pragma once

#include <model/model.hpp>

#include <QThread>

class StoreWorker : public QThread {

    Q_OBJECT

private:
    Model *mModel;
    const ViewType *mNextView;
public:
    StoreWorker(Model *model, const ViewType *nextView);
    void run() override;

signals:
    void progressDone(const ViewType *nextView);
    void progressError(QString);
};
