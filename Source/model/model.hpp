#pragma once

#include "model/session/session.hpp"
#include "settings.h"

#include <QRandomGenerator>

typedef std::vector<std::pair<QString, QString>> WordsList;

class Model {
private:
  Session *mSession;
  Settings *mSettings;
  QRandomGenerator *mRandomGen;
  WordsList mWords;
  WordsList mSentences;
  std::vector<QString> mSentenceAnswers;
  std::deque<SessionState *> mHistory;

public:
  Model(Settings *settings, QRandomGenerator *random);

  int newSession(bool force);
  Session* getSession() const;

  bool loadWords(bool forceReload = false);
  bool loadSentences(bool forceReload = false);

  WordsList getRandomWords(size_t count = 1);
  std::pair<QString, QString> getRandomSentence();
  std::vector<QString> getRandomSentenceAnswers(size_t count = 1);
  TestList generateTests();

  bool loadHistory();
  bool saveHistory();
  SessionState * getLastSession();
  void storeSession(SessionState *state);
};
