#include "model.hpp"
#include "settings.h"
#include "model/data/choicetest.h"
#include "model/data/checktest.h"
#include "model/data/inputtest.hpp"
#include "api/utils.h"

#include <QFile>
#include <QTextStream>
#include <iostream>
#include <algorithm>

Session* Model::getSession() const {
  return mSession;
}

Model::Model(Settings *settings, QRandomGenerator *random): mSession(nullptr), mSettings(settings), mRandomGen(random) {}

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
    int wordBasedCount = 0;//mRandomGen->bounded(10, 20);
    int sentenceBasedCount = mRandomGen->bounded(3, 10);

    for (int i = 0; i < wordBasedCount; i++) {
        auto options = getRandomWords(6);
        std::vector<QString> answers;
        uint direction = (uint) mRandomGen->bounded(0, 2);
        for (auto &p : options) answers.push_back(getFromPair(p, direction));
        uint randomIndex = (uint) mRandomGen->bounded(0, 6);

        if (mRandomGen->bounded(0, 6) > 3) {
            tests.push_back(new ChoiceTest(getFromPair(options[randomIndex], !direction), answers, randomIndex));
        } else {
            tests.push_back(new CheckTest(getFromPair(options[randomIndex], !direction), answers, randomIndex));
        }
    }
    for (int i = 0; i < sentenceBasedCount; i++) {
        std::pair<QString, QString> sentence = getRandomSentence();

        int random = mRandomGen->bounded(10);
        if (random % 2 == 0) {
            std::vector<QString> wordsForAnswers = getRandomSentenceAnswers(5);
            std::vector<QString> answers;
            for (auto w : wordsForAnswers) {
                answers.push_back(QString(sentence.first).replace("[ ]", w));
            }
            answers.push_back(QString(sentence.first).replace("[ ]", sentence.second));
            for (auto w : wordsForAnswers) printf("[DW] %s\n", w.toStdString().c_str());
            printf("[D] %s:%s\n", sentence.first.toStdString().c_str(), sentence.second.toStdString().c_str());
            for (auto w : answers) printf(">> [D] %s\n", w.toStdString().c_str());

            if (random > 4) {
                tests.push_back(new ChoiceTest(sentence.first, answers, 5));
            } else {
                tests.push_back(new CheckTest(sentence.first, answers, 5));
            }
        } else {
            tests.push_back(new InputTest(sentence.first, sentence.second));
        }
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
bool Model::loadSentences(bool forceReload) {
    if (mSentences.size() + mSentenceAnswers.size() > 0 && !forceReload) return true;
    mSentences.clear();
    mSentenceAnswers.clear();

    QFile *file = new QFile(":/data/sentences.db");
    if (!file->open(QIODevice::ReadOnly)) {
        if (file) delete  file;
        return false;
    }

    QTextStream *stream = new QTextStream(file);
    int lineNum = 0;
    while (!stream->atEnd()) {
        lineNum++;

        QString line = stream->readLine();
        if (line.startsWith("#")) continue;

        QStringList list = line.split("\t");
        QString sentence = list[0];
        QString answer = "";
        if (list.size() > 1) answer = list[1];
        mSentences.push_back(std::pair<QString, QString>(sentence, answer));
        // If not already added
        if (std::find(mSentenceAnswers.begin(), mSentenceAnswers.end(), answer) == mSentenceAnswers.end()) {
            mSentenceAnswers.push_back(answer);
        }
    }

    if (file) delete file;
    if (stream) delete stream;

    std::cout << "[D] " << mSentences.size() << " " << mSentenceAnswers.size() << "\n";
    return mSentences.size();
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

std::pair<QString, QString> Model::getRandomSentence() {
    size_t randomIndex = mRandomGen->bounded(0, mSentences.size());
    return mSentences[randomIndex];
}

std::vector<QString> Model::getRandomSentenceAnswers(size_t count) {
    std::vector<QString> list;
    if (count > mSentenceAnswers.size()) count = mSentenceAnswers.size();
    size_t lastIndex, randomIndex;
    for (size_t i = 0; i < count; i++) {
        lastIndex = mSentenceAnswers.size() - 1;
        randomIndex = mRandomGen->bounded(0, mSentenceAnswers.size());

        std::swap(mSentenceAnswers[lastIndex], mSentenceAnswers[randomIndex]);
        list.push_back(mSentenceAnswers[lastIndex]);
        mSentenceAnswers.pop_back();
    }
    for (size_t i = 0; i < list.size(); i++) mSentenceAnswers.push_back(list[i]);
    return list;
}
