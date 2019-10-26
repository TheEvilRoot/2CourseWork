#include "model.hpp"
#include "settings.h"
#include "model/data/choicetest.h"
#include "model/data/checktest.h"
#include "model/data/inputtest.hpp"
#include "api/utils.h"

#include<QStandardPaths>
#include <QFile>
#include <QTextStream>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <iostream>
#include <algorithm>
#include <QDir>
Model::Model(Settings *settings, RandomGenerator *random): mSession(nullptr), mSettings(settings), mRandomGen(random) {
    mVersion = QString::number(mSettings->versionMajor) + "." + QString::number(mSettings->versionMinor) + "-" + QString::number(mSettings->versionBuild) + mSettings->versionSign;
}

QString& Model::getVersion() {
    return mVersion;
}

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
  this->mSession = new Session(generateTests(), mSettings->attemptsCount);

  return 0;
}

Session* Model::getSession() const {
  return mSession;
}

SessionState* Model::getLastSession() {
    return mHistory.back();
}

void Model::storeSession(SessionState *state) {
    state->updateSolveTime();
    mHistory.push_back(state);
}

bool Model::loadWords(bool forceReload) {
    if (mWords.size() > 0 && !forceReload) return true;
    if (mWords.size()) mWords.clear();

    QFile *file = new QFile(":/data/words.db");
    if (!file->open(QIODevice::ReadOnly)) {
        auto message = QString::fromUtf8("Unable to open words file: ") + file->errorString();
        file->close();
        if (file) delete file;
        throw message;
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
            std::cerr << "[words.db] Line " << lineNum << " has invastoreSessionlid stucture\n";
            continue;
        }

       mWords.push_back(std::pair<QString, QString>(list[0], list[1]));
    }
    file->close();

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
        auto message = QString::fromUtf8("Unable to open sentences file: ") + file->errorString();
        file->close();
        if (file) delete file;
        throw message;
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

    file->close();

    if (file) delete file;
    if (stream) delete stream;

    std::cout << "[D] " << mSentences.size() << " " << mSentenceAnswers.size() << "\n";
    return mSentences.size();
}

/*
 * Algorithm:
 * get random index
 * swap with last
 * pop last to buffer
 * while count
 * add popped items back to list
 * return buffer
 * Time: O(count) + O(1) + O(1) + O(1) + O(count) = O(count) = O(N)
 */
WordsList Model::getRandomWords(size_t count) {
    WordsList list;
    if (count > mWords.size()) count = mWords.size();
    size_t lastIndex, randomIndex;
    for (size_t i = 0; i < count; i++) {
        lastIndex = mWords.size() - 1;
        randomIndex = mRandomGen->intInRange(mWords.size());

        std::swap(mWords[lastIndex], mWords[randomIndex]);
        list.push_back(mWords[lastIndex]);
        mWords.pop_back();
    }
    for (size_t i = 0; i < list.size(); i++) mWords.push_back(list[i]);
    return list;
}

std::pair<QString, QString> Model::getRandomSentence() {
    size_t randomIndex = mRandomGen->intInRange(mSentences.size());
    return mSentences[randomIndex];
}

std::vector<QString> Model::getRandomSentenceAnswers(size_t count) {
    std::vector<QString> list;
    if (count > mSentenceAnswers.size()) count = mSentenceAnswers.size();
    size_t lastIndex, randomIndex;
    for (size_t i = 0; i < count; i++) {
        lastIndex = mSentenceAnswers.size() - 1;
        randomIndex = mRandomGen->intInRange(mSentenceAnswers.size());

        std::swap(mSentenceAnswers[lastIndex], mSentenceAnswers[randomIndex]);
        list.push_back(mSentenceAnswers[lastIndex]);
        mSentenceAnswers.pop_back();
    }
    for (size_t i = 0; i < list.size(); i++) mSentenceAnswers.push_back(list[i]);
    return list;
}

std::vector<BaseTest *> Model::generateTests() {
    std::vector<BaseTest *> tests;
    int wordBasedCount = mRandomGen->intInRange(10, 20);
    int sentenceBasedCount = mRandomGen->intInRange(3, 10);

    for (int i = 0; i < wordBasedCount; i++) {
        auto options = getRandomWords(6);
        std::vector<QString> answers;
        uint direction = static_cast<uint>(mRandomGen->intInRange(2));
        for (auto &p : options) answers.push_back(getFromPair(p, direction));
        uint randomIndex = static_cast<uint>(mRandomGen->intInRange(6));

        if (mRandomGen->intInRange(6) > 3) {
            tests.push_back(new ChoiceTest(getFromPair(options[randomIndex], !direction), answers, randomIndex));
        } else {
            tests.push_back(new CheckTest(getFromPair(options[randomIndex], !direction), answers, randomIndex));
        }
    }
    for (int i = 0; i < sentenceBasedCount; i++) {
        std::pair<QString, QString> sentence = getRandomSentence();

        int random = mRandomGen->intInRange(10);
        BaseTest *testToPush;
        if (random % 2 == 0) {
            std::vector<QString> wordsForAnswers = getRandomSentenceAnswers(5);
            std::vector<QString> answers;
            for (auto w : wordsForAnswers) {
                answers.push_back(QString(sentence.first).replace("[ ]", w));
            }
            answers.push_back(QString(sentence.first).replace("[ ]", sentence.second));
            testToPush = new CheckTest(sentence.first, answers, 5);
        } else {
            testToPush = new InputTest(sentence.first, sentence.second);
        }
        testToPush->setSentenceBased();
        tests.push_back(testToPush);
    }
    return tests;
}

bool Model::loadHistory() {
    auto path = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QFile *file = new QFile(path + "/history.json");

    if (!file->exists()) {
        file->close();
        if (file) delete file;
        return true;
    } // If file does not exists, history loading is not necessary, so it's actually succeed

    if (!file->open(QIODevice::ReadOnly)) {
        auto message = QString::fromUtf8("Unable to open history file: ") + file->errorString();
        file->close();
        if (file) delete file;
        throw message;
    }

    QTextStream *stream = new QTextStream(file);
    stream->setCodec("UTF-8");

    QString text = stream->readAll();
    QJsonObject fileObj = QJsonDocument::fromJson(text.toUtf8()).object();

    if (!fileObj.keys().contains("history")) {
        file->remove();
        file->close();
        if (file) delete file;
        throw QString::fromUtf8("History file is invalid");
    }

    QJsonArray historyEntries = fileObj.value("history").toArray();

    for (auto entry : historyEntries) {
        auto state = new SessionState(entry.toObject());
        mHistory.push_back(state);
    }

    file->close();

    if (file) delete file;
    if (stream) delete stream;

    return true;
}

// Throws QString with error message
bool Model::saveHistory() {
    // Json generation
    QJsonObject obj;
    QJsonArray history;
    for (auto state : mHistory) {
        history.push_back(state->toJson());
    }
    obj.insert("history", history);
    QJsonDocument doc(obj);

    // Create AppData directory is doesn't exists
    auto path = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir *directory = new QDir(path);
    if (!directory->exists()) directory->mkpath(".");

    // Open file
    QFile *file = new QFile(path + "/history.json");
    if (!file->open(QIODevice::WriteOnly)) {
        throw QString::fromUtf8("Failed to open file to save history: ") + file->errorString();
    }

    QTextStream *stream = new QTextStream(file);
    stream->setCodec("UTF-8");
    stream->operator<<(QString::fromUtf8(doc.toJson()));

    file->close();
    return true;
}

std::deque<SessionState *>& Model::getHistory() {
    return mHistory;
}

void Model::sessionConclude() {
    mSession->generateConclusion();
}
