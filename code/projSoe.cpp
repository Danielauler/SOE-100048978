#include <stdio.h>
#include <unistd.h>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>
#include <tgbot/tgbot.h>
#include <unistd.h>
#include <wiringPi.h>

#define SERVO 1

using namespace std;
using namespace TgBot;

void sqwv(int pin, int degree, int N)
{
    int t1 = (100 * degree + 4) / 9 + 1500;
    int t2 = 20000 - t1;
    int i;
    for (i = 0; i < N; i++)
    {
        digitalWrite(pin, HIGH);
        usleep(t1);
        digitalWrite(pin, LOW);
        usleep(t2);
    }
}

void feederFunction(int delayTime, int N)
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
    Bot bot("931015860:AAHG6qZTMlopgG29lXC6-_rAPSmNrKiYXm4");

    InlineKeyboardMarkup::Ptr keyboard(new InlineKeyboardMarkup);
    vector<InlineKeyboardButton::Ptr> row0;
    InlineKeyboardButton::Ptr checkButton(new InlineKeyboardButton);
    checkButton->text = "check";
    checkButton->callbackData = "check";
    row0.push_back(checkButton);
    keyboard->inlineKeyboard.push_back(row0);

    bot.getEvents().onCommand("start", [&bot](Message::Ptr message) {
        bot.getApi().sendMessage(message->chat->id, "Olá, vou te ajudar a manter seu pet alimentado. Use o comando /help para mais informações");
    });

    bot.getEvents().onCommand("Alimentar", [&bot](Message::Ptr message) {
        thread feeder(feederFunction, 2, 40);
        feeder.join();
        bot.getApi().sendMessage(message->chat->id, "Alimentado");
    });

    bot.getEvents().onCommand("Help", [&bot](Message::Ptr message) {
        bot.getApi().sendMessage(message->chat->id, "Você pode: ", false, 0, keyboard);
    });
    bot.getEvents().onCommand("check", [&bot](Message::Ptr message) {
        printf("User wrote %s\n", message->text.c_str());
        if (StringTools::startsWith(message->text, "/start"))
        {
            return;
        }
        bot.getApi().sendMessage(message->chat->id, "Alimentado");
    });
    bot.getEvents().onCallbackQuery([&bot, &keyboard](CallbackQuery::Ptr query) {
        if (StringTools::startsWith(query->data, "check"))
        {
            string response = "ok";
            bot.getApi().sendMessage(query->message->chat->id, response, false, 0, keyboard, "Markdown");
        }
    });
    try
    {
        printf("Bot username: %s\n", bot.getApi().getMe()->username.c_str());
        bot.getApi().deleteWebhook();
        TgLongPoll longPoll(bot);
        while (true)
        {
            printf("Long poll started\n");
            longPoll.start();
        }
    }
    catch (TgException &e)
    {
        printf("error: %s\n", e.what());
    }
    return 0;
}