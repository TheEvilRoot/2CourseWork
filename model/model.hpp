#ifndef MODEL_HPP
#define MODEL_HPP

#include "model/session/session.hpp"
#include "settings.h"

#include <QRandomGenerator>

typedef std::vector<std::pair<QString, QString>> WordsList;

class Model {
private:
  Session *mSession;
  Settings *mSettings;
  WordsList mWords;
  QRandomGenerator *mRandomGen;

public:
  Model(Settings *settings);

  int newSession(bool force);
  Session* getSession() const;

  bool loadWords(bool forceReload = false);
  WordsList getRandomWords(size_t count = 1);
};

#endif // MODEL_HPP
