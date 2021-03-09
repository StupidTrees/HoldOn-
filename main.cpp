#include "main.h"
#include "AppDelegate.h"
#include "cocos2d.h"

#include "DeskTop.h"

int WINAPI _tWinMain(HINSTANCE hInstance,
                       HINSTANCE hPrevInstance,
                      LPTSTR    lpCmdLine,
                      int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);
    AppDelegate app;
    return Application::getInstance()->run();

}

