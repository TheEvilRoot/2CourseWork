#include "inputtest.hpp"

InputTest::InputTest(
        QString questionText,
        QString rightAnswer
): BaseTest(ViewType::INPUT, questionText, rightAnswer){}
