#pragma once

#include <model/model.hpp>

#include <QThread>

class WordsFileLoader : public QThread {

    Q_OBJECT

private:
    Model *mModel;
public:
    WordsFileLoader(Model *model);
    ~WordsFileLoader();

    void run() override;

signals:
    void progressDone();
    void progressError(QString, bool);
};
