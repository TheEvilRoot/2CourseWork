#include "model.hpp"

Session* Model::getSession() const {
  return mSession;
}

Model::Model(): mSession(nullptr) {}

int Model::newSession(bool force) {
  if (!force && this->mSession != nullptr) {
    return 1;
  }

  if (mSession) delete mSession;
  this->mSession = new Session();

  return 0;
}
