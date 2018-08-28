#ifndef HELPBROWSER_H
#define HELPBROWSER_H

#include "helpdialog.h"

#include <QString>

class HelpManager
{

public:

    static void Init();

    static void showIndex();
    static void showContents();

    static void FreeResources();

private:
    static HelpDialog *viewer;
};

#endif // HELPBROWSER_H
