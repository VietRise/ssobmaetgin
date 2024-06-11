#ifndef __SVR_LOCAL_NOTIFICATION_H__
#define __SVR_LOCAL_NOTIFICATION_H__

#include <string>
#include <vector>

typedef struct
{
    int         id;
    std::string name;
    std::string type;
    std::string title;
    std::string message;
    std::string image;
    long long   delayTime;
    double      money;
    long long   periodTime;
} LocalNotification;

class SvrLocalNotification
{
private:
    static bool getLocalNotificationDailyBonus(std::vector<LocalNotification>& listNotification,
                                               const int& id,
                                               const std::string& name,
                                               const std::vector<std::string>& type,
                                               const std::string& title,
                                               const std::string& message,
                                               const std::string& image,
                                               const double& delayTime,
                                               const int& number,
                                               const std::vector<double>& validTime);
    static bool getLocalNotificationTimeBonus(std::vector<LocalNotification>& listNotification,
                                              const int& id,
                                              const std::string& name,
                                              const std::vector<std::string>& type,
                                              const std::string& title,
                                              const std::string& message,
                                              const std::string& image,
                                              const double& delayTime,
                                              const int& number,
                                              const std::vector<double>& validTime);
    static bool getLocalNotificationHourlyBonus(std::vector<LocalNotification>& listNotification,
                                                const int& id,
                                                const std::string& name,
                                                const std::vector<std::string>& type,
                                                const std::string& title,
                                                const std::string& message,
                                                const std::string& image,
                                                const double& delayTime,
                                                const int& number,
                                                const std::vector<double>& validTime);
    static bool getLocalNotificationWelcomeBack(std::vector<LocalNotification>& listNotification,
                                                const int& id,
                                                const std::string& name,
                                                const std::vector<std::string>& type,
                                                const std::string& title,
                                                const std::string& message,
                                                const std::string& image,
                                                const double& delayTime,
                                                const int& number,
                                                const std::vector<double>& validTime);
    static bool getLocalNotificationUnfinishedGame(std::vector<LocalNotification>& listNotification,
                                                   const int& id,
                                                   const std::string& name,
                                                   const std::vector<std::string>& type,
                                                   const std::string& title,
                                                   const std::string& message,
                                                   const std::string& image,
                                                   const double& delayTime,
                                                   const int& number,
                                                   const std::vector<double>& validTime);
    static bool isValidTime(double periodTime, const std::vector<double>& validTime);

protected:
public:
    static std::vector<LocalNotification> getLocalNotificationData();
};

#endif //__SVR_LOCAL_NOTIFICATION_H__
