#pragma once
#include <string>
#include <vector>
#include <mutex>
#include <fstream>

enum class LogLevel
{
    Info,
    Warning,
    Error
};

class Logger
{
public:
    static Logger& GetInstance();

    void Log(LogLevel level, const std::string& message);
    void LogInfo(const std::string& message);
    void LogWarning(const std::string& message);
    void LogError(const std::string& message);

    void DrawImGuiConsole();

private:
    Logger();
    ~Logger();

    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    std::string LevelToString(LogLevel level);

    std::vector<std::string> m_LogBuffer;

    std::mutex m_Mutex;
    std::ofstream m_LogFile;

    bool m_ShowOptions = false;

    bool m_ShowInfo = true;
    bool m_ShowWarning = true;
    bool m_ShowError = true;
};
