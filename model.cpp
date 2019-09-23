#include "model.hpp"
#include "settings.h"

#include <QFile>
#include <QTextStream>
#include <iostream>
#include <set>

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
  this->mSession = new Session(mSettings->isAttemptMode);

  return 0;
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

WordsList Model::getRandomWords(size_t count) {
    WordsList list;
    if (count > mWords.size()) count = mWords.size();
    std::set<size_t> indeces;
    for (; indeces.size() < count;) {
        indeces.insert(mRandomGen->bounded((qint8) 0,(qint8) mWords.size()));
    }
    for (auto index : indeces) {
        list.push_back(mWords[index]);
    }
    return list;
}
