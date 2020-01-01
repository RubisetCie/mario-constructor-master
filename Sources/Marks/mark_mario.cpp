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

#ifdef DEBUGMODE
#include <iostream>
#endif

#include "../../Headers/globals.hpp"
#include "../../Headers/Miscs/effect_splash.hpp"
#include "../../Headers/Core/matrix.hpp"

#define MARIO_ACCEL 0.12
#define MARIO_DECEL 0.15

#define MARIO_MAXHSPEED 4
#define MARIO_MAXVSPEED 12
#define MARIO_MAXRUNSPEED 7.5
#define MARIO_MAXLUISPEED 8
#define MARIO_WARPSPEED 0.75
#define MARIO_DEATHSPEED 6

#define MARIO_ENDSPEED 2

#define MARIO_JUMPTIME 10
#define MARIO_JUMPSTRENGTH 12
#define MARIO_WARPTIME 80
#define MARIO_SWIMSTRENGTH 3.2
#define MARIO_BOUNCESTRENGTH 9

#define MARIO_ANIMTIME_END 8
#define MARIO_ANIMTIME_POWERUP 96
#define MARIO_ANIMTIME_FLICKER 64

using namespace sf;
using namespace std;

Mark_Mario::Mark_Mario(Texture* texture[], Texture* invincible_texture) : Pawn(),
    m_fireball  {(itemsTxt[9]), (itemsTxt[9])},
    m_beet      {(itemsTxt[10]), (itemsTxt[10])}
{
    m_mariotextures[0] = texture[0];
    m_mariotextures[1] = texture[1];
    m_mariotextures[2] = texture[2];
    m_mariotextures[3] = texture[3];
    m_mariotextures[4] = texture[4];
    m_mariotextures[5] = texture[5];

    m_warp = NULL;

    m_warpmario.setPrimitiveType(Quads);
    m_warpmario.resize(4);
    m_lastpowerup = game_powerup;
    m_animspeed = 8;
    m_animframes = 0;
    m_animtimer = 0;
    m_animpowerup = 0;
    m_animpowerups = 0;
    m_animtrace = 0;
    m_animfire = 0;

    m_jumping = 0;
    m_crouched = 0;
    m_cleartimer = -500;
    m_deathtimer = -1;
    m_deltajump = 0;
    m_warptimer = 0;
    m_invincibility = 0;

    m_onfloor = false;
    m_holdjump = false;
    m_bounce = false;
    m_active = true;

    m_swimming = false;

    m_movedistance = Vector2f(0, 0);

    m_checkpoint = NULL;

    m_sprite.setTexture(*m_mariotextures[game_powerup]);
    m_sprite.setOrigin(21, 32);

    m_invincible.setTexture(*invincible_texture);
    m_invincible.setTextureRect(IntRect(0, 0, 30, 30));
    m_invincible.setOrigin(0, 15);

    m_aabb.width = 28;

    if (game_powerup == 0)
        m_aabb.height = 30;
    else
    {
        m_aabb.height = 55;
        m_invincible.setScale(1, 1.933333);
    }
}

void Mark_Mario::setPosition(const Vector2f& pos)
{
    m_sprite.setPosition(pos);

    m_aabb.left = pos.x - 14;

    if (game_powerup == 0)
        m_aabb.top = pos.y + 2;
    else
        m_aabb.top = pos.y - 23;

    if (zoneb)
    {
        if (waterb != NULL)
        {
            if (!waterb->getType())
            {
                float height = waterb->getHeight();
                if (m_aabb.top + m_aabb.height > height)
                    m_swimming = true;
            }
        }
    }
    else
    {
        if (watera != NULL)
        {
            if (!watera->getType())
            {
                float height = watera->getHeight();
                if (m_aabb.top + m_aabb.height > height)
                    m_swimming = true;
            }
        }
    }
}

void Mark_Mario::update()
{
}

void Mark_Mario::secureUpdate()
{
    blockhitter.left = -64;
    enemystomper.left = -64;
    enemystomper_pt = NULL;

    if (blockkiller.left > -64)
    {
        if (blockkiller_create)
            blockkiller.left = -64;
        else
            blockkiller_create = true;
    }

    // Projectile handle :
    if (m_fireball[0].m_aabb.left > -64)
        m_fireball[0].update();

    if (m_fireball[1].m_aabb.left > -64)
        m_fireball[1].update();

    if (m_beet[0].m_aabb.left > -64)
        m_beet[0].update();

    if (m_beet[1].m_aabb.left > -64)
        m_beet[1].update();

    if (!m_active)
    {
        if (m_deathtimer != -1)
            deathUpdate();
        else if (m_warp != NULL)
            warpUpdate();
        else if (koopaEngaged)
            moveHorizontal(MARIO_ENDSPEED);

        return;
    }

    Matrix* currentMatrix;
    Vector2i* currentScale;
    list<Collider*>* currentList;

    bool focus = mainWindow->hasFocus();

    bool keyLeft = Keyboard::isKeyPressed(static_cast<Keyboard::Key>(keybindings[0])) && focus;
    bool keyRight = Keyboard::isKeyPressed(static_cast<Keyboard::Key>(keybindings[1])) && focus;
    bool keyDown = Keyboard::isKeyPressed(static_cast<Keyboard::Key>(keybindings[2])) && focus;
    bool keyJump = Keyboard::isKeyPressed(static_cast<Keyboard::Key>(keybindings[4])) && focus;
    bool keyRun = Keyboard::isKeyPressed(static_cast<Keyboard::Key>(keybindings[5])) && focus;

    // Liquid calculations :
    if (zoneb)
    {
        if (waterb != NULL)
        {
            float height = waterb->getHeight();

            if (waterb->getType())
            {
                if (m_aabb.top + m_aabb.height > height - 24)
                {
                    death();
                    return;
                }
            }
            else
            {
                if (m_aabb.top + m_aabb.height > height + 8)
                {
                    if (!m_swimming)
                    {
                        listEffect.emplace_back(new Effect_Splash(effectTxt[10], Vector2f(m_aabb.left - 2, height-38)));

                        if (!m_onfloor)
                            m_movedistance = Vector2f(0, 0);
                    }

                    m_swimming = true;
                }
                else if (m_aabb.top + m_aabb.height < height)
                {
                    if (m_swimming)
                        listEffect.emplace_back(new Effect_Splash(effectTxt[10], Vector2f(m_aabb.left - 2, height-38)));

                    m_swimming = false;
                }
            }
        }
    }
    else
    {
        if (watera != NULL)
        {
            float height = watera->getHeight();

            if (watera->getType())
            {
                if (m_aabb.top + m_aabb.height > height - 24)
                {
                    death();
                    return;
                }
            }
            else
            {
                if (m_aabb.top + m_aabb.height > height + 8)
                {
                    if (!m_swimming)
                    {
                        listEffect.emplace_back(new Effect_Splash(effectTxt[10], Vector2f(m_aabb.left - 2, height-38)));

                        if (!m_onfloor)
                            m_movedistance = Vector2f(0, 0);
                    }

                    m_swimming = true;
                }
                else if (m_aabb.top + m_aabb.height < height)
                {
                    if (m_swimming)
                        listEffect.emplace_back(new Effect_Splash(effectTxt[10], Vector2f(m_aabb.left - 2, height-38)));

                    m_swimming = false;
                }
            }
        }
    }

    // Keyboards Input :
    if (keyLeft && !keyRight && m_crouched == 0)
    {
        if (m_swimming)
        {
            if (m_movedistance.x > (MARIO_MAXHSPEED / -1.3) + (MARIO_ACCEL / 2))
            {
                if (m_movedistance.x > 0)
                    m_movedistance.x -= MARIO_ACCEL*1.5;

                if (game_powerup == 4)
                    m_movedistance.x -= MARIO_ACCEL*1.5;

                m_movedistance.x -= MARIO_ACCEL/1.5;
            }
        }
        else
        {
            if (m_movedistance.x > -MARIO_MAXHSPEED + MARIO_ACCEL)
            {
                if (m_movedistance.x > 0)
                    m_movedistance.x -= (MARIO_ACCEL*2);
                else if (m_jumping > 0)
                    m_movedistance.x -= MARIO_ACCEL;

                m_movedistance.x -= MARIO_ACCEL;
            }
            else
            {
                if (keyRun)
                {
                    if (game_powerup == 4)
                    {
                        if (m_movedistance.x > -MARIO_MAXLUISPEED + (MARIO_ACCEL/2))
                            m_movedistance.x -= (MARIO_ACCEL/2);
                        else
                            m_movedistance.x = -MARIO_MAXLUISPEED;
                    }
                    else
                    {
                        if (m_movedistance.x > -MARIO_MAXRUNSPEED + (MARIO_ACCEL/2))
                            m_movedistance.x -= (MARIO_ACCEL/2);
                        else
                            m_movedistance.x = -MARIO_MAXRUNSPEED;
                    }
                }
                else
                {
                    if (m_movedistance.x < -MARIO_MAXHSPEED - (MARIO_ACCEL/2))
                        m_movedistance.x += (MARIO_DECEL/2);
                    else
                        m_movedistance.x = -MARIO_MAXHSPEED;
                }
            }
        }

        if (m_movedistance.x <= 0)
            m_sprite.setScale(-1, 1);
    }
    else if (keyRight && !keyLeft && m_crouched == 0)
    {
        if (m_swimming)
        {
            if (m_movedistance.x < (MARIO_MAXHSPEED / 1.3) - (MARIO_ACCEL / 2))
            {
                if (m_movedistance.x < 0)
                    m_movedistance.x += MARIO_ACCEL*1.5;

                if (game_powerup == 4)
                    m_movedistance.x += MARIO_ACCEL*1.5;

                m_movedistance.x += MARIO_ACCEL/1.5;
            }
        }
        else
        {
            if (m_movedistance.x < MARIO_MAXHSPEED - MARIO_ACCEL)
            {
                if (m_movedistance.x < 0)
                    m_movedistance.x += (MARIO_ACCEL*2);
                else if (m_jumping > 0)
                    m_movedistance.x += MARIO_ACCEL;

                m_movedistance.x += MARIO_ACCEL;
            }
            else
            {
                if (keyRun)
                {
                    if (game_powerup == 4)
                    {
                        if (m_movedistance.x < MARIO_MAXLUISPEED - (MARIO_ACCEL/2))
                            m_movedistance.x += (MARIO_ACCEL/2);
                        else
                            m_movedistance.x = MARIO_MAXLUISPEED;
                    }
                    else
                    {
                        if (m_movedistance.x < MARIO_MAXRUNSPEED - (MARIO_ACCEL/2))
                            m_movedistance.x += (MARIO_ACCEL/2);
                        else
                            m_movedistance.x = MARIO_MAXRUNSPEED;
                    }
                }
                else
                {
                    if (m_movedistance.x > MARIO_MAXHSPEED + (MARIO_ACCEL/2))
                        m_movedistance.x -= (MARIO_DECEL/2);
                    else
                        m_movedistance.x = MARIO_MAXHSPEED;
                }
            }
        }

        if (m_movedistance.x >= 0)
            m_sprite.setScale(1, 1);
    }
    else
    {
        if (m_crouched == 1)
        {
            if (m_movedistance.x < -(MARIO_DECEL*2.0f))
                m_movedistance.x += (MARIO_DECEL*2.0f);
            else if (m_movedistance.x > (MARIO_DECEL*2.0f))
                m_movedistance.x -= (MARIO_DECEL*2.0f);
            else
                m_movedistance.x = 0;
        }
        else if (m_crouched == 0)
        {
            if (m_movedistance.x < -MARIO_DECEL)
                m_movedistance.x += MARIO_DECEL;
            else if (m_movedistance.x > MARIO_DECEL)
                m_movedistance.x -= MARIO_DECEL;
            else
                m_movedistance.x = 0;
        }
    }

    if (zoneb)
    {
        currentMatrix = collisionMatrixb;
        currentScale = &levelScaleb;
        currentList = &listColliderb;
    }
    else
    {
        currentMatrix = collisionMatrix;
        currentScale = &levelScale;
        currentList = &listCollider;
    }

    if (game_powerup > 0 && m_onfloor)
    {
        if (keyDown && m_crouched != 1)
        {
            m_crouched = 1;
            m_aabb.top += 25;
            m_aabb.height = 30;

            m_invincible.setScale(1, 1);
        }

        if (!keyDown && m_crouched == 1)
        {
            if (m_platform == NULL)
            {
                m_crouched = 2;
                m_aabb.top -= 25;
                m_aabb.height = 55;

                m_invincible.setScale(1, 1.933333);
            }
            else
            {
                if (!testVertical(-4, *currentScale, currentMatrix, currentList))
                {
                    m_crouched = 0;
                    m_aabb.top -= 25;
                    m_aabb.height = 55;

                    m_invincible.setScale(1, 1.933333);
                }
            }
        }
    }

    m_animspeed = roundf(16 / (abs(m_movedistance.x/2) + 2));

    if (m_animfire > 0)
        m_animfire--;

    if (m_invincibility > 0)
    {
        if (m_invincibility <= 110 && koopaEngaged == 0)
        {
            float volume;

            FMOD_Channel_GetVolume(musicChannel, &volume);

            if (volume > 0.022222)
                volume -= 0.022222;
            else
                volume = 0;

            FMOD_Channel_SetVolume(musicChannel, volume);
        }

        m_invincibility--;
        m_invincible.setTextureRect(IntRect((m_invincibility % 4) * 30, 0, 30, 30));

        if (m_invincibility == 0 && koopaEngaged == 0)
        {
            if (zoneb)
                FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(20), *musicZoneb, 0, &musicChannel);
            else
                FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(20), *musicZonea, 0, &musicChannel);
        }
    }

    if (keyJump && m_crouched == 0)
    {
        if (m_swimming)
        {
            if (!m_holdjump)
            {
                m_movedistance.y = -MARIO_SWIMSTRENGTH;

                m_holdjump = true;

                FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(0), sfxSamples[17], 0, NULL);
            }
        }
        else
        {
            if (m_jumping == 0 && m_onfloor && !m_holdjump)
            {
                m_holdjump = true;
                m_jumping = MARIO_JUMPTIME + roundf(abs(m_movedistance.x) / 3.0) + (game_powerup == 4 ? 2 : 0) + (rand() % 3) - 1;

                FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(0), sfxSamples[0], 0, NULL);
            }
            else if (m_jumping > 0)
                m_jumping--;
            else
                m_jumping = 0;
        }
    }
    else
    {
        m_holdjump = false;
        m_bounce = false;
        m_jumping = 0;
    }

    if (m_deltajump > 0)
        m_deltajump--;

    if (!m_onfloor)
    {
        if (m_swimming)
        {
            if (m_movedistance.y < MARIO_MAXVSPEED / 4)
                m_movedistance.y += mario_gravity / 8;
        }
        else
        {
            if (m_movedistance.y < MARIO_MAXVSPEED - mario_gravity)
            {
                if (m_movedistance.y < 0)
                    m_movedistance.y += mario_gravity / 2.5;

                m_movedistance.y += mario_gravity;
            }
        }

        if (m_crouched == 1)
        {
            m_crouched = 2;
            m_aabb.top -= 25;
            m_aabb.height = 55;

            m_invincible.setScale(1, 1.933333);
        }
    }

    if (m_swimming)
    {
        if (m_holdjump && m_movedistance.y < 0)
        {
            float height;

            if (zoneb)
                height = waterb->getHeight();
            else
                height = watera->getHeight();

            if (m_aabb.top + m_aabb.height < height + 8)
            {
                if (m_jumping == 0)
                    m_jumping = MARIO_JUMPTIME / 2;
                else if (m_jumping > 0)
                    m_jumping--;
                else
                    m_jumping = 0;

                m_swimming = false;

                listEffect.emplace_back(new Effect_Splash(effectTxt[10], Vector2f(m_aabb.left - 2, height-38)));
            }
        }
    }
    else
    {
        if (m_jumping > 0)
        {
            if (m_bounce)
                m_movedistance.y = -MARIO_BOUNCESTRENGTH;
            else
                m_movedistance.y = -MARIO_JUMPSTRENGTH;
        }
    }

    // Collision Events :
    if (m_movedistance.x != 0)
    {
        bool test;

        if (game_powerup == 0 || m_crouched == 1)
            test = makeMoveHorizontal(m_movedistance.x, *currentScale, currentMatrix, currentList);
        else
            test = makeMoveHorizontalHigh(m_movedistance.x, *currentScale, currentMatrix, currentList);

        if (test)
            m_movedistance.x = 0;
    }

    if (m_movedistance.y != 0)
    {
        if (makeMoveVertical(m_movedistance.y, *currentScale, currentMatrix, currentList))
        {
            if (m_movedistance.y > 0)
            {
                m_onfloor = true;
                m_movedistance.y = 0;
            }
            else
            {
                if (!m_swimming)
                {
                    m_movedistance.y = 0;

                    if ((rand() % 2) == 0)
                        m_movedistance.x = 0;
                }
                else
                    m_movedistance.y = 0.5;

                blockhitter.left = m_aabb.left;
                blockhitter.top = m_aabb.top - 24;
            }

            m_jumping = 0;
        }
    }

    if (m_platform != NULL)
    {
        if (game_powerup == 0 || m_crouched == 1)
            makeMoveHorizontalPlatform(m_platform->speed.x, *currentScale, currentMatrix, currentList);
        else
            makeMoveHorizontalHighPlatform(m_platform->speed.x, *currentScale, currentMatrix, currentList);

        makeMoveVerticalPlatform(m_platform->speed.y, *currentScale, currentMatrix, currentList);
    }

    if (m_crouched == 2)
    {
        bool testr = testHorizontalHigh(-1, true, *currentScale, currentMatrix, currentList);
        bool testl = testHorizontalHigh(1, false, *currentScale, currentMatrix, currentList);

        if (testr && testl)
        {
            float dir = m_sprite.getScale().x;

            if (dir > 0)
                m_aabb.left -= 2;
            else
                m_aabb.left += 2;

            m_movedistance.x = 0;
        }
        else if (testr && !testl)
        {
            m_aabb.left -= 2;
            m_movedistance.x = 0;
        }
        else if (testl && !testr)
        {
            m_aabb.left += 2;
            m_movedistance.x = 0;
        }
        else
            m_crouched = 0;

        if (m_aabb.left < 0 || m_aabb.left + 28 > (currentScale->x * 640))
            m_crouched = 0;
    }

    // Player push when autoscroll :
    if (autoscroll == zoneb + 1 || koopaEngaged > 0)
    {
        if (m_aabb.left < cameraPos.x - 320)
            pushH(cameraPos.x - 320 - m_aabb.left);
        else if (m_aabb.left > cameraPos.x + 292)
            pushH(cameraPos.x + 292 - m_aabb.left);

        if (!m_active)
            return;
    }

    if (m_onfloor)
    {
        if (!testVertical(1, *currentScale, currentMatrix, currentList))
        {
            m_onfloor = false;
            m_platform = NULL;
        }
    }

    // Warps handling :
    if (m_warp == 0 && /*m_invincibility == 0 &&*/ koopaEngaged == 0 && listPassages.size() > 0)
    {
        for (list<WarpsData*>::iterator it = listPassages.begin(); it != listPassages.end(); it++)
        {
            if ((*it)->entrancesection != zoneb || (*it)->exitx <= -1 || (*it)->exity <= -1)
                continue;

            Vector2f position((*it)->entrancex, (*it)->entrancey);

            switch ((*it)->entrancedir)
            {
                case 0 :
                    if (m_aabb.intersects(FloatRect(position.x + 16, position.y + 63, 32, 1)))
                    {
                        if (keyDown && m_onfloor)
                        {
                            m_warp = (*it);

                            m_warptimer = 0;

                            m_warpmario[0].position.y = position.y;
                            m_warpmario[1].position.y = position.y;
                            m_warpmario[2].position.y = position.y + 64;
                            m_warpmario[3].position.y = position.y + 64;

                            m_warpmario[0].texCoords.y = 0;
                            m_warpmario[1].texCoords.y = 0;
                            m_warpmario[2].texCoords.y = 64;
                            m_warpmario[3].texCoords.y = 64;

                            if (m_sprite.getScale().x > 0)
                            {
                                m_warpmario[0].position.x = position.x + 18;
                                m_warpmario[1].position.x = position.x + 49;
                                m_warpmario[2].position.x = position.x + 49;
                                m_warpmario[3].position.x = position.x + 18;

                                if (game_powerup == 0)
                                {
                                    m_warpmario[0].texCoords.x = 7;
                                    m_warpmario[1].texCoords.x = 38;
                                    m_warpmario[2].texCoords.x = 38;
                                    m_warpmario[3].texCoords.x = 7;
                                }
                                else
                                {
                                    m_warpmario[0].texCoords.x = 286;
                                    m_warpmario[1].texCoords.x = 317;
                                    m_warpmario[2].texCoords.x = 317;
                                    m_warpmario[3].texCoords.x = 286;
                                }
                            }
                            else
                            {
                                m_warpmario[0].position.x = position.x + 15;
                                m_warpmario[1].position.x = position.x + 46;
                                m_warpmario[2].position.x = position.x + 46;
                                m_warpmario[3].position.x = position.x + 15;

                                if (game_powerup == 0)
                                {
                                    m_warpmario[0].texCoords.x = 38;
                                    m_warpmario[1].texCoords.x = 7;
                                    m_warpmario[2].texCoords.x = 7;
                                    m_warpmario[3].texCoords.x = 38;
                                }
                                else
                                {
                                    m_warpmario[0].texCoords.x = 317;
                                    m_warpmario[1].texCoords.x = 286;
                                    m_warpmario[2].texCoords.x = 286;
                                    m_warpmario[3].texCoords.x = 317;
                                }
                            }

                            m_sprite.setPosition(position.x + 32, position.y + 32);

                            m_active = false;
                            m_swimming = false;
                            m_jumping = 0;
                            m_animtrace = 0;
                            m_animpowerup = 0;
                            m_lastpowerup = game_powerup;
                            m_animpowerups = false;
                            m_movedistance = Vector2f(0, 0);

                            if (game_powerup > 0)
                            {
                                m_aabb.top -= 25;
                                m_aabb.height = 55;

                                m_invincible.setScale(1, 1.933333);
                            }

                            m_sprite.setTexture(*m_mariotextures[game_powerup]);

                            m_crouched = 0;

                            m_platform = NULL;

                            FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(3), sfxSamples[6], 0, NULL);

                            return;
                        }
                    }
                    break;
                case 1 :
                    if (m_aabb.intersects(FloatRect(position.x + 63, position.y + 16, 1, 32)))
                    {
                        if (keyRight && m_crouched == 0)
                        {
                            if (!m_swimming)
                            {
                                if (m_aabb.top + m_aabb.height != position.y + 64)
                                    break;
                            }

                            m_warp = (*it);

                            m_warptimer = 0;

                            m_warpmario[0].position = Vector2f(position.x + 32, position.y);
                            m_warpmario[1].position = Vector2f(position.x + 64, position.y);
                            m_warpmario[2].position = Vector2f(position.x + 64, position.y + 64);
                            m_warpmario[3].position = Vector2f(position.x + 32, position.y + 64);

                            m_warpmario[0].texCoords = Vector2f(3, 0);
                            m_warpmario[1].texCoords = Vector2f(35, 0);
                            m_warpmario[2].texCoords = Vector2f(35, 64);
                            m_warpmario[3].texCoords = Vector2f(3, 64);

                            m_sprite.setPosition(position.x + 50, position.y + 32);
                            m_sprite.setScale(1, 1);

                            m_active = false;
                            m_swimming = false;
                            m_jumping = 0;
                            m_animtrace = 0;
                            m_animpowerup = 0;
                            m_lastpowerup = game_powerup;
                            m_animpowerups = false;
                            m_movedistance = Vector2f(0, 0);

                            m_sprite.setTexture(*m_mariotextures[game_powerup]);

                            m_platform = NULL;

                            FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(3), sfxSamples[6], 0, NULL);

                            return;
                        }
                    }
                    break;
                case 2 :
                    if (m_aabb.intersects(FloatRect(position.x + 16, position.y, 32, 1)))
                    {
                        if (Keyboard::isKeyPressed(static_cast<Keyboard::Key>(keybindings[3])) && focus && blockhitter.left <= -64)
                        {
                            m_warp = (*it);

                            m_warptimer = 0;

                            if (m_sprite.getScale().x > 0)
                            {
                                m_warpmario[0].position.x = position.x + 18;
                                m_warpmario[1].position.x = position.x + 51;
                                m_warpmario[2].position.x = position.x + 51;
                                m_warpmario[3].position.x = position.x + 18;

                                m_warpmario[0].texCoords.x = 127;
                                m_warpmario[1].texCoords.x = 160;
                                m_warpmario[2].texCoords.x = 160;
                                m_warpmario[3].texCoords.x = 127;
                            }
                            else
                            {
                                m_warpmario[0].position.x = position.x + 13;
                                m_warpmario[1].position.x = position.x + 46;
                                m_warpmario[2].position.x = position.x + 46;
                                m_warpmario[3].position.x = position.x + 13;

                                m_warpmario[0].texCoords.x = 160;
                                m_warpmario[1].texCoords.x = 127;
                                m_warpmario[2].texCoords.x = 127;
                                m_warpmario[3].texCoords.x = 160;
                            }

                            if (game_powerup == 0)
                            {
                                m_warpmario[0].position.y = position.y;
                                m_warpmario[1].position.y = position.y;
                                m_warpmario[2].position.y = position.y + 30;
                                m_warpmario[3].position.y = position.y + 30;

                                m_warpmario[0].texCoords.y = 34;
                                m_warpmario[1].texCoords.y = 34;
                                m_warpmario[2].texCoords.y = 64;
                                m_warpmario[3].texCoords.y = 64;

                                m_sprite.setPosition(position.x + 32, position.y - 2);
                            }
                            else
                            {
                                m_warpmario[0].position.y = position.y;
                                m_warpmario[1].position.y = position.y;
                                m_warpmario[2].position.y = position.y + 55;
                                m_warpmario[3].position.y = position.y + 55;

                                m_warpmario[0].texCoords.y = 9;
                                m_warpmario[1].texCoords.y = 9;
                                m_warpmario[2].texCoords.y = 64;
                                m_warpmario[3].texCoords.y = 64;

                                m_sprite.setPosition(position.x + 32, position.y + 23);
                            }

                            m_active = false;
                            m_swimming = false;
                            m_jumping = 0;
                            m_animtrace = 0;
                            m_animpowerup = 0;
                            m_lastpowerup = game_powerup;
                            m_animpowerups = false;
                            m_movedistance = Vector2f(0, 0);

                            m_sprite.setTexture(*m_mariotextures[game_powerup]);

                            m_platform = NULL;

                            FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(3), sfxSamples[6], 0, NULL);

                            return;
                        }
                    }
                    break;
                case 3 :
                    if (m_aabb.intersects(FloatRect(position.x, position.y + 16, 1, 32)))
                    {
                        if (keyLeft && m_crouched == 0)
                        {
                            if (!m_swimming)
                            {
                                if (m_aabb.top + m_aabb.height != position.y + 64)
                                    break;
                            }

                            m_warp = (*it);

                            m_warptimer = 0;

                            m_warpmario[0].position = Vector2f(position.x, position.y);
                            m_warpmario[1].position = Vector2f(position.x + 32, position.y);
                            m_warpmario[2].position = Vector2f(position.x + 32, position.y + 64);
                            m_warpmario[3].position = Vector2f(position.x, position.y + 64);

                            m_warpmario[0].texCoords = Vector2f(39, 0);
                            m_warpmario[1].texCoords = Vector2f(7, 0);
                            m_warpmario[2].texCoords = Vector2f(7, 64);
                            m_warpmario[3].texCoords = Vector2f(39, 64);

                            m_sprite.setPosition(position.x + 14, position.y + 32);
                            m_sprite.setScale(-1, 1);

                            m_active = false;
                            m_swimming = false;
                            m_jumping = 0;
                            m_animtrace = 0;
                            m_animpowerup = 0;
                            m_lastpowerup = game_powerup;
                            m_animpowerups = false;
                            m_movedistance = Vector2f(0, 0);

                            m_sprite.setTexture(*m_mariotextures[game_powerup]);

                            m_platform = NULL;

                            FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(3), sfxSamples[6], 0, NULL);

                            return;
                        }
                    }
                    break;
            }
        }
    }

    // Death beneath ground test :
    if (autoscroll == zoneb + 1 || koopaEngaged > 0)
    {
        if (m_aabb.top > cameraPos.y + 272)
        {
            death();
            return;
        }
    }
    else
    {
        if (m_aabb.top > (currentScale->y * 480) + 96)
        {
            death();
            return;
        }
    }

    if (game_powerup == 0 || m_crouched == 1)
    {
        m_sprite.setPosition(roundf(m_aabb.left + 14), roundf(m_aabb.top - 2));

        if (m_invincibility > 0)
            m_invincible.setPosition(roundf(m_aabb.left-1), roundf(m_aabb.top + 14));
    }
    else
    {
        m_sprite.setPosition(roundf(m_aabb.left + 14), roundf(m_aabb.top + 23));

        if (m_invincibility > 0)
            m_invincible.setPosition(roundf(m_aabb.left-1), roundf(m_aabb.top + 26));
    }

    // Animations :
    animate();
}

void Mark_Mario::afterUpdate()
{
}

void Mark_Mario::warpUpdate()
{
    if (!m_tracing.empty() || !m_tracingb.empty())
        updateTrace();

    if (m_warptimer >= MARIO_WARPTIME)
    {
        if (m_warptimer == MARIO_WARPTIME)
        {
            Vector2f position(m_warp->exitx, m_warp->exity);

            // Zone change :
            if (zoneb != m_warp->exitsection)
            {
                if (musicZonea != musicZoneb)
                {
                    #ifdef DEBUGMODE
                    cout << "New music" << endl;
                    #endif // DEBUGMODE
                    if (m_invincibility == 0)
                    {
                        if (m_warp->exitsection == 1)
                            FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(20), *musicZoneb, 0, &musicChannel);
                        else
                            FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(20), *musicZonea, 0, &musicChannel);
                    }
                }

                for (list<Effect*>::iterator it = listEffect.begin(); it != listEffect.end(); it++)
                    delete *it;

                listEffect.clear();

                effectCloudPos[0] = 0;
                effectCloudPos[1] = 0;
                effectWeatherPos = 0;
                effectLightning = (rand() % 350) + 100;

                zoneb = m_warp->exitsection;
            }

            // Inwater calculations :
            if (zoneb)
            {
                if (waterb != NULL)
                {
                    if (!waterb->getType())
                    {
                        float height = waterb->getHeight();
                        if (m_aabb.top + m_aabb.height > height)
                            m_swimming = true;
                    }
                }
            }
            else
            {
                if (watera != NULL)
                {
                    if (!watera->getType())
                    {
                        float height = watera->getHeight();
                        if (m_aabb.top + m_aabb.height > height)
                            m_swimming = true;
                    }
                }
            }

            switch (m_warp->exitdir)
            {
                case 0 :
                    if (m_sprite.getScale().x > 0)
                    {
                        m_warpmario[0].position.x = position.x + 18;
                        m_warpmario[1].position.x = position.x + 51;
                        m_warpmario[2].position.x = position.x + 51;
                        m_warpmario[3].position.x = position.x + 18;

                        m_warpmario[0].texCoords.x = 127;
                        m_warpmario[1].texCoords.x = 160;
                        m_warpmario[2].texCoords.x = 160;
                        m_warpmario[3].texCoords.x = 127;
                    }
                    else
                    {
                        m_warpmario[0].position.x = position.x + 13;
                        m_warpmario[1].position.x = position.x + 46;
                        m_warpmario[2].position.x = position.x + 46;
                        m_warpmario[3].position.x = position.x + 13;

                        m_warpmario[0].texCoords.x = 160;
                        m_warpmario[1].texCoords.x = 127;
                        m_warpmario[2].texCoords.x = 127;
                        m_warpmario[3].texCoords.x = 160;
                    }

                    if (game_powerup == 0)
                    {
                        m_warpmario[0].position.y = position.y - 34;
                        m_warpmario[1].position.y = position.y - 34;
                        m_warpmario[2].position.y = position.y - 34;
                        m_warpmario[3].position.y = position.y - 34;

                        m_warpmario[0].texCoords.y = 98;
                        m_warpmario[1].texCoords.y = 98;
                        m_warpmario[2].texCoords.y = 64;
                        m_warpmario[3].texCoords.y = 64;

                        m_sprite.setPosition(position.x + 32, position.y - 66);
                    }
                    else
                    {
                        m_warpmario[0].position.y = position.y - 9;
                        m_warpmario[1].position.y = position.y - 9;
                        m_warpmario[2].position.y = position.y - 9;
                        m_warpmario[3].position.y = position.y - 9;

                        m_warpmario[0].texCoords.y = 73;
                        m_warpmario[1].texCoords.y = 73;
                        m_warpmario[2].texCoords.y = 64;
                        m_warpmario[3].texCoords.y = 64;

                        m_sprite.setPosition(position.x + 32, position.y - 41);
                    }

                    m_aabb.left = position.x + 18;
                    m_aabb.top = position.y;

                    m_sprite.setTextureRect(IntRect(120, 0, 40, 64));

                    break;
                case 1 :
                    m_warpmario[0].position = Vector2f(position.x - 28, position.y);
                    m_warpmario[1].position = Vector2f(position.x - 28, position.y);
                    m_warpmario[2].position = Vector2f(position.x - 28, position.y + 64);
                    m_warpmario[3].position = Vector2f(position.x - 28, position.y + 64);

                    m_warpmario[0].texCoords = Vector2f(67, 0);
                    m_warpmario[1].texCoords = Vector2f(39, 0);
                    m_warpmario[2].texCoords = Vector2f(39, 64);
                    m_warpmario[3].texCoords = Vector2f(67, 64);

                    m_aabb.left = position.x;

                    if (game_powerup == 0)
                        m_aabb.top = position.y + 34;
                    else
                        m_aabb.top = position.y + 9;

                    m_sprite.setPosition(position.x - 50, position.y + 32);
                    m_sprite.setScale(1, 1);
                    m_sprite.setTextureRect(IntRect(0, 0, 40, 64));

                    break;
                case 2 :
                    m_warpmario[0].position.y = position.y + 64;
                    m_warpmario[1].position.y = position.y + 64;
                    m_warpmario[2].position.y = position.y + 64;
                    m_warpmario[3].position.y = position.y + 64;

                    m_warpmario[0].texCoords.y = 0;
                    m_warpmario[1].texCoords.y = 0;
                    m_warpmario[2].texCoords.y = 0;
                    m_warpmario[3].texCoords.y = 0;

                    if (m_sprite.getScale().x > 0)
                    {
                        m_warpmario[0].position.x = position.x + 18;
                        m_warpmario[1].position.x = position.x + 49;
                        m_warpmario[2].position.x = position.x + 49;
                        m_warpmario[3].position.x = position.x + 18;

                        if (game_powerup == 0)
                        {
                            m_warpmario[0].texCoords.x = 7;
                            m_warpmario[1].texCoords.x = 38;
                            m_warpmario[2].texCoords.x = 38;
                            m_warpmario[3].texCoords.x = 7;
                        }
                        else
                        {
                            m_warpmario[0].texCoords.x = 286;
                            m_warpmario[1].texCoords.x = 317;
                            m_warpmario[2].texCoords.x = 317;
                            m_warpmario[3].texCoords.x = 286;
                        }
                    }
                    else
                    {
                        m_warpmario[0].position.x = position.x + 15;
                        m_warpmario[1].position.x = position.x + 46;
                        m_warpmario[2].position.x = position.x + 46;
                        m_warpmario[3].position.x = position.x + 15;

                        if (game_powerup == 0)
                        {
                            m_warpmario[0].texCoords.x = 38;
                            m_warpmario[1].texCoords.x = 7;
                            m_warpmario[2].texCoords.x = 7;
                            m_warpmario[3].texCoords.x = 38;
                        }
                        else
                        {
                            m_warpmario[0].texCoords.x = 317;
                            m_warpmario[1].texCoords.x = 286;
                            m_warpmario[2].texCoords.x = 286;
                            m_warpmario[3].texCoords.x = 317;
                        }
                    }

                    m_aabb.left = position.x + 18;

                    m_sprite.setPosition(position.x + 32, position.y + 96);

                    if (game_powerup == 0)
                    {
                        m_aabb.top = position.y + 34;
                        m_sprite.setTextureRect(IntRect(0, 0, 40, 64));
                    }
                    else
                    {
                        m_aabb.top = position.y + 9;
                        m_sprite.setTextureRect(IntRect(280, 0, 40, 64));
                    }

                    break;
                case 3 :
                    m_warpmario[0].position = Vector2f(position.x + 92, position.y);
                    m_warpmario[1].position = Vector2f(position.x + 92, position.y);
                    m_warpmario[2].position = Vector2f(position.x + 92, position.y + 64);
                    m_warpmario[3].position = Vector2f(position.x + 92, position.y + 64);

                    m_warpmario[0].texCoords = Vector2f(39, 0);
                    m_warpmario[1].texCoords = Vector2f(67, 0);
                    m_warpmario[2].texCoords = Vector2f(67, 64);
                    m_warpmario[3].texCoords = Vector2f(39, 64);

                    m_aabb.left = position.x + 36;

                    if (game_powerup == 0)
                        m_aabb.top = position.y + 34;
                    else
                        m_aabb.top = position.y + 9;

                    m_sprite.setPosition(position.x + 114, position.y + 32);
                    m_sprite.setScale(-1, 1);
                    m_sprite.setTextureRect(IntRect(0, 0, 40, 64));

                    break;
            }

            // Incoming autoscroll :
            if (!autoscrollPath.empty() && autoscroll == zoneb + 1)
            {
                Vector2f firstnode(autoscrollPath[autoscroll_node]);

                cameraPos = m_sprite.getPosition();

                if (firstnode.x > cameraPos.x - 300 && firstnode.x < cameraPos.x + 300 && firstnode.y > cameraPos.y - 220 && firstnode.y < cameraPos.y + 220)
                {
                    cameraPos = firstnode;
                    autoscroll_node = 1;
                }
            }

            FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(3), sfxSamples[6], 0, NULL);
        }

        switch (m_warp->exitdir)
        {
            case 0 :
                if (m_warpmario[0].position.y < m_warp->exity - MARIO_WARPSPEED)
                {
                    m_warpmario[0].position.y += MARIO_WARPSPEED;
                    m_warpmario[1].position.y += MARIO_WARPSPEED;
                }
                else
                {
                    m_warpmario[0].position.y = m_warp->exity;
                    m_warpmario[1].position.y = m_warp->exity;
                }

                m_warpmario[2].position.y += MARIO_WARPSPEED;
                m_warpmario[3].position.y += MARIO_WARPSPEED;

                m_warpmario[0].texCoords.y -= MARIO_WARPSPEED;
                m_warpmario[1].texCoords.y -= MARIO_WARPSPEED;

                m_sprite.move(0, MARIO_WARPSPEED);

                break;
            case 1 :
                if (m_warpmario[0].position.x < m_warp->exitx - MARIO_WARPSPEED)
                {
                    m_warpmario[0].position.x += MARIO_WARPSPEED;
                    m_warpmario[3].position.x += MARIO_WARPSPEED;
                }
                else
                {
                    m_warpmario[0].position.x = m_warp->exitx;
                    m_warpmario[3].position.x = m_warp->exitx;
                }

                m_warpmario[1].position.x += MARIO_WARPSPEED;
                m_warpmario[2].position.x += MARIO_WARPSPEED;

                m_warpmario[0].texCoords.x -= MARIO_WARPSPEED;
                m_warpmario[3].texCoords.x -= MARIO_WARPSPEED;

                if (m_warptimer % 8 == 0)
                {
                    if (m_warpmario[1].texCoords.x < 80)
                    {
                        m_warpmario[0].texCoords.x += 40;
                        m_warpmario[1].texCoords.x += 40;
                        m_warpmario[2].texCoords.x += 40;
                        m_warpmario[3].texCoords.x += 40;
                    }
                    else
                    {
                        m_warpmario[0].texCoords.x -= 80;
                        m_warpmario[1].texCoords.x -= 80;
                        m_warpmario[2].texCoords.x -= 80;
                        m_warpmario[3].texCoords.x -= 80;
                    }
                }

                m_sprite.move(MARIO_WARPSPEED, 0);

                break;
            case 2 :
                m_warpmario[0].position.y -= MARIO_WARPSPEED;
                m_warpmario[1].position.y -= MARIO_WARPSPEED;

                m_warpmario[2].texCoords.y += MARIO_WARPSPEED;
                m_warpmario[3].texCoords.y += MARIO_WARPSPEED;

                m_sprite.move(0, -MARIO_WARPSPEED);

                break;
            case 3 :
                if (m_warpmario[1].position.x > m_warp->exitx + 64 + MARIO_WARPSPEED)
                {
                    m_warpmario[1].position.x -= MARIO_WARPSPEED;
                    m_warpmario[2].position.x -= MARIO_WARPSPEED;
                }
                else
                {
                    m_warpmario[1].position.x = m_warp->exitx + 64;
                    m_warpmario[2].position.x = m_warp->exitx + 64;
                }

                m_warpmario[0].position.x -= MARIO_WARPSPEED;
                m_warpmario[3].position.x -= MARIO_WARPSPEED;

                m_warpmario[1].texCoords.x -= MARIO_WARPSPEED;
                m_warpmario[2].texCoords.x -= MARIO_WARPSPEED;

                if (m_warptimer % 8 == 0)
                {
                    if (m_warpmario[0].texCoords.x < 80)
                    {
                        m_warpmario[0].texCoords.x += 40;
                        m_warpmario[1].texCoords.x += 40;
                        m_warpmario[2].texCoords.x += 40;
                        m_warpmario[3].texCoords.x += 40;
                    }
                    else
                    {
                        m_warpmario[0].texCoords.x -= 80;
                        m_warpmario[1].texCoords.x -= 80;
                        m_warpmario[2].texCoords.x -= 80;
                        m_warpmario[3].texCoords.x -= 80;
                    }
                }

                m_sprite.move(-MARIO_WARPSPEED, 0);

                break;
        }

        if (m_warptimer > MARIO_WARPTIME + MARIO_WARPTIME)
        {
            m_active = true;
            m_warp = NULL;

            if (m_invincibility > 0)
            {
                if (game_powerup == 0)
                    m_invincible.setPosition(roundf(m_aabb.left-1), roundf(m_aabb.top + 14));
                else
                    m_invincible.setPosition(roundf(m_aabb.left-1), roundf(m_aabb.top + 26));
            }
        }
    }
    else
    {
        switch (m_warp->entrancedir)
        {
            case 0 :
                m_warpmario[0].position.y += MARIO_WARPSPEED;
                m_warpmario[1].position.y += MARIO_WARPSPEED;

                m_warpmario[2].texCoords.y -= MARIO_WARPSPEED;
                m_warpmario[3].texCoords.y -= MARIO_WARPSPEED;

                m_sprite.move(0, MARIO_WARPSPEED);

                break;
            case 1 :
                if (m_warpmario[0].position.x < m_warp->entrancex + 64 - MARIO_WARPSPEED)
                {
                    m_warpmario[0].position.x += MARIO_WARPSPEED;
                    m_warpmario[3].position.x += MARIO_WARPSPEED;

                    m_warpmario[1].texCoords.x -= MARIO_WARPSPEED;
                    m_warpmario[2].texCoords.x -= MARIO_WARPSPEED;
                }

                if (m_warptimer % 8 == 0)
                {
                    if (m_warpmario[0].texCoords.x < 80)
                    {
                        m_warpmario[0].texCoords.x += 40;
                        m_warpmario[1].texCoords.x += 40;
                        m_warpmario[2].texCoords.x += 40;
                        m_warpmario[3].texCoords.x += 40;
                    }
                    else
                    {
                        m_warpmario[0].texCoords.x -= 80;
                        m_warpmario[1].texCoords.x -= 80;
                        m_warpmario[2].texCoords.x -= 80;
                        m_warpmario[3].texCoords.x -= 80;
                    }
                }

                m_sprite.move(MARIO_WARPSPEED, 0);

                break;
            case 2 :
                if (m_warpmario[2].position.y > m_warp->entrancey + MARIO_WARPSPEED)
                {
                    m_warpmario[2].position.y -= MARIO_WARPSPEED;
                    m_warpmario[3].position.y -= MARIO_WARPSPEED;

                    m_warpmario[0].texCoords.y += MARIO_WARPSPEED;
                    m_warpmario[1].texCoords.y += MARIO_WARPSPEED;
                }
                else
                {
                    m_warpmario[2].position.y = m_warp->entrancey;
                    m_warpmario[3].position.y = m_warp->entrancey;
                }

                m_sprite.move(0, -MARIO_WARPSPEED);

                break;
            case 3 :
                if (m_warpmario[1].position.x > m_warp->entrancex + MARIO_WARPSPEED)
                {
                    m_warpmario[1].position.x -= MARIO_WARPSPEED;
                    m_warpmario[2].position.x -= MARIO_WARPSPEED;

                    m_warpmario[0].texCoords.x -= MARIO_WARPSPEED;
                    m_warpmario[3].texCoords.x -= MARIO_WARPSPEED;
                }

                if (m_warptimer % 8 == 0)
                {
                    if (m_warpmario[1].texCoords.x < 80)
                    {
                        m_warpmario[0].texCoords.x += 40;
                        m_warpmario[1].texCoords.x += 40;
                        m_warpmario[2].texCoords.x += 40;
                        m_warpmario[3].texCoords.x += 40;
                    }
                    else
                    {
                        m_warpmario[0].texCoords.x -= 80;
                        m_warpmario[1].texCoords.x -= 80;
                        m_warpmario[2].texCoords.x -= 80;
                        m_warpmario[3].texCoords.x -= 80;
                    }
                }

                m_sprite.move(-MARIO_WARPSPEED, 0);

                break;
        }
    }

    m_warptimer++;
}

void Mark_Mario::deathUpdate()
{
    m_deathtimer++;

    if (m_deathtimer == 35)
        m_movedistance.y = -MARIO_DEATHSPEED;
    else if (m_deathtimer == 250)
    {
        // Game over :
        if (game_lives <= 0)
        {
            m_sprite.setTexture(*hudTxt[6], true);
            m_sprite.setPosition(cameraPos.x - 44, cameraPos.y + 24);

            m_movedistance.y = 0;

            FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(20), sfxSamples[32], 0, NULL);
        }
        else
        {
            subLife();

            restartLevel = true;
        }
    }
    else if (m_deathtimer == 665)
        exitToTitle = true;

    if (m_deathtimer >= 35 && m_deathtimer < 250)
        m_movedistance.y += mario_gravity / 3;

    m_sprite.move(m_movedistance);
}

void Mark_Mario::animate()
{
    if (!m_tracing.empty() || !m_tracingb.empty())
        updateTrace();

    if (m_animpowerup > 0 && m_active)
    {
        m_animpowerup--;

        if (m_animpowerup % 2 == 0)
        {
            if (m_animpowerups)
                m_sprite.setTexture(*m_mariotextures[game_powerup]);
            else
                m_sprite.setTexture(*m_mariotextures[m_lastpowerup]);

            m_animpowerups = !m_animpowerups;
        }

        if (m_animpowerup == 1)
        {
            m_lastpowerup = game_powerup;
            m_animpowerups = false;
        }

        m_sprite.setTextureRect(IntRect(80, 0, 40, 64));

        return;
    }

    if (m_onfloor)
    {
        if (m_crouched == 1)
            m_sprite.setTextureRect(IntRect(280, 0, 40, 64));
        else
        {
            if (m_movedistance.x == 0)
            {
                if (m_animfire > 0)
                    m_sprite.setTextureRect(IntRect(320, 0, 40, 64));
                else
                    m_sprite.setTextureRect(IntRect(0, 0, 40, 64));
            }
            else
            {
                m_animtimer++;

                if (m_active)
                {
                    if (m_animtimer >= m_animspeed)
                    {
                        m_animframes++;
                        m_animtimer = 0;

                        if (m_animframes >= 3)
                            m_animframes = 0;
                    }
                }
                else
                {
                    if (m_animtimer >= 8)
                    {
                        m_animframes++;
                        m_animtimer = 0;

                        if (m_animframes >= 3)
                            m_animframes = 0;
                    }
                }

                m_sprite.setTextureRect(IntRect(m_animframes * 40, 0, 40, 64));
            }
        }
    }
    else
    {
        if (m_swimming)
        {
            if (m_movedistance.y < 0)
            {
                m_animtimer++;

                if (m_animtimer >= 5)
                {
                    m_animframes++;
                    m_animtimer = 0;

                    if (m_animframes >= 3)
                        m_animframes = 0;
                }

                m_sprite.setTextureRect(IntRect((m_animframes * 40) + 160, 0, 40, 64));
            }
            else
                m_sprite.setTextureRect(IntRect(160, 0, 40, 64));
        }
        else
            m_sprite.setTextureRect(IntRect(120, 0, 40, 64));
    }

    if (game_powerup == 4 && !m_onfloor)
    {
        if (m_animtrace < 2)
            m_animtrace++;
        else
        {
            Vector2f pos(m_aabb.left-7, m_aabb.top-9);
            bool ri = true;

            m_animtrace = 0;

            if (m_sprite.getScale().x < 0)
            {
                pos.x += 2;
                ri = false;
            }

            addTrace(pos, m_sprite.getTextureRect(), ri);
        }
    }
    else if (m_animtrace > 0)
        m_animtrace--;
}

void Mark_Mario::setPower(unsigned char power, bool hurt)
{
    if (power == 0)
    {
        if (m_crouched != 1)
        {
            m_aabb.top += 25;
            m_aabb.height = 30;

            m_invincible.setScale(1, 1);
        }
        else
            m_crouched = 0;
    }
    else if (power == 1 && game_powerup == 0)
    {
        m_crouched = 2;

        m_aabb.top -= 25;
        m_aabb.height = 55;

        m_invincible.setScale(1, 1.933333);
    }

    m_sprite.setTexture(*m_mariotextures[power]);

    game_powerup = power;

    m_animpowerup = MARIO_ANIMTIME_POWERUP;
    m_animfire = 0;

    if (hurt)
        m_animtrace = MARIO_ANIMTIME_FLICKER;
    else
        m_animtrace = 0;
}

void Mark_Mario::moveHorizontal(float dist)
{
    bool free = true;

    m_cleartimer++;

    if (m_cleartimer >= 5)
    {
        if (timer > 0)
        {
            char timebuf[6]("");

            if (m_animpowerup < 4)
            {
                timer--;

                if (timer <= 0)
                {
                    timer = 0;
                    m_cleartimer = 50001;
                }

                m_animpowerup++;
            }
            else
            {
                timer -= m_cleartimer;

                addPoints(100);

                if (timer <= 0)
                {
                    timer = 0;
                    m_cleartimer = 50001;
                }

                FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(1), sfxSamples[30], 0, NULL);

                m_animpowerup = 0;
            }

            sprintf(timebuf, "%d", timer);
            timerFont->setText(timebuf, 6, Vector2f(roundf(cameraPos.x) + 257, roundf(cameraPos.y) - 204), true);
        }

        if (m_cleartimer > 50065)
        {
            if (loadingType == 0)
                exitToTitle = true;
            else
                nextLevel = true;
        }
    }

    Vector2f cameuPos;
    if (koopaEngaged == 0 && autoscroll != zoneb + 1)
    {
        cameuPos = m_sprite.getPosition();

        if (cameuPos.x < 320)
            cameuPos.x = 320;

        if (cameuPos.y < 240)
            cameuPos.y = 240;
    }

    if (zoneb)
    {
        if (koopaEngaged == 0 && autoscroll != zoneb + 1)
        {
            if (cameuPos.x > (levelScaleb.x * 640) - 320)
                cameuPos.x = (levelScaleb.x * 640) - 320;

            if (cameuPos.y > (levelScaleb.y * 480) - 240)
                cameuPos.y = (levelScaleb.y * 480) - 240;
        }

        if (game_powerup == 0)
        {
            if (makeMoveHorizontalNR(dist, levelScaleb, collisionMatrixb, &listColliderb))
                free = false;
        }
        else
        {
            if (makeMoveHorizontalHighNR(dist, levelScaleb, collisionMatrixb, &listColliderb))
                free = false;
        }

        if (m_movedistance.y != 0)
        {
            if (makeMoveVerticalNR(m_movedistance.y, levelScaleb, collisionMatrixb, &listColliderb))
            {
                m_onfloor = true;
                m_movedistance.y = 0;
            }
        }

        if (m_onfloor)
        {
            if (!testVertical(1, levelScaleb, collisionMatrixb, &listColliderb))
            {
                m_onfloor = false;
                m_platform = NULL;
            }
        }
    }
    else
    {
        if (koopaEngaged == 0 && autoscroll != zoneb + 1)
        {
            if (cameuPos.x > (levelScale.x * 640) - 320)
                cameuPos.x = (levelScale.x * 640) - 320;

            if (cameuPos.y > (levelScale.y * 480) - 240)
                cameuPos.y = (levelScale.y * 480) - 240;
        }

        if (game_powerup == 0)
        {
            if (makeMoveHorizontalNR(dist, levelScale, collisionMatrix, &listCollider))
                free = false;
        }
        else
        {
            if (makeMoveHorizontalHighNR(dist, levelScale, collisionMatrix, &listCollider))
                free = false;
        }

        if (m_movedistance.y != 0)
        {
            if (makeMoveVerticalNR(m_movedistance.y, levelScale, collisionMatrix, &listCollider))
            {
                m_onfloor = true;
                m_movedistance.y = 0;
            }
        }

        if (m_onfloor)
        {
            if (!testVertical(1, levelScale, collisionMatrix, &listCollider))
            {
                m_onfloor = false;
                m_platform = NULL;
            }
        }
    }

    if (koopaEngaged == 0 && autoscroll != zoneb + 1)
        cameraPos = Vector2f(lerp(cameraPos.x, cameuPos.x, 0.5), lerp(cameraPos.y, cameuPos.y, 0.5));

    if (m_invincibility > 0)
    {
        if (m_invincibility <= 110)
        {
            float volume;

            FMOD_Channel_GetVolume(musicChannel, &volume);

            if (volume > 0.011111)
                volume -= 0.011111;
            else
                volume = 0;

            FMOD_Channel_SetVolume(musicChannel, volume);
        }

        m_invincibility--;
        m_invincible.setTextureRect(IntRect((m_invincibility % 4) * 30, 0, 30, 30));
    }

    // Liquid calculations :
    if (zoneb)
    {
        if (waterb != NULL)
        {
            if (!waterb->getType())
            {
                float height = waterb->getHeight();

                if (m_aabb.top + m_aabb.height > height + 8)
                {
                    if (!m_swimming)
                        listEffect.emplace_back(new Effect_Splash(effectTxt[10], Vector2f(m_aabb.left - 2, height-38)));

                    m_swimming = true;
                }
                else if (m_aabb.top + m_aabb.height < height)
                {
                    if (m_swimming)
                        listEffect.emplace_back(new Effect_Splash(effectTxt[10], Vector2f(m_aabb.left - 2, height-38)));

                    m_swimming = false;
                }
            }
        }
    }
    else
    {
        if (watera != NULL)
        {
            if (!watera->getType())
            {
                float height = watera->getHeight();

                if (m_aabb.top + m_aabb.height > height + 8)
                {
                    if (!m_swimming)
                        listEffect.emplace_back(new Effect_Splash(effectTxt[10], Vector2f(m_aabb.left - 2, height-38)));

                    m_swimming = true;
                }
                else if (m_aabb.top + m_aabb.height < height)
                {
                    if (m_swimming)
                        listEffect.emplace_back(new Effect_Splash(effectTxt[10], Vector2f(m_aabb.left - 2, height-38)));

                    m_swimming = false;
                }
            }
        }
    }

    if (!m_onfloor)
    {
        if (m_swimming)
        {
            if (m_movedistance.y < MARIO_MAXVSPEED / 4)
                m_movedistance.y += mario_gravity / 8;
        }
        else
        {
            if (m_movedistance.y < MARIO_MAXVSPEED - mario_gravity)
                m_movedistance.y += mario_gravity;
        }
    }

    if (game_powerup == 0)
    {
        m_sprite.setPosition(roundf(m_aabb.left + 14), roundf(m_aabb.top - 2));
        m_invincible.setPosition(roundf(m_aabb.left-1), roundf(m_aabb.top + 14));
    }
    else
    {
        m_sprite.setPosition(roundf(m_aabb.left + 14), roundf(m_aabb.top + 23));
        m_invincible.setPosition(roundf(m_aabb.left-1), roundf(m_aabb.top + 26));
    }

    if (free)
        animate();
    else
    {
        if (m_onfloor)
            m_sprite.setTextureRect(IntRect(0, 0, 40, 64));
    }
}

void Mark_Mario::fire(int code)
{
    if (!m_active)
        return;

    if (m_crouched != 0)
        return;

    if (code == keybindings[5])
    {
        Vector2f pos(m_sprite.getPosition());

        if (game_powerup == 2)
        {
            Vector2f ppos(pos.x, pos.y - 2);
            bool dir;

            if (m_sprite.getScale().x > 0)
            {
                ppos.x += 10;
                dir = true;
            }
            else
            {
                ppos.x -= 10;
                dir = false;
            }

            if (m_fireball[0].m_aabb.left <= -64)
            {
                m_fireball[0].cast(ppos, dir);
                m_animfire = 6;

                FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(1), sfxSamples[8], 0, NULL);
            }
            else if (m_fireball[1].m_aabb.left <= -64)
            {
                m_fireball[1].cast(ppos, dir);
                m_animfire = 6;

                FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(1), sfxSamples[8], 0, NULL);
            }
        }
        else if (game_powerup == 3)
        {
            Vector2f ppos(pos.x, pos.y - 6);
            bool dir;

            if (m_sprite.getScale().x > 0)
            {
                ppos.x -= 1;
                dir = true;
            }
            else
            {
                ppos.x -= 24;
                dir = false;
            }

            if (m_beet[0].m_aabb.left <= -64)
            {
                m_beet[0].cast(ppos, dir);
                m_animfire = 6;

                FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(1), sfxSamples[8], 0, NULL);
            }
            else if (m_beet[1].m_aabb.left <= -64)
            {
                m_beet[1].cast(ppos, dir);
                m_animfire = 6;

                FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(1), sfxSamples[8], 0, NULL);
            }
        }
    }
    else if (code == keybindings[4])
        m_deltajump = 6;
}

void Mark_Mario::bounce(unsigned int maxstrenght, bool spring)
{
    if (m_swimming)
        m_movedistance.y = -MARIO_SWIMSTRENGTH;
    else
    {
        m_movedistance.y = -MARIO_JUMPSTRENGTH;

        if (spring)
        {
            if (m_deltajump > 0)
            {
                m_holdjump = true;
                m_jumping = maxstrenght;
            }
        }
        else
        {
            m_holdjump = true;
            m_bounce = true;

            if (game_powerup == 4)
                maxstrenght += 2;

            m_jumping = maxstrenght;
        }
    }
}

void Mark_Mario::disorient()
{
    if (m_sprite.getScale().x > 0)
        m_movedistance.x = 6;
    else
        m_movedistance.x = -6;
}

void Mark_Mario::pushH(float distance)
{
    Matrix* currentMatrix;
    Vector2i* currentScale;
    list<Collider*>* currentList;

    if (zoneb)
    {
        currentMatrix = collisionMatrixb;
        currentScale = &levelScaleb;
        currentList = &listColliderb;
    }
    else
    {
        currentMatrix = collisionMatrix;
        currentScale = &levelScale;
        currentList = &listCollider;
    }

    if (game_powerup == 0 || m_crouched == 1)
    {
        makeMoveHorizontalPlatform(distance, *currentScale, currentMatrix, currentList);

        if (testHorizontal(distance / abs(distance), *currentScale, currentMatrix, currentList))
            death();
    }
    else
    {
        makeMoveHorizontalHighPlatform(distance, *currentScale, currentMatrix, currentList);

        if (testHorizontalHigh(distance / abs(distance), distance>0?true:false, *currentScale, currentMatrix, currentList))
            death();
    }
}

void Mark_Mario::pushV(float distance)
{
    Matrix* currentMatrix;
    Vector2i* currentScale;
    list<Collider*>* currentList;

    if (zoneb)
    {
        currentMatrix = collisionMatrixb;
        currentScale = &levelScaleb;
        currentList = &listColliderb;
    }
    else
    {
        currentMatrix = collisionMatrix;
        currentScale = &levelScale;
        currentList = &listCollider;
    }

    makeMoveVerticalPlatform(distance, *currentScale, currentMatrix, currentList);

    if ((distance > 0 && m_movedistance.y < 0) || (distance < 0 && m_movedistance.y > 0))
        m_movedistance.y = 0;

    if (testVertical(distance / abs(distance), *currentScale, currentMatrix, currentList))
        death();
}

void Mark_Mario::clearCourse()
{
    m_active = false;
    m_animspeed = MARIO_ANIMTIME_END;
    m_animpowerup = 0;

    m_movedistance.x = 1;

    if (autoscrollSpeed < 4)
        autoscrollSpeed = 4;

    timer = ceil(timer / 25);

    m_sprite.setScale(1, 1);

    FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(20), musicSamples[25], 0, &musicChannel);
}

void Mark_Mario::death()
{
    m_sprite.setScale(1, 1);
    m_sprite.setTexture(*m_mariotextures[5], true);
    m_sprite.move(4, 32);

    m_movedistance = Vector2f(0, 0);
    m_animtrace = 0;
    m_deathtimer = 0;
    m_active = false;

    m_invincibility = 0;

    m_tracing.clear();
    m_tracingb.clear();

    FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(20), sfxSamples[31], 0, NULL);
}

Vector2f Mark_Mario::getPosition() const
{
    return m_sprite.getPosition() - Vector2f(21, 32);
}

Vector2f Mark_Mario::getSize() const
{
    return Vector2f(40, 64);
}

ID Mark_Mario::getID() const
{
    return ID_PLAYER;
}

void Mark_Mario::draw(RenderTarget& target, RenderStates) const
{
    if (!m_tracing.empty())
        target.draw(&m_tracing.front(), m_tracing.size(), Quads, m_mariotextures[4]);

    if (!m_tracingb.empty())
        target.draw(&m_tracingb.front(), m_tracingb.size(), Quads, m_mariotextures[4]);

    if (game_powerup != 4 && m_animtrace > 0 && m_warp == 0)
    {
        if (m_animtrace % 2 == 0)
            target.draw(m_sprite);
    }
    else
    {
        if (m_warp == NULL)
            target.draw(m_sprite);
        else
            target.draw(m_warpmario, m_mariotextures[game_powerup]);
    }

    if (m_invincibility > 0 && m_warp == NULL)
        target.draw(m_invincible, BlendAdd);

    if (m_fireball[0].m_aabb.left > -64)
        target.draw(m_fireball[0]);

    if (m_fireball[1].m_aabb.left > -64)
        target.draw(m_fireball[1]);

    if (m_beet[0].m_aabb.left > -64)
        target.draw(m_beet[0]);

    if (m_beet[1].m_aabb.left > -64)
        target.draw(m_beet[1]);
}
