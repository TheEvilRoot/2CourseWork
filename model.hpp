#ifndef MODEL_HPP
#define MODEL_HPP

#include "session.hpp"

#include <QRandomGenerator>

class Model {
private:
  Session *mSession;
  QRandomGenerator *mRand;

public:
  Model(QRandomGenerator *rand);

  int newSession(bool force);
  Session* getSession() const;

  int getRandomPosition();
};

#endif // MODEL_HPP
