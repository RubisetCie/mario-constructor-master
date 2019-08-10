/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#include <SFML/Graphics.hpp>
#include <cmath>

extern "C"
{
    #include <FMODEx/fmod.h>
    #include <windows.h>
}

#include "../../Headers/Core/gscene.hpp"
#include "../../Headers/gglobals.hpp"

#define FADESPEED 4

using namespace sf;
using namespace std;

static bool InitAssets();
static void UpdateAssets();

Texture* logo;
Sprite* sprLogo;

RectangleShape* fadeRect;

Uint8 fadeAlpha;

bool Scene::Logo()
{
    typedef enum States {ENTRANCE,
                         END}
                         States;

    bool exitLoop = false;

    if (!InitAssets())
    {
        MessageBox(NULL, "Failed to initialize the assets on the Logo !", "Assets Error !", MB_OK | MB_ICONERROR | MB_TASKMODAL);

        mainWindow->close();

        exitLoop = true;
    }

    States state = ENTRANCE;

    unsigned int timeb = 0;

    Event eventSystem;

    /**The following code do : Define the main loop of the scene.*/

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
                        exitLoop = true;

                        mainWindow->close();

                        break;
                    }

                    if (state == ENTRANCE)
                    {
                        timeb = 0;
                        state = END;
                    }

                    break;

                case Event::MouseMoved          :

                    mpos_absolute = Vector2i((eventSystem.mouseMove.x - wpos.x) / windowScale, (eventSystem.mouseMove.y - wpos.y) / windowScale);

                    break;

                case Event::MouseButtonPressed  :

                    if (state == ENTRANCE)
                    {
                        timeb = 0;
                        state = END;
                    }

                    break;

                case Event::Resized :

                    windowScale = min(eventSystem.size.width / 640.0, eventSystem.size.height / 480.0);
                    wpos = Vector2f((eventSystem.size.width - (windowScale * 640)) / 2, (eventSystem.size.height - (windowScale * 480)) / 2);

                    mainWindow->setView(View(FloatRect(0, 0, eventSystem.size.width, eventSystem.size.height)));

                    textureRender.setPosition(wpos);
                    textureRender.setScale(windowScale, windowScale);

                    break;

                default                 : break;
            }
        }

        switch (state)
        {
            case ENTRANCE :

                if (fadeAlpha > FADESPEED)
                    fadeAlpha -= FADESPEED;
                else
                    fadeAlpha = 0;

                if (timeb > 200)
                    state = END;

                break;

            case END      :

                if (fadeAlpha < 255-FADESPEED)
                    fadeAlpha += FADESPEED;
                else
                {
                    targetScene = SCENE_TITLE;

                    exitLoop = true;
                }

                break;
        }

        UpdateAssets();

        timeb++;

        mainTexture.draw(*sprLogo);

        if (fadeAlpha > 0)
            mainTexture.draw(*fadeRect);

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

    delete logo;
    delete sprLogo;

    delete fadeRect;

    return true;
}

static bool InitAssets()
{
    bool allright = true;

    fadeAlpha = 255;

    logo = new Texture;
    if (!logo->loadFromFile("Data/Gfx/Logo.jpg"))
        allright = false;

    fadeRect = new RectangleShape(Vector2f(640, 480));
    fadeRect->setFillColor(Color::Black);

    sprLogo = new Sprite(*logo);

    {
        FMOD_RESULT result;

        result = FMOD_System_CreateStream(soundSystem,
                                "Data/Sfx/Musics/TitleScreen.mp3",
                                FMOD_LOOP_NORMAL |
                                FMOD_SOFTWARE |
                                FMOD_2D,
                                NULL,
                                &music);

        if (result != FMOD_OK)
            return false;
    }

    return allright;
}

static void UpdateAssets()
{
    fadeRect->setFillColor(Color(0, 0, 0, fadeAlpha));
}
