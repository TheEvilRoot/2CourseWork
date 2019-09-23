#ifndef MAINPRESENTER_HPP
#define MAINPRESENTER_HPP

#include "model.hpp"
#include "mainview.hpp"

#include <QString>
#include <QObject>

class MainPresenter: public QObject {

    Q_OBJECT

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
  void initApplication();

public slots:
    void onProgressDone();
    void onError(QString message);
};

#endif // MAINPRESENTER_HPP
