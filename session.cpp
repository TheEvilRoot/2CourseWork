#include "session.hpp"

Session::Session(): tests({new BaseTest("Hello World", "Yeah"), new BaseTest("Yeeeee", "me!")}), position(0){}
