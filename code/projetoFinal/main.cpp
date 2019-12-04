#include <stdio.h>
#include <unistd.h>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>
#include <csignal>
#include <cstdio>
#include <cstdlib>
#include <tgbot/tgbot.h>
#include <unistd.h>
#include <wiringPi.h>
#include "verificaTigela.h"

#define SERVO 1

using namespace std;
using namespace TgBot;

void takePic()
{
    system("fswebcam 320x240 foto_img.jpg")
}

bool verifyBowl()
{
    bool existencia;
    // thread takePhoto(takePic);
    // takePhoto.join();

    existencia = existencia_racao('foto_img.jpg');
    cout << "tem ração: " << existencia << endl;
    return existencia;
}

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
    const string photoFilePath = "foto_img.jpg";
    const string photoMimeType = "image/jpeg";
    int N = 40;
    wiringPiSetup();
    pinMode(SERVO, OUTPUT);
    sqwv(SERVO, 0, N);
    Bot bot("931015860:AAHG6qZTMlopgG29lXC6-_rAPSmNrKiYXm4");

    InlineKeyboardMarkup::Ptr keyboard(new InlineKeyboardMarkup);
    InlineKeyboardMarkup::Ptr keyboard2(new InlineKeyboardMarkup);
    vector<InlineKeyboardButton::Ptr> row0;
    InlineKeyboardButton::Ptr checkButton(new InlineKeyboardButton);
    InlineKeyboardButton::Ptr checkButton2(new InlineKeyboardButton);
    InlineKeyboardButton::Ptr checkButton3(new InlineKeyboardButton);
    InlineKeyboardButton::Ptr checkButton4(new InlineKeyboardButton);
    vector<InlineKeyboardButton::Ptr> row1;
    checkButton->text = "alimentar";
    checkButton->callbackData = "alimentar";
    row0.push_back(checkButton);
    keyboard->inlineKeyboard.push_back(row0);
    checkButton2->text = "agendar uma refeição";
    checkButton2->callbackData = "agendar";
    row1.push_back(checkButton2);
    keyboard->inlineKeyboard.push_back(row1);

    checkButton3->text = "Alimentar mesmo assim";
    checkButton3->callbackData = "alimentarSemVerificar";
    row0.push_back(checkButton3);
    keyboard2->inlineKeyboard.push_back(row0);
    checkButton4->text = "Cancelar";
    checkButton4->callbackData = "cancelar";
    row1.push_back(checkButton4);
    keyboard2->inlineKeyboard.push_back(row1);

    bot.getEvents().onCommand("start", [&bot](Message::Ptr message) {
        bot.getApi().sendMessage(message->chat->id, "Olá, vou te ajudar a manter seu pet alimentado. Use o comando /help para mais informações");
    });

    bot.getEvents().onCommand("alimentarSemVerificar", [&bot](Message::Ptr message) {
        thread feeder(feederFunction, 2, 40);
        feeder.join();
        bot.getApi().sendMessage(message->chat->id, "Alimentado");
    });

    bot.getEvents().onCommand("alimentar", [&bot, &photoFilePath, &photoMimeType, &keyboard2](Message::Ptr message) {
        bool existencia = verifyBowl();
        if (!existencia)
        {
            thread feeder(feederFunction, 2, 40);
            feeder.join();
            string response = "Ok, alimentado";
            bot.getApi().sendMessage(query->message->chat->id, response);
        }
        else
        {
            bot.getApi().sendMessage(message->chat->id, "A tigela ainda está cheia!", false, 0, keyboard2);
            bot.getApi().sendPhoto(message->chat->id, InputFile::fromFile(photoFilePath, photoMimeType));
        }
    });

    bot.getEvents().onCommand("agendar", [&bot, &keyboard](Message::Ptr message) {
        string response = "ok";
        bot.getApi().sendMessage(message->chat->id, response, false, 0, keyboard, "Markdown");
    });

    bot.getEvents().onCommand("help", [&bot, &keyboard](Message::Ptr message) {
        bot.getApi().sendMessage(message->chat->id, "Você pode: ", false, 0, keyboard);
    });

    bot.getEvents().onCallbackQuery([&bot, &keyboard](CallbackQuery::Ptr query) {
        if (StringTools::startsWith(query->data, "alimentar"))
        {
            bool existencia = verifyBowl();
            if (!existencia)
            {
                thread feeder(feederFunction, 2, 40);
                feeder.join();
                string response = "Ok, alimentado";
                bot.getApi().sendMessage(query->message->chat->id, response);
            }
            else
            {
                bot.getApi().sendMessage(message->chat->id, "A tigela ainda está cheia!", false, 0, keyboard2);
            }
        }
    });

    bot.getEvents().onCallbackQuery([&bot, &keyboard](CallbackQuery::Ptr query) {
        if (StringTools::startsWith(query->data, "cancel"))
        {
            string response = "ok";
            bot.getApi().sendMessage(message->chat->id, response);
        }
    });

    signal(SIGINT, [](int s) {
        printf("SIGINT got\n");
        exit(0);
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