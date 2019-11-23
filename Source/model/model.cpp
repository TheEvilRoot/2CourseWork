#include "model.hpp"
#include "settings.hpp"
#include "model/data/choicetest.hpp"
#include "model/data/checktest.hpp"
#include "model/data/inputtest.hpp"
#include "api/utils.hpp"

#include <QStandardPaths>
#include <QFile>
#include <QTextStream>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <algorithm>
#include <QDir>
#include <QDebug>

Model::Model(Settings *settings,
             RandomGenerator *random): mSession(nullptr), mSettings(settings), mRandomGen(random) {
    mVersion = QString::number(mSettings->versionMajor) + "." + QString::number(mSettings->versionMinor) + "-" + QString::number(mSettings->versionBuild) + mSettings->versionSign;
}

QString& Model::getFromPair(std::pair<QString, QString> &pair, uint index) {
    index %= 2;
    return index == 0 ? pair.first : pair.second;
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
  delete mSession;
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
    if (!mWords.empty() && !forceReload) return true;
    if (!mWords.empty()) mWords.clear();

    auto *file = new QFile(":/data/words.db");
    if (!file->open(QIODevice::ReadOnly)) {
        auto message = file->errorString();
        delete file;

        throw QString::fromUtf8("Unable to open words file: ") + message;
    }

    auto *stream = new QTextStream(file);
    stream->setCodec("UTF-8");

    int lineNum = 0;
    while (!stream->atEnd()) {
        lineNum++;

        QString line = stream->readLine();
        if (line.startsWith("#")) continue;

        QStringList list = line.split("\t");
        if (list.size() < 2) {
            qDebug() << "[words.db] Line " << lineNum << " has invastoreSessionlid stucture\n";
            continue;
        }

       mWords.push_back({ list[0], list[1] });
    }

    file->close();

    delete file;
    delete stream;
    return !mWords.empty();
}

bool Model::loadSentences(bool forceReload) {
    if (mSentences.size() + mSentenceAnswers.size() > 0 && !forceReload) return true;
    mSentences.clear();
    mSentenceAnswers.clear();

    auto *file = new QFile(":/data/sentences.db");
    if (!file->open(QIODevice::ReadOnly)) {
        auto message = file->errorString();
        delete file;

        throw QString::fromUtf8("Unable to open sentences file: ") + message;
    }

    auto *stream = new QTextStream(file);
    int lineNum = 0;
    while (!stream->atEnd()) {
        lineNum++;

        auto line = stream->readLine();
        if (line.startsWith("#")) continue;

        auto list = line.split("\t");
        auto sentence = list[0];

        QString answer = "";
        if (list.size() > 1)
            answer = list[1];
        mSentences.push_back(std::pair<QString, QString>(sentence, answer));
        // If not already added
        if (std::find(mSentenceAnswers.begin(), mSentenceAnswers.end(), answer) == mSentenceAnswers.end()) {
            mSentenceAnswers.push_back(answer);
        }
    }

    file->close();

    delete file;
    delete stream;

    return !mSentences.empty();
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

    if (count > mWords.size())
        count = mWords.size();

    size_t randomIndex = 0;
    for (size_t i = 0; i < count; i++) {
        randomIndex = mRandomGen->intInRange(mWords.size());

        std::iter_swap(mWords.end() - 1, mWords.begin() + randomIndex);

        list.push_back(mWords.back());
        mWords.pop_back();
    }

    for (const auto& word : list) {
        mWords.push_back(word);
    }

    return list;
}

std::pair<QString, QString> Model::getRandomSentence() {
    size_t randomIndex = mRandomGen->intInRange(mSentences.size());
    return mSentences[randomIndex];
}

std::vector<QString> Model::getRandomSentenceAnswers(size_t count) {
    std::vector<QString> list;
    if (count > mSentenceAnswers.size())
        count = mSentenceAnswers.size();

    size_t randomIndex = 0;
    for (size_t i = 0; i < count; i++) {
        randomIndex = mRandomGen->intInRange(mSentenceAnswers.size());

        std::iter_swap(mSentenceAnswers.end() - 1, mSentenceAnswers.begin() + randomIndex);

        list.push_back(mSentenceAnswers.back());
        mSentenceAnswers.pop_back();
    }

    for (const auto& sentence : list) {
        mSentenceAnswers.push_back(sentence);
    }

    return list;
}

std::vector<BaseTest *> Model::generateTests() {
    std::vector<BaseTest *> tests;
    int wordBasedCount = mSettings->wordsTestsCount;
    int sentenceBasedCount = mSettings->sentencesTestsCount;
    int numberOfOptions = 6;

    // Generate word-based tests
    for (int i = 0; i < wordBasedCount; i++) {
        auto options = getRandomWords(numberOfOptions);
        std::vector<QString> answers;

        auto direction = static_cast<uint>(mRandomGen->intInRange(2));

        for (size_t i = 0; i < numberOfOptions; i++) {
            answers.push_back (getFromPair(options[i], direction));
        }

        uint randomIndex = static_cast<uint>(mRandomGen->intInRange(numberOfOptions));

        BaseTest *test = nullptr;
        if (mRandomGen->intInRange(2) == 1) {
            test = new ChoiceTest(getFromPair(options[randomIndex], !direction), answers, randomIndex);
        } else {
            test = new CheckTest(getFromPair(options[randomIndex], !direction), answers, randomIndex);
        }

        if (test)
            tests.push_back(test);
    }

    int probabilityModifyer = 10;
    int numberOfSentences = 5;

    // Generate sentence based tests
    for (int i = 0; i < sentenceBasedCount; i++) {
        std::pair<QString, QString> sentence = getRandomSentence();

        int random = mRandomGen->intInRange(probabilityModifyer);
        BaseTest *testToPush = nullptr;
        if (random % 2 == 0) {
            std::vector<QString> wordsForAnswers = getRandomSentenceAnswers(numberOfSentences);
            std::vector<QString> answers;

            for (const auto& wordForAnswer : wordsForAnswers) {
                answers.push_back (QString(sentence.first).replace("[ ]", wordForAnswer));
            }

            answers.push_back(QString(sentence.first).replace("[ ]", sentence.second));

            // Set correct answer to random position
            size_t randomCorrentIndex = mRandomGen->intInRange(answers.size());
            std::iter_swap(answers.begin() + randomCorrentIndex, answers.end() - 1);

            testToPush = new CheckTest(sentence.first, answers, randomCorrentIndex);
        } else {
            testToPush = new InputTest(sentence.first, sentence.second);
        }

        if (testToPush) {
            testToPush->setSentenceBased();
            tests.push_back(testToPush);
        }
    }

    return tests;
}

bool Model::loadHistory() {
    auto path = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    auto *file = new QFile(path + "/history.json");

    if (!file->exists()) {
        file->close(); // very controversial, should we close the file if it doesn't exists ?!
        delete file;
        return true;
    } // If file does not exists, history loading is not necessary, so it's actually succeed

    if (!file->open(QIODevice::ReadOnly)) {
        auto message = file->errorString();
        delete file;

        throw QString::fromUtf8("Unable to open history file: ") + message;
    }

    auto *stream = new QTextStream(file);
    stream->setCodec("UTF-8");

    auto text = stream->readAll();
    auto fileObj = QJsonDocument::fromJson(text.toUtf8()).object();

    if (!fileObj.keys().contains("history")) {
        file->remove();
        file->close();
        delete file;

        throw QString::fromUtf8("History file is invalid");
    }

    auto historyEntries = fileObj.value("history").toArray();

    for (auto entry : historyEntries) {
        auto state = new SessionState(entry.toObject());
        mHistory.push_back(state);
    }

    file->close();

    delete file;
    delete stream;

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
    auto *directory = new QDir(path);
    if (!directory->exists()) directory->mkpath(".");

    // Open file
    auto *file = new QFile(path + "/history.json");
    if (!file->open(QIODevice::WriteOnly)) {
        throw QString::fromUtf8("Failed to open file to save history: ") + file->errorString();
    }

    auto *stream = new QTextStream(file);
    stream->setCodec("UTF-8");
    stream->operator<<(QString::fromUtf8(doc.toJson()));

    file->close();

    delete directory;
    delete file;
    delete stream;

    return true;
}

std::deque<SessionState *>& Model::getHistory() {
    return mHistory;
}

void Model::sessionConclude() {
    mSession->generateConclusion();
}
