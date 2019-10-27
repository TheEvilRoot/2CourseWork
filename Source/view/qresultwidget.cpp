#include "qresultwidget.hpp"

QResultWidget::QResultWidget() {
    QFont font;

    QSizePolicy sizePolicy3(QSizePolicy::Preferred, QSizePolicy::Maximum);
    sizePolicy3.setHorizontalStretch(0);
    sizePolicy3.setVerticalStretch(0);

    QSizePolicy sizePolicy8(QSizePolicy::Preferred, QSizePolicy::MinimumExpanding);
    sizePolicy8.setHorizontalStretch(0);
    sizePolicy8.setVerticalStretch(0);
    sizePolicy8.setHeightForWidth(questionLabel->sizePolicy().hasHeightForWidth());

    gridLayout = new QGridLayout();
    gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
    gridLayout->setSizeConstraint(QLayout::SetDefaultConstraint);
    nextBtn = new QPushButton(this);
    nextBtn->setObjectName(QString::fromUtf8("nextBtn"));
    nextBtn->setMinimumSize(QSize(50, 50));
    nextBtn->setMaximumSize(QSize(50, 50));

    gridLayout->addWidget(nextBtn, 1, 2, 1, 1);

    prevBtn = new QPushButton(this);
    prevBtn->setObjectName(QString::fromUtf8("prevBtn"));
    prevBtn->setMinimumSize(QSize(50, 50));
    prevBtn->setMaximumSize(QSize(50, 50));

    gridLayout->addWidget(prevBtn, 1, 0, 1, 1);

    contentVIew = new QVBoxLayout();
    contentVIew->setObjectName(QString::fromUtf8("contentVIew"));
    questionLabel = new QLabel(this);
    questionLabel->setObjectName(QString::fromUtf8("questionLabel"));
    questionLabel->setSizePolicy(sizePolicy8);

    font.setPointSize(18);
    font.setBold(false);
    questionLabel->setFont(font);

    questionLabel->setScaledContents(false);
    questionLabel->setAlignment(Qt::AlignCenter);
    questionLabel->setWordWrap(true);

    contentVIew->addWidget(questionLabel);

    userAnswerLabel = new QLabel(this);
    userAnswerLabel->setObjectName(QString::fromUtf8("userAnswerLabel"));
    sizePolicy3.setHeightForWidth(userAnswerLabel->sizePolicy().hasHeightForWidth());
    userAnswerLabel->setSizePolicy(sizePolicy3);

    font.setPointSize(15);
    font.setBold(false);
    userAnswerLabel->setFont(font);

    userAnswerLabel->setStyleSheet(QString::fromUtf8("color: red;"));
    userAnswerLabel->setAlignment(Qt::AlignCenter);
    userAnswerLabel->setWordWrap(true);

    contentVIew->addWidget(userAnswerLabel);

    correctLabel = new QLabel(this);
    correctLabel->setObjectName(QString::fromUtf8("correctLabel"));
    sizePolicy3.setHeightForWidth(correctLabel->sizePolicy().hasHeightForWidth());
    correctLabel->setSizePolicy(sizePolicy3);
    correctLabel->setAlignment(Qt::AlignCenter);

    contentVIew->addWidget(correctLabel);


    gridLayout->addLayout(contentVIew, 1, 1, 1, 1);

    titleLabel = new QLabel(this);
    titleLabel->setObjectName(QString::fromUtf8("titleLabel"));
    sizePolicy3.setHeightForWidth(titleLabel->sizePolicy().hasHeightForWidth());
    titleLabel->setSizePolicy(sizePolicy3);

    font.setPointSize(16);
    font.setBold(true);
    titleLabel->setFont(font);

    titleLabel->setAlignment(Qt::AlignCenter);

    gridLayout->addWidget(titleLabel, 0, 1, 1, 1);

    titleLabel->setText("Hello");
    correctLabel->setText("Correct");
    questionLabel->setText("Question");
    userAnswerLabel->setText("userAnswer");
    setLayout(gridLayout);
}
