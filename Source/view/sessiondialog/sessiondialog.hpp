#pragma once

#include "presenter/main/mainpresenter.hpp"

#include <QDialog>

namespace Ui {
  class SessionDialog;
}

class SessionDialog : public QDialog {
  Q_OBJECT

public:
  explicit SessionDialog(MainPresenter *presenter, QWidget *parent = nullptr);
  ~SessionDialog();

private:
  Ui::SessionDialog *ui;
  MainPresenter *mPresenter;

  void initDialog();
};
