#include "DeviceInfo.h"

DeviceInfo::DeviceInfo()
{
    this->_deviceLanguage        = "";
    this->_deviceLanguageCountry = "";
}

DeviceInfo::~DeviceInfo()
{
}

void DeviceInfo::setDeviceLanguage(const std::string& language)
{
    this->_deviceLanguage = language;
}

void DeviceInfo::setDeviceLanguageCountry(const std::string& country)
{
    this->_deviceLanguageCountry = country;
}

const std::string& DeviceInfo::getDeviceLanguage()
{
    return this->_deviceLanguage;
}

const std::string& DeviceInfo::getDeviceLanguageCountry()
{
    return this->_deviceLanguageCountry;
}

