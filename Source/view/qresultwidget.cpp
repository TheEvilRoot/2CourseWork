#include "qresultwidget.hpp"

QResultWidget::QResultWidget() {
    QFont font1;
    font1.setPointSize(18);

    QFont font;
    font.setPointSize(16);
    font.setBold(true);
    font.setWeight(75);

    QFont font2;
    font2.setPointSize(15);

    QSizePolicy sizePolicy3(QSizePolicy::Preferred, QSizePolicy::Maximum);
    sizePolicy3.setHorizontalStretch(0);
    sizePolicy3.setVerticalStretch(0);

    gridLayout_12 = new QGridLayout();
    gridLayout_12->setObjectName(QString::fromUtf8("gridLayout_12"));
    gridLayout_12->setSizeConstraint(QLayout::SetDefaultConstraint);
    resTestNext = new QPushButton(this);
    resTestNext->setObjectName(QString::fromUtf8("resTestNext"));
    resTestNext->setMinimumSize(QSize(50, 50));
    resTestNext->setMaximumSize(QSize(50, 50));

    gridLayout_12->addWidget(resTestNext, 1, 2, 1, 1);

    resTestPrev = new QPushButton(this);
    resTestPrev->setObjectName(QString::fromUtf8("resTestPrev"));
    resTestPrev->setMinimumSize(QSize(50, 50));
    resTestPrev->setMaximumSize(QSize(50, 50));

    gridLayout_12->addWidget(resTestPrev, 1, 0, 1, 1);

    verticalLayout_2 = new QVBoxLayout();
    verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
    resTestQuestionLabel = new QLabel(this);
    resTestQuestionLabel->setObjectName(QString::fromUtf8("resTestQuestionLabel"));
    QSizePolicy sizePolicy8(QSizePolicy::Preferred, QSizePolicy::MinimumExpanding);
    sizePolicy8.setHorizontalStretch(0);
    sizePolicy8.setVerticalStretch(0);
    sizePolicy8.setHeightForWidth(resTestQuestionLabel->sizePolicy().hasHeightForWidth());
    resTestQuestionLabel->setSizePolicy(sizePolicy8);
    resTestQuestionLabel->setFont(font1);
    resTestQuestionLabel->setScaledContents(false);
    resTestQuestionLabel->setAlignment(Qt::AlignCenter);
    resTestQuestionLabel->setWordWrap(true);

    verticalLayout_2->addWidget(resTestQuestionLabel);

    resTestUserAnswerLabel = new QLabel(this);
    resTestUserAnswerLabel->setObjectName(QString::fromUtf8("resTestUserAnswerLabel"));
    sizePolicy3.setHeightForWidth(resTestUserAnswerLabel->sizePolicy().hasHeightForWidth());
    resTestUserAnswerLabel->setSizePolicy(sizePolicy3);
    resTestUserAnswerLabel->setFont(font2);
    resTestUserAnswerLabel->setStyleSheet(QString::fromUtf8("color: red;"));
    resTestUserAnswerLabel->setAlignment(Qt::AlignCenter);
    resTestUserAnswerLabel->setWordWrap(true);

    verticalLayout_2->addWidget(resTestUserAnswerLabel);

    resTestCorrectLabel = new QLabel(this);
    resTestCorrectLabel->setObjectName(QString::fromUtf8("resTestCorrectLabel"));
    sizePolicy3.setHeightForWidth(resTestCorrectLabel->sizePolicy().hasHeightForWidth());
    resTestCorrectLabel->setSizePolicy(sizePolicy3);
    resTestCorrectLabel->setAlignment(Qt::AlignCenter);

    verticalLayout_2->addWidget(resTestCorrectLabel);


    gridLayout_12->addLayout(verticalLayout_2, 1, 1, 1, 1);

    resTestTitle = new QLabel(this);
    resTestTitle->setObjectName(QString::fromUtf8("resTestTitle"));
    sizePolicy3.setHeightForWidth(resTestTitle->sizePolicy().hasHeightForWidth());
    resTestTitle->setSizePolicy(sizePolicy3);
    resTestTitle->setFont(font);
    resTestTitle->setAlignment(Qt::AlignCenter);

    gridLayout_12->addWidget(resTestTitle, 0, 1, 1, 1);

    resTestTitle->setText("Hello");
    resTestCorrectLabel->setText("Correct");
    resTestQuestionLabel->setText("Question");
    resTestUserAnswerLabel->setText("userAnswer");
    setLayout(gridLayout_12);
}
