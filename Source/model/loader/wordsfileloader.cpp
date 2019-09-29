#include "wordsfileloader.h"

WordsFileLoader::WordsFileLoader(Model *model): mModel(model) { }

void WordsFileLoader::run() {
    sleep(5);
    if (mModel->loadWords()) {
       emit progressDone();
    } else {
        emit progressError("Unable to load words");
    }
}
