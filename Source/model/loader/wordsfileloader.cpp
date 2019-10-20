#include "wordsfileloader.h"

#include <iostream>

WordsFileLoader::WordsFileLoader(Model *model): mModel(model) { }

void WordsFileLoader::run() {
    try {
        if (!mModel->loadWords() || !mModel->loadSentences()) emit progressError("Something really went wrong.", true);
    } catch(QString& msg) {
        emit progressError(msg, true);
        return;
    }

    try {
        mModel->loadHistory();
    } catch (QString& msg) {
        emit progressError(msg, false);
        return;
    }

    emit progressDone();
}
