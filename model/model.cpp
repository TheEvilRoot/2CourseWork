#include "model.hpp"
#include "settings.h"
#include "model/data/choicetest.h"

#include <QFile>
#include <QTextStream>
#include <iostream>

Session* Model::getSession() const {
  return mSession;
}

Model::Model(Settings *settings): mSession(nullptr), mSettings(settings), mRandomGen(new QRandomGenerator) {}

int Model::newSession(bool force) {
  // If session is not null
  // If session is not finished
  // And if not force creation
  // Return Error 1
  if (this->mSession != nullptr && !this->mSession->isFinished() && !force) {
    return 1;
  }

  // Otherwise delete (free) current session (if even exists) and create a new one!
  if (mSession) delete mSession;
  this->mSession = new Session(generateTests(), mSettings->isAttemptMode);

  return 0;
}

std::vector<BaseTest *> Model::generateTests() {
    std::vector<BaseTest *> tests;
    int count = mRandomGen->bounded(10, 15);

    for (int i = 0; i < count; i++) {
        auto options = getRandomWords(6);
        std::vector<QString> answers;
        for (auto [a, q] : options) answers.push_back(a);
        size_t randomIndex = mRandomGen->bounded(0, 6);
        tests.push_back(new ChoiceTest(options[randomIndex].second, answers, randomIndex));
    }
    return tests;
}

bool Model::loadWords(bool forceReload) {
    if (mWords.size() > 0 && !forceReload) return true;
    if (mWords.size()) mWords.clear();

    QFile *file = new QFile(":/data/words.db");
    if (!file->open(QIODevice::ReadOnly)) {
        if (file) delete file;
        return false;
    }

    QTextStream *stream = new QTextStream(file);
    stream->setCodec("UTF-8");

    int lineNum = 0;
    while (!stream->atEnd()) {
        lineNum++;

        QString line = stream->readLine();
        if (line.startsWith("#")) continue;

        QStringList list = line.split("\t");
        if (list.size() < 2) {
            std::cerr << "[words.db] Line " << lineNum << " has invalid stucture\n";
            continue;
        }

       mWords.push_back(std::pair<QString, QString>(list[0], list[1]));
    }

    if (file) delete file;
    if (stream) delete stream;

    return mWords.size();
}

// Algorithm:
//
// get random index
// swap with last
// pop last to buffer
// while count
// add popped items back to list
// return buffer
// Time: O(count) + O(1) + O(1) + O(1) + O(count) = O(count) = O(N)

WordsList Model::getRandomWords(size_t count) {
    WordsList list;
    if (count > mWords.size()) count = mWords.size();
    size_t lastIndex, randomIndex;
    for (size_t i = 0; i < count; i++) {
        lastIndex = mWords.size() - 1;
        randomIndex = mRandomGen->bounded(0, mWords.size());

        std::swap(mWords[lastIndex], mWords[randomIndex]);
        list.push_back(mWords[lastIndex]);
        mWords.pop_back();
    }
    for (size_t i = 0; i < list.size(); i++) mWords.push_back(list[i]);
    return list;
}
