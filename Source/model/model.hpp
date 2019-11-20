#pragma once

#include "model/session/session.hpp"
#include "settings.hpp"
#include "api/randomgenerator.hpp"
#include "api/graphdict.hpp"

typedef std::vector<std::pair<QString, QString>> WordsList;

class Model {
private:
  Session *mSession;
  Settings *mSettings;
  RandomGenerator *mRandomGen;
  GraphDict *mWordsGraph;
  WordsList mSentences;
  std::vector<QString> mSentenceAnswers;
  std::deque<SessionState *> mHistory;

  QString mVersion;

public:
  Model(Settings *settings, RandomGenerator *random);
  QString& getFromPair(std::pair<QString, QString> &pair, uint index);
  QString& getVersion();

  int newSession(bool force);
  Session* getSession() const;

  bool loadWords(bool forceReload = false);
  bool loadSentences(bool forceReload = false);

  WordsList getRandomWords(size_t count = 1);
  std::pair<QString, QString> getRandomSentence();
  std::vector<QString> getRandomSentenceAnswers(size_t count = 1);
  TestList generateTests();
  void sessionConclude();

  bool loadHistory();
  bool saveHistory();
  SessionState * getLastSession();
  void storeSession(SessionState *state);
  std::deque<SessionState *>& getHistory();
};
