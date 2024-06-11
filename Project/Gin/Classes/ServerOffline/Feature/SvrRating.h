#ifndef __SVR_RATING_H__
#define __SVR_RATING_H__

#include <string>
#include <vector>

typedef struct
{
    std::string name;
    int         enable;
    double      min;
    double      max;
} RatingObjectCondition;

typedef struct
{
    std::string                        name;
    std::vector<RatingObjectCondition> listValue;
} RatingCondition;

class SvrRating
{
private:
    static long long getLastTimeShowRatingPopup();
    static bool checkConditionSuccess(const std::string& conditionName, const RatingObjectCondition& memberCondition, double value);
protected:
public:
    static void initStateRating();
    static void setStateRating(int state);
    static int getStateRating();
    static void getDataConditionRatingPopup();
    static std::string checkConditionToShowRatingPopup(int where);
    static void setLastTimeShowRatingPopup(long long lastTimeShowRating);
    static int getCountShowRatingPopup();
    static void setCountShowRatingPopup(int count);
    static double getConditionValue(const std::string& conditionMemberName, int where = 0);

    static int                          _ratingStateNeverShow;
    static std::string                  _dataRatingJson;
    static std::vector<RatingCondition> _listRatingCondition;
};

#endif //__SVR_RATING_H__
