/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#include <SFML/Graphics.hpp>
#include <fstream>

#ifdef DEBUGMODE
#include <iostream>
#endif

extern "C"
{
    #include <fmod.h>
    #ifndef LINUX
    #include <windows.h>
    #include <shlwapi.h>
    #include <shlobj.h>
    #else
    #include <unistd.h>
    #endif
}

#ifdef LINUX
#include <QMessageBox>
#endif

#include "../../Headers/gglobals.hpp"

#define FADESPEED 4
#define SIZESELECTED 0.85

using namespace sf;
using namespace std;

static bool InitAssets();
static void UpdateAssets();

Texture* titleTex;
Texture* buttonTex[3];
Texture* screenBackTex;
Sprite* title;
Sprite* button[2];
Sprite* screenBack;

FMOD_SOUND* mouseSound;
FMOD_SOUND* clickSound;
FMOD_SOUND* enterLevel;

bool animDone;
bool menuBlock;

unsigned int timeb;

float titleSpeed;
float buttonSpeed[2];

extern Uint8 fadeAlpha;
extern RectangleShape* fadeRect;

bool buttonSelect[2];

extern Texture* circleFadeTex;
extern Sprite* circleSpr;
extern RectangleShape* blackBorder;
extern bool fadeCircle;

bool Scene::Title()
{
    bool exitLoop = false;

    if (!InitAssets())
    {
#ifndef LINUX
        MessageBox(NULL, "Failed to initialize the assets on the Title !", "Assets Error !", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
        QMessageBox messageBox(QMessageBox::Critical, QStringLiteral("Assets Error !"), QStringLiteral("Failed to initialize the assets on the Title !"), QMessageBox::Ok);
        messageBox.exec();
#endif
        mainWindow->close();
        exitLoop = true;
    }

    Event eventSystem;
    bool toGo = false;

    while (!exitLoop)
    {
        while (mainWindow->pollEvent(eventSystem))
        {
            switch (eventSystem.type)
            {
                case Event::Closed      :
                    exitLoop = true;

                    mainWindow->close();

                    break;

                case Event::KeyPressed  :

                    if (eventSystem.key.code == Keyboard::Escape)
                    {
                        if (fadeCircle)
                            break;

                        extern bool showCursor;
                        mainWindow->setMouseCursorVisible(true);
#ifndef LINUX
                        if (MessageBox(NULL, "Do you want to quit ?", "Quit ?", MB_YESNO | MB_ICONQUESTION | MB_TASKMODAL) == IDYES)
#else
                        QMessageBox messageBox(QMessageBox::Question, QStringLiteral("Quit ?"), QStringLiteral("Do you want to quit ?"), QMessageBox::Yes | QMessageBox::No);
                        if (messageBox.exec() == QMessageBox::Yes)
#endif
                        {
                            exitLoop = true;

                            mainWindow->close();
                        }
                        mainWindow->setMouseCursorVisible(showCursor);
                        break;
                    }

                    if (!animDone)
                    {
                        timeb = 300;
                        animDone = true;

                        title->setPosition(320, 87);

                        button[0]->setPosition(200, 300);
                        button[1]->setPosition(440, 300);
                    }

                    break;

                case Event::MouseMoved  :

                    mpos_absolute = Vector2i((eventSystem.mouseMove.x - wpos.x) / windowScale, (eventSystem.mouseMove.y - wpos.y) / windowScale);

                    if (animDone && !menuBlock)
                    {
                        if (mpos_absolute.x > 127 && mpos_absolute.x < 273 && mpos_absolute.y > 250  && mpos_absolute.y < 350)
                        {
                            if (!buttonSelect[0])
                                FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(0), mouseSound, 0, NULL);

                            buttonSelect[0] = true;
                        }
                        else
                            buttonSelect[0] = false;

                        if (mpos_absolute.x > 367 && mpos_absolute.x < 513 && mpos_absolute.y > 250  && mpos_absolute.y < 350)
                        {
                            if (!buttonSelect[1])
                                FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(0), mouseSound, 0, NULL);

                            buttonSelect[1] = true;
                        }
                        else
                            buttonSelect[1] = false;
                    }

                    break;

                case Event::MouseButtonPressed :

                    if (!animDone)
                    {
                        timeb = 300;
                        animDone = true;

                        title->setPosition(320, 87);

                        button[0]->setPosition(200, 300);
                        button[1]->setPosition(440, 300);
                    }

                    if (menuBlock || eventSystem.mouseButton.button != Mouse::Left)
                        break;

                    if (buttonSelect[0])
                    {
                        menuBlock = true;
                        toGo = true;

                        fadeCircle = true;

                        circleFadeTex = new Texture;

                        if (!circleFadeTex->loadFromFile("Data/Gfx/CircleFade.bmp"))
                        {
#ifndef LINUX
                            MessageBox(NULL, "Failed to load Texture :\nData/Gfx/CircleFade.bmp", "Failed to load Texture", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
                            QMessageBox messageBox(QMessageBox::Critical, QStringLiteral("Failed to load Texture"), QStringLiteral("Failed to load Texture :\nData/Gfx/CircleFade.bmp"), QMessageBox::Ok);
                            messageBox.exec();
#endif

                            mainWindow->close();

                            return false;
                        }

                        circleFadeTex->setSmooth(true);

                        circleSpr = new Sprite(*circleFadeTex);
                        circleSpr->setPosition(320, 240);
                        circleSpr->setScale(1.5, 1.5);
                        circleSpr->setOrigin(240, 240);

                        FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(1), enterLevel, 0, NULL);

                        FMOD_Channel_Stop(musicChannel);

                        buttonSelect[0] = false;
                    }
                    else if (buttonSelect[1])
                    {
                        extern bool showCursor;
                        mainWindow->setMouseCursorVisible(true);
#ifndef LINUX
                        int userAnswer;
#endif
                        FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(1), clickSound, 0, NULL);
#ifndef LINUX
                        userAnswer = MessageBox(NULL, "Do you want to quit ?", "Quit ?", MB_YESNO | MB_ICONQUESTION | MB_TASKMODAL);
                        if (userAnswer == IDYES)
#else
                        QMessageBox messageBox(QMessageBox::Question, QStringLiteral("Quit ?"), QStringLiteral("Do you want to quit ?"), QMessageBox::Yes | QMessageBox::No);
                        if (messageBox.exec() == QMessageBox::Yes)
#endif
                        {
                            exitLoop = true;

                            mainWindow->close();

                            break;
                        }
                        mainWindow->setMouseCursorVisible(showCursor);
                    }

                    break;

                case Event::Resized :

                    windowScale = min(eventSystem.size.width / 640.0, eventSystem.size.height / 480.0);
                    wpos = Vector2f((eventSystem.size.width - (windowScale * 640)) / 2, (eventSystem.size.height - (windowScale * 480)) / 2);

                    mainWindow->setView(View(FloatRect(0, 0, eventSystem.size.width, eventSystem.size.height)));

                    textureRender.setPosition(wpos);
                    textureRender.setScale(windowScale, windowScale);

                    break;

                default             : break;
            }
        }

        UpdateAssets();

        if (toGo)
        {
            if (fadeAlpha > FADESPEED)
                fadeAlpha -= FADESPEED;
            else
                fadeAlpha = 0;

            float circleScale = circleSpr->getScale().x;

            if (circleScale > 0.01)
                circleSpr->scale(0.96, 0.96);
            else
            {
                exitLoop = true;
                targetScene = SCENE_INGAME;
            }
        }
        else
        {
            if (fadeAlpha > FADESPEED)
                fadeAlpha -= FADESPEED;
            else
                fadeAlpha = 0;
        }

        if (animDone)
        {
            if (timeb < 12)
                timeb++;
            else
                timeb = 0;
        }
        else
        {
            if (timeb < 300)
                timeb++;
        }

        mainTexture.draw(*screenBack);
        mainTexture.draw(*title);

        if (timeb > 175 || animDone)
        {
            mainTexture.draw(*button[0]);
            mainTexture.draw(*button[1]);
        }

        if (frame_Water < 23)
            frame_Water++;
        else
            frame_Water = 0;

        fadeRect->setFillColor(Color(0, 0, 0, fadeAlpha));

        if (fadeAlpha > 0)
            mainTexture.draw(*fadeRect);

        if (fadeCircle)
        {
            Vector2f circleSize = circleSpr->getScale();

            mainTexture.draw(*circleSpr, BlendMultiply);

            blackBorder->setPosition(0, 0);
            blackBorder->setSize(Vector2f(320 - (240 * circleSize.x), 480));

            mainTexture.draw(*blackBorder);

            blackBorder->setPosition(320 + (240 * circleSize.x), 0);
            blackBorder->setSize(Vector2f(320 - (240 * circleSize.x), 480));

            mainTexture.draw(*blackBorder);

            blackBorder->setPosition(0, 0);
            blackBorder->setSize(Vector2f(640, 240 - (240 * circleSize.y)));

            mainTexture.draw(*blackBorder);

            blackBorder->setPosition(0, 240 + (240 * circleSize.y));
            blackBorder->setSize(Vector2f(640, 240 - (240 * circleSize.y)));

            mainTexture.draw(*blackBorder);
        }

        if (exitLoop)
            mainTexture.clear(Color::Black);

        cursor->setPosition(static_cast<Vector2f>(mpos_absolute));

        mainTexture.draw(*cursor);

        if (exitLoop)
            mainTexture.draw(*loading);

        mainTexture.display();

        mainWindow->clear(Color::Black);
        mainWindow->draw(textureRender);
        mainWindow->display();
    }

    delete fadeRect;

    if (circleFadeTex != NULL)
        delete circleFadeTex;

    if (circleSpr != NULL)
        delete circleSpr;

    delete titleTex;
    delete buttonTex[0];
    delete buttonTex[1];
    delete buttonTex[2];
    delete screenBackTex;

    delete blackBorder;

    delete title;
    delete button[0];
    delete button[1];
    delete screenBack;

    FMOD_Sound_Release(clickSound);
    FMOD_Sound_Release(mouseSound);
    FMOD_Sound_Release(enterLevel);

    return true;
}

static bool InitAssets()
{
    bool allright = true;

    FMOD_RESULT result;

    animDone = false;
    menuBlock = false;

    fadeCircle = false;

    timeb = 0;

    titleSpeed = 7;

    buttonSpeed[0] = 11;
    buttonSpeed[1] = 11;

    buttonSelect[0] = false;
    buttonSelect[1] = false;

    titleTex = new Texture;

    if (!titleTex->loadFromFile("Data/Gfx/TitleScreen/Title.png"))
        allright = false;

    screenBackTex = new Texture;

    if (!screenBackTex->loadFromFile("Data/Gfx/TitleScreen/Titlescreen.png"))
        allright = false;

    circleFadeTex = NULL;
    circleSpr = NULL;

    blackBorder = new RectangleShape;
    blackBorder->setFillColor(Color::Black);

    buttonTex[0] = new Texture;
    buttonTex[1] = new Texture;
    buttonTex[2] = new Texture;

    if (!buttonTex[0]->loadFromFile("Data/Gfx/TitleScreen/Button_PlayLevel.png"))
        allright = false;

    if (!buttonTex[1]->loadFromFile("Data/Gfx/TitleScreen/Button_PlayScenario.png"))
        allright = false;

    if (!buttonTex[2]->loadFromFile("Data/Gfx/TitleScreen/Button_Quit.png"))
        allright = false;

    buttonTex[0]->setSmooth(true);
    buttonTex[1]->setSmooth(true);
    buttonTex[2]->setSmooth(true);

    title = new Sprite(*titleTex);
    screenBack = new Sprite(*screenBackTex);

    button[0] = new Sprite(*buttonTex[0]);
    button[1] = new Sprite(*buttonTex[2]);

    title->setPosition(320, -80);
    title->setOrigin(320, 90);

    button[0]->setPosition(-94, 300);
    button[1]->setPosition(734, 300);

    button[0]->setOrigin(76, 50);
    button[1]->setOrigin(77, 50);

    fadeRect = new RectangleShape(Vector2f(640, 480));
    fadeRect->setFillColor(Color::Black);

    result = FMOD_System_PlaySound(soundSystem,
                      static_cast<FMOD_CHANNELINDEX>(20),
                      music,
                      0,
                      &musicChannel);

    if (result != FMOD_OK)
        allright = false;

    result = FMOD_System_CreateSound(soundSystem,
                                     "Data/Sfx/Menu1.wav",
                                     FMOD_DEFAULT,
                                     NULL,
                                     &mouseSound);

    if (result != FMOD_OK)
        allright = false;

    result = FMOD_System_CreateSound(soundSystem,
                                     "Data/Sfx/Menu2.wav",
                                     FMOD_DEFAULT,
                                     NULL,
                                     &clickSound);

    if (result != FMOD_OK)
        allright = false;

    result = FMOD_System_CreateSound(soundSystem,
                                     "Data/Sfx/EnterLevel.wav",
                                     FMOD_DEFAULT,
                                     NULL,
                                     &enterLevel);

    if (result != FMOD_OK)
        allright = false;

    return allright;
}

static void UpdateAssets()
{
    fadeRect->setFillColor(Color(0, 0, 0, fadeAlpha));

    if (!animDone)
    {
        if (timeb > 80 && timeb < 145)
        {
            if (titleSpeed > 0)
            {
                title->move(0, titleSpeed);
                titleSpeed -= 0.15;
            }
            else
            {
                title->setPosition(320, 87);
                titleSpeed = 0;
            }
        }
        else if (timeb > 145)
        {
            if (buttonSpeed[0] > 0)
            {
                button[0]->move(buttonSpeed[0], 0);
                buttonSpeed[0] -= 0.21;
            }
            else
            {
                button[0]->setPosition(200, 300);
                buttonSpeed[0] = 0;
            }

            if (timeb > 160)
            {
                if (buttonSpeed[1] > 0)
                {
                    button[1]->move(-buttonSpeed[1], 0);
                    buttonSpeed[1] -= 0.21;
                }
                else
                {
                    button[1]->setPosition(440, 300);
                    buttonSpeed[1] = 0;

                    animDone = true;
                }
            }
        }
    }
    else
    {
        for (unsigned int i = 0; i < 2; i++)
        {
            float scale = button[i]->getScale().x;

            if (buttonSelect[i])
            {
                if (scale > SIZESELECTED + 0.05)
                    scale -= 0.05;
                else
                    scale = SIZESELECTED;
            }
            else
            {
                if (scale < 0.95)
                    scale += 0.05;
                else
                    scale = 1;
            }

            button[i]->setScale(scale, scale);
        }
    }
}

bool checkLoadResources(ifstream& levelFile, const char* filename)
{
    extern char procPath[MAX_PATH];
    char filePath[MAX_PATH];
    char getString[MAX_PATH];
    char CMLid[4];
    ifstream checkFile;

    strcpy(filePath, filename);
#ifndef LINUX
    char messageText[512];

    PathRemoveFileSpec(filePath);
    SetCurrentDirectory(filePath);
#else
    basename(filePath);
    chdir(filePath);
#endif
    levelFile.read(CMLid, 4);

    if (CMLid[0] != 'C' || CMLid[1] != 'M' || CMLid[2] != 'L')
    {
#ifndef LINUX
        MessageBox(NULL, "This file is not a valid CML Level !", "Error !", MB_OK | MB_TASKMODAL | MB_ICONERROR);
#else
        QMessageBox messageBox(QMessageBox::Critical, QStringLiteral("Error !"), QStringLiteral("This file is not a valid CML Level !"), QMessageBox::Ok);
        messageBox.exec();
#endif
        return false;
    }

    if (CMLid[3] > EDITOR_VERSION)
    {
#ifndef LINUX
        MessageBox(NULL, "This Level was made with an Higher Version of Mario Constructor Master Editor !", "Error !", MB_OK | MB_TASKMODAL | MB_ICONERROR);
#else
        QMessageBox messageBox(QMessageBox::Critical, QStringLiteral("Error !"), QStringLiteral("This Level was made with an Higher Version of Mario Constructor Master Editor !"), QMessageBox::Ok);
        messageBox.exec();
#endif
        return false;
    }

    for (register unsigned int i = 0; i < 5; i++)
    {
        levelFile.read(getString, 1);

        if (getString[0] != '\0')
        {
            levelFile.seekg(-1, ios::cur);

            for (register unsigned int j = 0; true; j++)
            {
                levelFile.read(&getString[j], 1);

                if (getString[j] == '\0')
                    break;
            }

            checkFile.open(getString);

            if (!checkFile.good())
            {
#ifndef LINUX
                sprintf(messageText, "The resource is not found :\n%s !", getString);
                MessageBox(NULL, messageText, "Resource not found !", MB_TASKMODAL | MB_ICONERROR | MB_OK);
#else
                QMessageBox messageBox(QMessageBox::Critical, QStringLiteral("Resource not found !"), QString("The resource is not found :\n%1 !").arg(getString), QMessageBox::Ok);
                messageBox.exec();
#endif
                return false;
            }
            else
                checkFile.close();
        }
    }

    // Reset the current directory :
#ifndef LINUX
    SetCurrentDirectory(procPath);
#else
    chdir(procPath);
#endif
    return true;
}
