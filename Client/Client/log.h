#ifndef LOG_H
#define LOG_H
#include <vector>
#include <string>
#include <sstream>
#include <hge.h>
#include <hgefont.h>
#include "game.h"

namespace NeonHockey
{
    class Log
    {
    public:
        enum class Level { Info, Error };

        static Log& getInstance(Level l);

        struct EndLine {} static endl;

        template <typename T>
        Log &operator<<(T &obj)
        {
            is << obj;

            return *this;
        }

        Log &operator<<(EndLine &endl);

        void render(hgeFont* fnt);

        struct LogEntry
        {
            Level level;
            std::string str;
        };

        const std::vector<LogEntry> &getEntries();

    private:
        Log();
        Log(const Log& root) = delete;
        Log& operator=(const Log&) = delete;

        Level _level;
        std::vector<LogEntry> _entries;
        std::stringstream is;

        int _maxEntriesCount = 10;
        int _height = Game::screenHeight()/2;
        int _entryHeight = _height/_maxEntriesCount;
    };
}

#endif // LOG_H
