#include "Logger.h"
#include "imgui/imgui.h"
#include <ctime>

#include <chrono>
#include <iomanip>
#include <sstream>

#include <fstream>

Logger& Logger::GetInstance()
{
    static Logger instance;
    return instance;
}

static std::string GetTimestamp()
{
    using namespace std::chrono;

    auto now = system_clock::now();
    auto ms = duration_cast<milliseconds>(now.time_since_epoch()) % 1000;

    std::time_t t = system_clock::to_time_t(now);
    std::tm tm{};
#if defined(_WIN32)
    localtime_s(&tm, &t);
#else
    localtime_r(&t, &tm);
#endif

    std::ostringstream oss;
    oss << "["
        << std::setfill('0')
        << std::setw(2) << tm.tm_hour << ":"
        << std::setw(2) << tm.tm_min << ":"
        << std::setw(2) << tm.tm_sec << "."
        << std::setw(3) << ms.count()
        << "]";

    return oss.str();
}

Logger::Logger()
{
    m_LogFile.open("log.txt", std::ios::out | std::ios::app);

    // Show previous log 
    /*
    std::ifstream in("log.txt");
    if (in.is_open())
    {
        std::string line;
        while (std::getline(in, line))
            m_LogBuffer.push_back(line);
    }
    */
}

Logger::~Logger()
{
    if (m_LogFile.is_open())
        m_LogFile.close();
}

std::string Logger::LevelToString(LogLevel level)
{
    switch (level)
    {
        case LogLevel::Info: return "[INFO]";
        case LogLevel::Warning: return "[WARN]";
        case LogLevel::Error: return "[ERROR]";
    }
    return "[UNKNOWN]";
}

void Logger::Log(LogLevel level, const std::string& message)
{
    std::lock_guard<std::mutex> lock(m_Mutex);

    std::string finalMessage =
        GetTimestamp() + " " +
        LevelToString(level) + " " +
        message;
    m_LogBuffer.push_back(finalMessage);

    if (m_LogFile.is_open())
    {
        m_LogFile << finalMessage << std::endl;
        m_LogFile.flush();
    }
}

void Logger::LogInfo(const std::string& message)
{
    Log(LogLevel::Info, message);
}

void Logger::LogWarning(const std::string& message)
{
    Log(LogLevel::Warning, message);
}

void Logger::LogError(const std::string& message)
{
    Log(LogLevel::Error, message);
}

void Logger::DrawImGuiConsole()
{
    ImGui::Begin("Game Log");

    if (ImGui::Button("Options"))
        m_ShowOptions = !m_ShowOptions;
    
    ImGui::SameLine();

    if (ImGui::Button("Clear"))
        m_LogBuffer.clear();

    ImGui::SameLine();

    if (ImGui::Button("Test Info"))
        Logger::GetInstance().LogInfo("This is a test info message");
    
    ImGui::SameLine();

    if (ImGui::Button("Test Warning"))
        Logger::GetInstance().LogWarning("This is a test warning message");

    ImGui::SameLine();

    if (ImGui::Button("Test Error"))
        Logger::GetInstance().LogError("This is a test error message");

    ImGui::Separator();

    if (m_ShowOptions)
    {
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
        ImGui::Checkbox("Info", &m_ShowInfo);
        ImGui::PopStyleColor();

        ImGui::SameLine();

        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.70f, 0.25f, 1.0f));
        ImGui::Checkbox("Warning", &m_ShowWarning);
        ImGui::PopStyleColor();

        ImGui::SameLine();

        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.2f, 0.2f, 1.0f));
        ImGui::Checkbox("Error", &m_ShowError);
        ImGui::PopStyleColor();

        ImGui::Separator();
    }

    ImGui::BeginChild(
        "LogScrollRegion",
        ImVec2(0, 0),
        false,
        ImGuiWindowFlags_HorizontalScrollbar
    );

    for (const auto& msg : m_LogBuffer)
    {
        if (msg.find("[INFO]") != std::string::npos && !m_ShowInfo)
            continue;
        if (msg.find("[WARN]") != std::string::npos && !m_ShowWarning)
            continue;
        if (msg.find("[ERROR]") != std::string::npos && !m_ShowError)
            continue;

        if (msg.find("[WARN]") != std::string::npos)
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.70f, 0.25f, 1.0f));
        else if (msg.find("[ERROR]") != std::string::npos)
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.2f, 0.2f, 1.0f));
        else
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));

        ImGui::TextWrapped("%s", msg.c_str());
        ImGui::PopStyleColor();
    }

    if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
        ImGui::SetScrollHereY(1.0f);

    ImGui::EndChild();
    ImGui::End();
}
