#include <stdio.h>
#include <unistd.h>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>
#include <tgbot/tgbot.h>
#include <unistd.h>
#include <wiringPi.h>

#define SERVO 26

using namespace std;

void sqwv(int pin, int degree, int N)
{
	int t1 = (100*degree+4)/9+1500;
	int t2 = 20000-t1;
	int i;
	for(i=0; i<N; i++)
	{
		digitalWrite(pin, HIGH);
		usleep(t1);
		digitalWrite(pin, LOW);
		usleep(t2);
	}
}

void feederFunction(int delayTime, N)
{
	sqwv(SERVO, 90, N);
    sleep(delayTime);
    sqwv(SERVO, 0, N);
};

int main()
{
    int N = 40;
    wiringPiSetup();
    pinMode(SERVO, OUTPUT);
    sqwv(SERVO, 0, N);
    TgBot::Bot bot("931015860:AAHG6qZTMlopgG29lXC6-_rAPSmNrKiYXm4");
    bot.getEvents().onCommand("start", [&bot](TgBot::Message::Ptr message) {
        bot.getApi().sendMessage(message->chat->id, "Hi!");
    });

    // bot.getEvents().onCommand("Alimentar", [&bot](TgBot::Message::Ptr message) {
    //     bot.getApi().sendMessage(message->chat->id, message->text);
    // });
    bot.getEvents().onAnyMessage([&bot](TgBot::Message::Ptr message) {
        printf("User wrote %s\n", message->text.c_str());
        if (StringTools::startsWith(message->text, "/start"))
        {
            return;
        }
        thread feeder(feederFunction, 2, N);
        feeder.join();
        bot.getApi().sendMessage(message->chat->id, "Alimentado");
    });
    try
    {
        printf("Bot username: %s\n", bot.getApi().getMe()->username.c_str());
        TgBot::TgLongPoll longPoll(bot);
        while (true)
        {
            printf("Long poll started\n");
            longPoll.start();
        }
    }
    catch (TgBot::TgException &e)
    {
        printf("error: %s\n", e.what());
    }
    return 0;
}