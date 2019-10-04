#include "wordsfileloader.h"

#include <iostream>

WordsFileLoader::WordsFileLoader(Model *model): mModel(model) { }

void WordsFileLoader::run() {
//    sleep(5);
    bool loadWords = mModel->loadWords();
    bool loadSentences = mModel->loadSentences();
    bool loadHistory = mModel->loadHistory();
    auto res = ("Words loaded: " + QString::number(loadWords) + " Sentences loaded: " + QString::number(loadSentences) + "History loaded: " + QString::number(loadHistory));
    if (loadWords && loadSentences && loadHistory) {
        emit progressDone();
    } else {
        emit progressError(res);
    }
}
