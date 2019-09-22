#ifndef MAINPRESENTER_HPP
#define MAINPRESENTER_HPP

#include "model.hpp"
#include "mainview.hpp"

#include <QString>

class MainPresenter {
private:
  Model *mModel;
  MainView *mView;
public:
  MainPresenter(Model *model, MainView *view);

  void proceedAnswer(QString answer);
  void requestNewSession(bool force = false, bool continueSession = false);
  void handleSession();
  void completeTest();
  void initView(const ViewType *type);
};

#endif // MAINPRESENTER_HPP
