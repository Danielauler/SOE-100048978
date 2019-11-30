#include <stdio.h>
#include <thread>
#include <iostream>
#include <mutex>
#include <fstream>
#include <chrono>
#include <condition_variable>
#include <future>
#include <tgbot/tgbot.h>

using namespace std;

void factorial(int N)
{
    int res = 1;

    for (int i = N; i > 1; i--)
        res *= i;

    cout << "Result is: " << res << endl;
};

int main()
{
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
        thread feeder(factorial, 4);
        t1.join();
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