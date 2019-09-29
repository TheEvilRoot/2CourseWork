#pragma once

#include <QString>

QString& getFromPair(std::pair<QString, QString> &pair, uint index) {
    index %= 2;
    return index == 0 ? pair.first : pair.second;
}
