#pragma once

#include <QtGlobal>

#ifdef Q_OS_MAC
#define setTextFor(__widget, __text) (__widget)->setText(__text); (__widget)->repaint()
#else
#define setTextFor(__widget, __text) (__widget)->setText(__text)
#endif
