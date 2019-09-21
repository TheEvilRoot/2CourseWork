#ifndef MAINVIEW_HPP
#define MAINVIEW_HPP

#include <string>

#include "viewtype.h"

class MainView { // TODO: Solve warning
public:
  virtual ~MainView() {}
  virtual bool presentView(const ViewType *type) = 0;
  virtual void showLoading() = 0;
  virtual void hideLoading() = 0;
  virtual void showMessage(std::string message) = 0;
  virtual void disableContent() = 0;
  virtual void enableContent() = 0;
  virtual void askSession() = 0;
};

#endif // MAINVIEW_HPP
