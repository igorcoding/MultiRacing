#include "log.h"
#include "game.h"

namespace NeonHockey
{

    Log &Log::getInstance(Level l)
    {
        static Log inst;
        inst._level = l;

        return inst;
    }

    Log::Log()
    {

    }

    Log &Log::operator<<(Log::EndLine &endl)
    {
        _entries.push_back({_level, is.str()});
        is.clear();

        return *this;
    }

    const std::vector<Log::LogEntry> &Log::getEntries()
    {
        return _entries;
    }

    void Log::render(hgeFont *fnt)
    {
        //Game &game = Game::getInstance();
        int i = 0;
        for(auto &entry: _entries)
        {
            if(entry.level == Level::Error)
                fnt->SetColor(ARGB(255, 255, 0, 0));
            else
                fnt->SetColor(ARGB(255, 255, 25, 255));

            fnt->printf(0, (Game::screenHeight() - _height) + i++*_entryHeight, 0,
                        entry.str.c_str());
        }

    }
}
