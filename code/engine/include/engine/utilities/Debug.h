#pragma once

#include <iostream>
#include <fstream>
#include <windows.h>
#include <array>
#include <string>
#include <sstream>
#include <iomanip>
#include <OpenGL/OpenGlInclude.h>

namespace EisEngine {
    using namespace std;

    enum LogPriority{ InfoP = 0, DebugP = 1, WarnP = 2, ErrorP = 3, FatalP = 4};

    #ifndef DEBUG_INFO
    /// Logs an informational message indicating the file and line number where the macro is called.
    /// @param message - std::string: A string to be displayed in the log message.
    #define DEBUG_INFO(message) Debug::Info(__FILE__, __LINE__, message);
    #endif


    #ifndef DEBUG_LOG
    /// Logs a message indicating the file and line number where the macro is called
    /// @param message - std::string: A string to be displayed in the log message.
    #define DEBUG_LOG(message) Debug::Log(__FILE__, __LINE__, message);
    #endif


    #ifndef DEBUG_WARN
    /// Logs a warning message indicating the file and line number where the macro is called.
    /// @param message - std::string: A string to be displayed in the warning message.
    #define DEBUG_WARN(message) Debug::Warn(__FILE__, __LINE__, message);
    #endif


    #ifndef DEBUG_ERROR
    /// Logs an error message indicating the file and line number where the macro is called.
    /// This **does not** interrupt the game, and should be used e.g. in error catching.
    /// @param message - std::string: A string to be displayed in the error message.
    #define DEBUG_ERROR(message) Debug::Error(__FILE__, __LINE__, message);
    #endif


    #ifndef DEBUG_RUNTIME_ERROR
    /// Logs an error message indicating the file and line number where the macro is called.
    /// This **does** interrupt the game when called.
    /// @param message - std::string: A string to be displayed in the error message.
    #define DEBUG_RUNTIME_ERROR(message) Debug::RuntimeError(__FILE__, __LINE__, message);
    #endif


    #ifndef DEBUG_OPENGL
    /// Logs an error message indicating the file and line number where the macro is called.
    /// @param entityName - std::string: An identifier to the entity potentially triggering the error.
    #define DEBUG_OPENGL(entityName) Debug::Check_GL_Error(__FILE__, __LINE__, entityName);
    #endif

    /// A utility class used for debugging purposes - provides more practical console inputs than the standard library's console interactions.
    /// /!\ STATIC CLASS ONLY! Use associated macros DEBUG_[TYPE]!
    class Debug {
    public:
        // Delete constructor, static-only class.
        Debug() = delete;

        /// sends a specific line of text to the console.
        /// @param file - [use __FILE__] -> gives file information to the file the statement is called from.
        /// @param line - [use __LINE__] -> gives the line the statement was called from.
        /// @param text - a string to be sent to the console.
        static void Log(const char* file, int line, const std::string &text){
            CreateLog(DebugP, file, line, text);
        }

        /// sends an information message to the console.
        /// @param file - [use __FILE__] -> gives file information to the file the statement is called from.
        /// @param line - [use __LINE__] -> gives the line the statement was called from.
        /// @param text - a string to be displayed as information.
        static void Info(const char* file, int line, const std::string &text){
            CreateLog(InfoP, file, line, text);
        }

        /// sends a warning message to the console
        /// @param file - [use __FILE__] -> gives file information to the file the statement is called from.
        /// @param line - [use __LINE__] -> gives the line the statement was called from.
        /// @param warning - a string to be displayed as a warning message.
        static void Warn(const char* file, int line, const std::string &warning){
            CreateLog(WarnP, file, line, warning);
        }

        /// sends an error message to the console.
        /// @param file - [use __FILE__] -> gives file information to the file the statement is called from.
        /// @param line - [use __LINE__] -> gives the line the statement was called from.
        /// @param errorText - a string to be displayed as the explanation behind the error message.
        static void Error(const char* file, int line, const std::string &errorText){
            CreateLog(ErrorP, file, line, errorText);
        }

        /// throws a runtime error, crashing the game and logging the error message in the process.
        /// @param file - [use __FILE__] -> gives file information to the file the statement is called from.
        /// @param line - [use __LINE__] -> gives the line the statement was called from.
        /// @param err - the error message to be displayed.
        static void RuntimeError(const char* file, int line, const std::string &err){
            CreateLog(FatalP, file, line, err);
        }

        /// Fetches OpenGL errors and prints them if any are found.
        static void Check_GL_Error(const char* file, int line, const std::string& entityName) {
            // return early if priority is set to only map runtime errors.
            if(LogPriority::ErrorP < Priority)
                return;

            GLenum error = glGetError();
            std::string time = GetTime();

            if (error != GL_NO_ERROR) {
                HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
                // set console colour to GL Error setting :)
                SetConsoleTextAttribute(hConsole, 12);

                // compile error message
                std::stringstream logStream;
                logStream << "["<< file <<"("<< line << ")]:\n";
                logStream << "[OpenGL Error ("<< entityName << ")] - (" << time << "): ";
                logStream << std::to_string(error);

                // print error message & reset console colour to default.
                std::cout << logStream.str() << std::endl;
                SetConsoleTextAttribute(hConsole, 7);
            }
        }

        /// Sets the minimum priority hurdle for debug messages.
        static void SetPriority(LogPriority val) {Priority = val;}
    private:
        /// \n Compiles the log info to a message in the console.
        static void CreateLog(LogPriority priority,
                              const char* file,
                              int line,
                              const std::string &message){
            if(priority < Priority)
                return;

            HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

            std::string time = GetTime();

            std::stringstream logStream;
            logStream << "["<< file <<"("<< line << ")]:\n";
            logStream << "[" << PriorityToString(priority) << "] - (" << time << "): ";
            logStream << message;

            switch(priority){
                // Change console colour if severity is > info/debug, + print before throwing a runtime error when priority is fatal.
                case DebugP:
                case InfoP:
                    break;
                case WarnP:
                    SetConsoleTextAttribute(hConsole, 6);
                    break;
                case ErrorP:
                    SetConsoleTextAttribute(hConsole, 12);
                    break;
                case FatalP:
                    SetConsoleTextAttribute(hConsole, 12);
                    std::cout << logStream.str() << std::endl;
                    SetConsoleTextAttribute(hConsole, 7);
                    throw std::runtime_error(logStream.str());
            }

            // print message & reset console color
            std::cout << logStream.str() << std::endl;
            SetConsoleTextAttribute(hConsole, 7);
        }

        /// Maps every log priority to a string.
        static std::string PriorityToString(LogPriority p){
            switch (p) {
                case InfoP:
                    return "Info";
                case DebugP:
                    return "Debug";
                case WarnP:
                    return "Warning";
                case ErrorP:
                    return "Error";
                case FatalP:
                    return "Fatal";
            }
            return "Unexpected log priority ";
        }

        /// Get current system time.
        static std::string GetTime(){
            time_t currentTime;
            struct tm* localTime;

            time(&currentTime);
            localTime = localtime(&currentTime);

            const std::array<int, 3> time{localTime->tm_hour, localTime->tm_min, localTime->tm_sec};

            return CompileTimeToString(time);
        }

        /// Converts an array of time values to a string displaying hh:mm:ss.
        static std::string CompileTimeToString(const std::array<int, 3> &time)
        {
            std::stringstream ss;
            ss <<
                std::setw(2) << std::setfill('0') << std::to_string(time[0]) << ":" <<
                std::setw(2) << std::setfill('0') << std::to_string(time[1]) << ":" <<
                std::setw(2) << std::setfill('0') << std::to_string(time[2]);
            return ss.str();
        }

        static LogPriority Priority;
    };
}
