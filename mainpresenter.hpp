#ifndef MAINPRESENTER_HPP
#define MAINPRESENTER_HPP

#include "model.hpp"
#include "mainview.hpp"

class MainPresenter {
private:
  Model *mModel;
  MainView *mView;
public:
  MainPresenter(Model *model, MainView *view);

  void proceedAnswer(std::string answer);
  void requestNewSession(bool force = false, bool continueSession = false);
  void initSession();
  void completeTest();
};

#endif // MAINPRESENTER_HPP
