#include "InGameDefine.h"

std::map<int, std::map<int, std::map<double, std::vector<double>>>> s_betRange;
std::vector<std::string>                                            s_strNum          = { "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "," };
int                                                                 s_winHitpotNumber = 0;
std::vector<MoneyRangeDefine>                                       s_moneyRangeName;
std::map<int, std::map<int, std::vector<TargetScoreData>>>          s_targetScoreDataRange;
