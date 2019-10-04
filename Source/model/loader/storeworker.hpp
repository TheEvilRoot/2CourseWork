#pragma once

#include <model/model.hpp>

#include <QThread>

class StoreWorker : public QThread {

    Q_OBJECT

private:
    Model *mModel;
public:
    StoreWorker(Model *model);
    void run() override;

signals:
    void progressDone();
    void progressError(QString);
};
