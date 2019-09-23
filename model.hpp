#ifndef MODEL_HPP
#define MODEL_HPP

#include "session.hpp"
#include "settings.h"

// #include <Random>

class Model {
private:
  Session *mSession;
  Settings *mSettings;

public:
  Model(Settings *settings);

  int newSession(bool force);
  Session* getSession() const;

  // int getRandomPosition();
};

#endif // MODEL_HPP
