#include "pch.hpp"
#include <logger/logger.hpp>

#if defined(SDK_ENABLE_LOGGING) && defined(_WIN32)
#include <windows.h>
#include <iostream>
#include <chrono>
#include <thread>

// This callback runs when the game crashes
LONG WINAPI CrashHandler(EXCEPTION_POINTERS* exceptionInfo) {
    std::cout << "\n=========================================\n";
    std::cout << "[cs2-sdk] CRASH DETECTED!\n";
    std::cout << "Exception Code: 0x" << std::hex << exceptionInfo->ExceptionRecord->ExceptionCode << std::dec << '\n';
    std::cout << "Console locked. Close this window manually to exit.\n";
    std::cout << "=========================================\n";

    // Infinite loop keeps the process alive and the console open
    // until you click the "X" close button on the window.
    while (true) {
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }

    return EXCEPTION_EXECUTE_HANDLER; 
}
#endif

void CLogger::Initialize() {
#if defined(SDK_ENABLE_LOGGING) && defined(_WIN32)
    AllocConsole();
    SetConsoleTitleA("cs2-sdk console");

    freopen_s(&m_ConsoleStream, "CONOUT$", "w", stdout);

    // Register the crash handler to catch exceptions
    SetUnhandledExceptionFilter(CrashHandler);
#endif
}

void CLogger::LogStringInternal(const std::string& str) {
#ifdef SDK_ENABLE_LOGGING
    std::cout << "[cs2-sdk] " << str << '\n';
#endif
}

CLogger::~CLogger() {
#if defined(SDK_ENABLE_LOGGING) && defined(_WIN32)
    SetUnhandledExceptionFilter(nullptr);

    if (m_ConsoleStream) {
        fclose(m_ConsoleStream);
    }
    FreeConsole();
#endif
}
