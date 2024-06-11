#include "CardIndexConverter.h"
#include "ServerOffline/SvrCard.h"

#define GIN_OFFSET 0

int CardIndexConverter::convertServerIndexToClientIndex(const int& serverCardIndex)
{
    if (serverCardIndex == -1)
        return -1;
    return (serverCardIndex + GIN_OFFSET) % SvrCard::Common::MAX_CARD;
}

int CardIndexConverter::convertClientIndexToServerIndex(const int& clientCardIndex)
{
    if (clientCardIndex == -1)
        return -1;
    int index = clientCardIndex - GIN_OFFSET;
    if (index < 0)
        index += SvrCard::Common::MAX_CARD;
    return index;
}

std::vector<int> CardIndexConverter::convertServerIndexesToClientIndexes(const std::vector<int>& serverCardIndex)
{
    std::vector<int> result;
    for (const int& index : serverCardIndex)
        result.push_back(convertServerIndexToClientIndex(index));
    return result;
}

std::vector<int> CardIndexConverter::convertClientIndexesToServerIndexes(const std::vector<int>& clientCardIndex)
{
    std::vector<int> result;
    for (const int& index : clientCardIndex)
        result.push_back(convertClientIndexToServerIndex(index));
    return result;
}