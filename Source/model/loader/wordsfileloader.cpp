#include "wordsfileloader.h"

#include <iostream>

WordsFileLoader::WordsFileLoader(Model *model): mModel(model) { }

void WordsFileLoader::run() {
//    sleep(5);
    bool loadWords = mModel->loadWords();
    bool loadSentences = mModel->loadSentences();
    if (loadWords && loadSentences) {
        emit progressDone();
    } else {
        emit progressError("Words loaded: " + QString::number(loadWords) + " Sentences loaded: " + QString::number(loadSentences));
    }
}
