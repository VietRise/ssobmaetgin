#ifndef __CARD_INDEX_CONVERTER_H__
#define __CARD_INDEX_CONVERTER_H__

#include <vector>

class CardIndexConverter
{
private:
protected:
public:
    static int convertServerIndexToClientIndex(const int& serverCardIndex);
    static int convertClientIndexToServerIndex(const int& clientCardIndex);

    static std::vector<int> convertServerIndexesToClientIndexes(const std::vector<int>& serverCardIndex);
    static std::vector<int> convertClientIndexesToServerIndexes(const std::vector<int>& clientCardIndex);
};

#endif
