#include "wordsfileloader.hpp"

#include <QDebug>

WordsFileLoader::WordsFileLoader(Model *model): mModel(model) { }

void WordsFileLoader::run() {
    try {
        auto result = mModel->loadWords() &&
            mModel->loadSentences() &&
            mModel->loadHistory();
        qDebug() << "File loader: " << result;
        emit progressDone();
    } catch(QString& msg) {
        emit progressError(msg, true);
    }
}
