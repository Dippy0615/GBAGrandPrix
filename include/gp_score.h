#ifndef GP_SCORE_H
#define GP_SCORE_H

namespace gp
{
    class Score
    {
        private:
            int _level;
            int _millis;
            int _secs;
            int _mins;
        public:
            Score(int level, int millis, int secs, int mins);
            int level();
            int millis();
            int secs();
            int mins();
            bool is_empty();
            bool operator<(Score score);
    };
}

#endif