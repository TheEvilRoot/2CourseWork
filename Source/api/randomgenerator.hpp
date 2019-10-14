#ifndef RANDOMGENERATOR_HPP
#define RANDOMGENERATOR_HPP

#include <QtGlobal>
#if QT_VERSION >= QT_VERSION_CHECK(5,10,0)
#define QT_RANDOM_GEN_PRESENT
#include <QRandomGenerator>
#else
#include <ctime>
#include <cmath>
#endif

class RandomGenerator {
private:
#ifdef QT_RANDOM_GEN_PRESENT
    QRandomGenerator *mQtRandomGen;
#endif
public:
    RandomGenerator(unsigned int seed) {
#ifdef QT_RANDOM_GEN_PRESENT
      mQtRandomGen = new QRandomGenerator(seed);
#else
      srand(seed);
#endif
    }

    int intInRange(int upper) {
        int result = 0;
#ifdef QT_RANDOM_GEN_PRESENT
         result = mQtRandomGen->bounded(upper);
#else
        result = rand() % upper;
#endif
        return result;
    }

    int intInRange(int lower, int upper) {
        int result = 0;
#ifdef QT_RANDOM_GEN_PRESENT
        result = mQtRandomGen->bounded(lower, upper);
#else
        result = (rand() % (upper - lower)) + lower;
#endif
        return result;
    }

};

#undef QT_RANDOM_GEN_PRESENT
#endif