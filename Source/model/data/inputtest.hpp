#pragma once

#include "model/data/basetest.hpp"

class InputTest: public BaseTest {
public:
    InputTest(
        QString questionText,
        QString rightAnswer
    ): BaseTest(ViewType::INPUT, questionText, rightAnswer){ }


};
