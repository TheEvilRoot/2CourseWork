#ifndef SESSION_HPP
#define SESSION_HPP

#include "basetest.hpp"

#include <vector>

class Session {
private:
    std::vector<BaseTest*> tests;
    unsigned int position;
public:
    Session();
};

#endif // SESSION_HPP
