// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"

#include <Windows.h>
#include <psapi.h>
#include <string>
#include <format>
#include <algorithm>

const std::wstring SEUCCESS_TITLE(L"Success");

const std::wstring ERROR_TITLE(L"Error");

const std::wstring CORRECT_PROCESS_NAME(L"notepad.exe");


class SmartHandle final
{
public:
    SmartHandle(HANDLE handle) :
        m_handle(handle)
    {}

    ~SmartHandle()
    {
        try
        {
            if (nullptr != m_handle)
            {
                CloseHandle(m_handle);
            }
        }
        catch (...) {}
    }

    HANDLE get() const
    {
        return m_handle;
    }
    
    bool isValid() const
    {
        return nullptr != m_handle;
    }

private:
    HANDLE m_handle;
};

void run()
{    
    SmartHandle myHandle(GetCurrentProcess());
    if (myHandle.isValid())
    {
        std::wstring name;
        wchar_t moudlueName[MAX_PATH];
        if (GetModuleBaseName(myHandle.get(), nullptr, moudlueName, MAX_PATH))
        {
            auto pid = GetCurrentProcessId();
            name = std::wstring(moudlueName);
            transform(name.begin(), name.end(), name.begin(), towlower);

            std::wstring msg;
            std::wstring title;
            if (name == CORRECT_PROCESS_NAME)
            {
                msg = std::format(L"YES! im in the right process!\nprocess: {}\nPID: {}", name, pid);
                title = SEUCCESS_TITLE;
            }
            else
            {
                msg = std::format(L"No, that is not the right process to run me in :(\nprocess: {}\nPID: {}", name, pid);
                title = ERROR_TITLE;
            }
            std::ignore = MessageBox(nullptr, msg.c_str(), title.c_str(), MB_OK);
        }        
    }
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        run();        
        break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

extern "C" __declspec(dllexport) void pleaseDontCallMe()
{
    MessageBox(nullptr, L"Hey how did you get here?\nIts not part of the assignment!", 0, 0);
}