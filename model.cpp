#include "model.hpp"
#include "settings.h"

Session* Model::getSession() const {
  return mSession;
}

Model::Model(QRandomGenerator *rand): mSession(nullptr), mRand(rand) {}

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
  this->mSession = new Session(Settings::sAttemptsMode);

  return 0;
}


int Model::getRandomPosition() {
  if (!mSession) return 0;
  int upperBound = mSession->getTestsCount();
  return mRand->bounded(0, upperBound);
}
