#include "session.hpp"

#include <iostream>

Session::Session(): mTests({new BaseTest(ViewType::CHOICE, "Hello World", "Yeah"), new BaseTest(ViewType::INPUT, "Yeeeee", "me!")}), mPosition(0){}

Session::~Session() {
  std::cerr << "Session descruction initiated!!!!!!\n";
}

void Session::nextTest() {
  mPosition++;
}

void Session::checkTest(bool result) {
  // TODO: Write to history
}

BaseTest* Session::currentTest() const {
  if (mPosition < mTests.size()){
    return mTests[mPosition];
  } else {
    return nullptr;
  }
}

bool Session::isFinished() {
  return mPosition >= mTests.size();
}

int Session::getTestsCount() const {
  return mTests.size();
}
