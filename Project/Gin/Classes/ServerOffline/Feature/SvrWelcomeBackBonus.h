#ifndef __SVR_WELCOMEBACK_BONUS_H__
#define __SVR_WELCOMEBACK_BONUS_H__

#include <string>
#include <vector>

class SvrWelcomeBackBonus
{
private:
protected:
public:
    static std::pair<int, double> getWelcomeBackBonus(int userlevel, double dayDiff);
    static bool isWelcomeBackEnable();
    static std::vector<std::pair<int, double>> getWelcomeBackBonus(int userlevel);
};

#endif //__SVR_WELCOMEBACK_BONUS_H__
