#ifndef MODEL_HPP
#define MODEL_HPP

#include "session.hpp"

class Model {
private:
  Session *mSession;
public:
  Model();

  int newSession(bool force);
  Session* getSession() const;
};

#endif // MODEL_HPP
