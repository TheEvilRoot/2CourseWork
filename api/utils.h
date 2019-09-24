#ifndef UTILS_H
#define UTILS_H

#include <QString>

QString& getFromPair(std::pair<QString, QString> &pair, uint index) {
    index %= 2;
    return index == 0 ? pair.first : pair.second;
}

#endif // UTILS_H
