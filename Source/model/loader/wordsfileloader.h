#ifndef WORDSFILELOADER_H
#define WORDSFILELOADER_H

#include <model/model.hpp>

#include <QThread>

class WordsFileLoader : public QThread {

    Q_OBJECT

private:
    Model *mModel;
public:
    WordsFileLoader(Model *model);
    void run() override;

signals:
    void progressDone();
    void progressError(QString);
};

#endif // WORDSFILELOADER_H
