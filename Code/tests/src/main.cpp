#define CATCH_CONFIG_RUNNER
#include "catch.hpp"
#ifdef _WIN32
#include <windows.h>
#else
#endif
#include <Platform.h>

#ifdef _WIN32
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    TP_UNUSED(hInstance);
    TP_UNUSED(hPrevInstance);
    TP_UNUSED(lpCmdLine);
    TP_UNUSED(nShowCmd);
#else
int main(int aArgc, char** apArgv)
{
    TP_UNUSED(aArgc);
    TP_UNUSED(apArgv);

#endif
    return Catch::Session().run();
}
