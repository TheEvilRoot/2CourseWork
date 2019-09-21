#ifndef SESSION_HPP
#define SESSION_HPP

#include "basetest.hpp"

#include <vector>

class Session {
private:
    std::vector<BaseTest*> mTests;
    unsigned int mPosition;
public:
    Session();
    ~Session();
    void nextTest();
    void checkTest(bool result);
    BaseTest * currentTest() const;

};

#endif // SESSION_HPP
