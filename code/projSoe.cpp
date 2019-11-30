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

int factorial(future<int> &f)
{
    int res = 1;

    int N = f.get();
    for (int i = N; i > 1; i--)
        res *= i;

    cout << "Result is: " << res << endl;
    return res;
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
        if (!std::isnan(message->text))
        {
            int x;
            promise<int> p;
            future<int> f = p.get_future();
            future<int> fu = std::async(std::launch::async, factorial, std::ref(f));

            this_thread::sleep_for(chrono::milliseconds(20));
            p.set_value(message->text);

            x = fu.get();
        }
        bot.getApi().sendMessage(message->chat->id, "Seu número é" + x);
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