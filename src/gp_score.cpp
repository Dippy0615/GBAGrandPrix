#include "gp_score.h"

namespace gp
{
    Score::Score(int level, int millis, int secs, int mins) : _level(level), _millis(millis), _secs(secs), _mins(mins)
    {

    }

    int Score::level()
    {
        return _level;
    }

    int Score::millis()
    {
        return _millis;
    }

    int Score::secs()
    {
        return _secs;
    }

    int Score::mins()
    {
        return _mins;
    }

    bool Score::is_empty()
    {
        return _millis==0 && _secs==0 && _mins==0;
    }

    bool Score::operator<(Score score)
    {
        if(_mins<score.mins()) return true;
        if(_mins<=score.mins() && _secs<score.secs()) return true;
        if(_mins<=score.mins() && _secs<=score.secs() && _millis<score.millis()) return true;
        return false;
    }
}