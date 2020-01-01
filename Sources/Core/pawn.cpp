/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#include <SFML/Graphics.hpp>
#include <cmath>
#include "../../Headers/Core/pawn.hpp"
#include "../../Headers/Core/matrix.hpp"
#include "../../Headers/Sprites/gotspeed.hpp"
#include "../../Headers/Sprites/gotcollision.hpp"
#include "../../Headers/Miscs/effect_fire.hpp"
#include "../../Headers/globals.hpp"

#ifdef DEBUGMODE
#include <iostream>
#endif

using namespace std;
using namespace sf;

Pawn::Pawn() : Placeable()
{
    m_platform = NULL;
}

bool Pawn::makeMoveVertical(float mov, const Vector2i& levelSize, Matrix* collision, list<Collider*>* more)
{
    Vector2f testPoint[3];
    bool collide = false;

    ID id = getID();

    if (mov > 0)
    {
        testPoint[0] = Vector2f(m_aabb.left + 1, m_aabb.top + m_aabb.height + mov);
        testPoint[1] = Vector2f(m_aabb.left + m_aabb.width - 1, m_aabb.top + m_aabb.height + mov);

        if (testPoint[0].y < 1)
            goto LBL_PA1;

        if (testPoint[0].y > (levelSize.y * 480) - 1)
            goto LBL_PA1;
    }
    else
    {
        testPoint[0] = Vector2f(m_aabb.left + 1, m_aabb.top + mov);
        testPoint[1] = Vector2f(m_aabb.left + m_aabb.width - 1, m_aabb.top + mov);
        testPoint[2] = Vector2f(m_aabb.left + 16, m_aabb.top + mov);

        if (id == ID_STOMPABLE && player != NULL)
        {
            if (player->m_active)
            {
                if (testPoint[0].y - mov >= player->m_aabb.top + player->m_aabb.height)
                {
                    for (register unsigned char i = 0; i < 3; i++)
                    {
                        if (player->m_aabb.contains(testPoint[i]))
                        {
                            enemystomper.left = m_aabb.left - 1;
                            enemystomper.top = m_aabb.top - 6;
                            enemystomper_pt = dynamic_cast<GotCollision*>(this)->m_collider;

                            player->bounce(14);

                            break;
                        }
                    }
                }
            }
        }

        if (testPoint[0].y < 1)
            goto LBL_PA1;
    }

    for (register unsigned char i = 0; i < 2; i++)
    {
        if (testPoint[i].x < 1)
            testPoint[i].x = 1;
        else if (testPoint[i].x > (levelSize.x * 640) - 1)
            testPoint[i].x = (levelSize.x * 640) - 1;
    }

    for (register unsigned char i = 0; i < 2; i++)
    {
        if (collision->getValue(testPoint[i].x / 32, testPoint[i].y / 32) == 1)
        {
            testPoint[0].y = roundf(testPoint[0].y / 32) * 32;

            collide = true;
            goto LBL_PA1;
        }
    }

    for (list<Collider*>::iterator it = more->begin(); it != more->end(); it++)
    {
        if ((*it)->object == this)
            continue;

        ColliderType type = (*it)->type;
        char dir = (*it)->dir;

        if (id != ID_PLAYER)
        {
            if (type == C_STOMPABLE || type == C_CENTIPEDE)
                continue;
        }

        if (type == C_UNSTOMPABLE)
            continue;

        if (mov > 0)
        {
            if (type == C_STOMPABLE)
            {
                if (player->m_invincibility > 0)
                    continue;

                for (register unsigned char i = 0; i < 2; i++)
                {
                    if ((*it)->rect.contains(testPoint[i]) && testPoint[i].y-mov <= (*it)->rect.top)
                    {
                        testPoint[0].y = (*it)->rect.top;

                        enemystomper.left = m_aabb.left - 2;
                        enemystomper.top = testPoint[0].y;
                        enemystomper_pt = (*it);

                        player->bounce(14);

                        collide = false;
                        goto LBL_PA1;
                    }
                }

                continue;
            }

            if (dir != 2)
            {
                if (dir == 1)
                {
                    if ((*it)->speed.y >= 0)
                    {
                        if (testPoint[0].y - mov >= (*it)->rect.top)
                            continue;
                    }
                    else
                    {
                        if (testPoint[0].y - mov >= (*it)->rect.top - (*it)->speed.y + 1)
                            continue;
                    }
                }

                for (register unsigned char i = 0; i < 2; i++)
                {
                    if ((*it)->rect.contains(testPoint[i]))
                    {
                        if (type == C_CENTIPEDE)
                        {
                            if (testPoint[0].y - (*it)->rect.top < 12)
                                testPoint[0].y = (*it)->rect.top;
                        }
                        else
                            testPoint[0].y = (*it)->rect.top;

                        #ifdef DEBUGMODE
                        if (id == ID_PLAYER)
                            cout << "Moist" << endl;
                        #endif // DEBUGMODE

                        collide = true;
                        goto LBL_PA1;
                    }
                }
            }
        }
        else
        {
            if (type == C_STOMPABLE)
                continue;

            if (dir != 1)
            {
                for (register unsigned char i = 0; i < 2; i++)
                {
                    if (dir == 2)
                    {
                        if (testPoint[i].y + mov < (*it)->rect.top)
                            continue;
                    }

                    if ((*it)->rect.contains(testPoint[i]))
                    {
                        if (type == C_CENTIPEDE)
                        {
                            if ((*it)->rect.top + (*it)->rect.height - testPoint[0].y < 12)
                                testPoint[0].y = (*it)->rect.top + (*it)->rect.height;
                        }
                        else
                            testPoint[0].y = (*it)->rect.top + (*it)->rect.height;

                        collide = true;
                        goto LBL_PA1;
                    }
                }
            }
        }
    }

    LBL_PA1 :

    if (mov > 0)
        m_aabb.top = testPoint[0].y - m_aabb.height;
    else
        m_aabb.top = testPoint[0].y;

    return collide;
}

bool Pawn::makeMoveHorizontal(float mov, const Vector2i& levelSize, Matrix* collision, list<Collider*>* more)
{
    Vector2f testPoint[4];
    ID id = getID();

    bool collide = false, willfall = false;

    if (mov > 0)
    {
        testPoint[0] = Vector2f(m_aabb.left + m_aabb.width + mov, m_aabb.top + 1);
        testPoint[1] = Vector2f(m_aabb.left + m_aabb.width + mov, m_aabb.top + m_aabb.height - 1);

        if (testPoint[0].x > (levelSize.x * 640) - 1)
        {
            testPoint[0].x = (levelSize.x * 640) - 1;

            collide = true;
            goto LBL_PA2;
        }
    }
    else
    {
        testPoint[0] = Vector2f(m_aabb.left + mov, m_aabb.top + 1);
        testPoint[1] = Vector2f(m_aabb.left + mov, m_aabb.top + m_aabb.height - 1);

        if (testPoint[0].x < 1)
        {
            testPoint[0].x = 1;

            collide = true;
            goto LBL_PA2;
        }
    }

    if (id == ID_WONTFALL)
    {
        testPoint[2] = Vector2f(m_aabb.left + 24 + mov, testPoint[1].y + 2);
        testPoint[3] = Vector2f(m_aabb.left + 8 + mov, testPoint[1].y + 2);
    }
    else
    {
        testPoint[2] = Vector2f(1, 1);
        testPoint[3] = Vector2f(1, 1);
    }

    for (register unsigned char i = 0; i < 4; i++)
    {
        if (testPoint[i].y > (levelSize.y * 480) - 1)
            testPoint[i].y = (levelSize.y * 480) - 1;
        else if (testPoint[i].y < 1)
            testPoint[i].y = 1;
    }

    if (id == ID_WONTFALL)
    {
        if (collision->getValue(testPoint[2].x / 32, testPoint[2].y / 32) != collision->getValue(testPoint[3].x / 32, testPoint[3].y / 32))
        {
            if (mov > 0)
            {
                if (!collision->getValue(testPoint[2].x / 32, testPoint[2].y / 32))
                    willfall = true;
            }
            else
            {
                if (!collision->getValue(testPoint[3].x / 32, testPoint[3].y / 32))
                    willfall = true;
            }
        }
    }

    for (register unsigned char i = 0; i < 2; i++)
    {
        if (collision->getValue(testPoint[i].x / 32, testPoint[i].y / 32) == 1)
        {
            testPoint[0].x = roundf(testPoint[0].x / 32) * 32;

            if (id == ID_SHELL)
            {
                if (m_aabb.left > cameraPos.x - 384 && m_aabb.left < cameraPos.x + 352 && m_aabb.top > cameraPos.y - 304 && m_aabb.top < cameraPos.y + 272)
                {
                    if (mov > 0)
                    {
                        spritehitter.left = testPoint[0].x + 4;
                        spritehitter.top = testPoint[0].y + 8;
                        spritehitter.width = 8;
                        spritehitter.height = 16;
                        spritehitter_create = false;
                    }
                    else
                    {
                        spritehitter.left = testPoint[0].x - 12;
                        spritehitter.top = testPoint[0].y + 8;
                        spritehitter.width = 8;
                        spritehitter.height = 16;
                        spritehitter_create = false;
                    }
                }
            }

            collide = true;
            goto LBL_PA2;
        }
    }

    for (list<Collider*>::iterator it = more->begin(); it != more->end(); it++)
    {
        ColliderType type = (*it)->type;
        Placeable* object = (*it)->object;
        char dir = (*it)->dir;

        if (id != ID_PLAYER)
        {
            if (type == C_CENTIPEDE)
                continue;
        }

        if (object == this)
            continue;

        if (id == ID_ENEMY || id == ID_STOMPABLE || id == ID_WONTFALL || id == ID_SHELL)
        {
            if (object == NULL)
                goto LBL_NOTENEMY;

            GotSpeed* walk;
            ID objectid;
            float speed = (*it)->speed.x;

            if (mov > 0)
            {
                if (id == ID_ENEMY || id == ID_STOMPABLE || id == ID_WONTFALL)
                {
                    if (object <= reinterpret_cast<Placeable*>(1))
                        goto LBL_NOTENEMY;

                    walk = dynamic_cast<GotSpeed*>(object);
                    objectid = object->getID();

                    if (objectid != ID_ENEMY && objectid != ID_STOMPABLE && objectid != ID_WONTFALL)
                        continue;

                    if ((*it)->rect.contains(testPoint[0]))
                    {
                        if (speed != 0)
                        {
                            if (speed < 0)
                                walk->m_movedistance.x = -walk->m_movedistance.x;

                            return true;
                        }
                    }
                }
                else
                {
                    if (object == reinterpret_cast<Placeable*>(1))
                        objectid = ID_USELESS;
                    else
                        objectid = object->getID();

                    for (register unsigned char i = 0; i < 2; i++)
                    {
                        if ((*it)->rect.contains(testPoint[i]))
                        {
                            if (objectid == ID_SHELL && speed < 0)
                                enemykiller.left = testPoint[0].x - 8;
                            else
                                enemykiller.left = testPoint[0].x;

                            enemykiller.top = testPoint[0].y + 8;

                            enemykiller.width = 16;
                            enemykiller.height = 22;
                            enemykiller_create = false;
                            enemykiller_fire = 2;

                            #ifdef DEBUGMODE
                            cout << "Hit" << endl;
                            #endif // DEBUGMODE

                            listEffect.emplace_back(new Effect_Fire(effectTxt[9], Vector2f(testPoint[0].x - 15, testPoint[0].y + 1)));

                            break;
                        }
                    }

                    continue;
                }
            }
            else
            {
                if (id == ID_ENEMY || id == ID_STOMPABLE || id == ID_WONTFALL)
                {
                    if (object <= reinterpret_cast<Placeable*>(1))
                        goto LBL_NOTENEMY;

                    walk = dynamic_cast<GotSpeed*>(object);
                    objectid = object->getID();

                    if (objectid != ID_ENEMY && objectid != ID_STOMPABLE && objectid != ID_WONTFALL)
                        continue;

                    if ((*it)->rect.contains(testPoint[0]))
                    {
                        if (speed != 0)
                        {
                            if (speed > 0)
                                walk->m_movedistance.x = -walk->m_movedistance.x;

                            return true;
                        }
                    }
                }
                else
                {
                    if (object == reinterpret_cast<Placeable*>(1))
                        objectid = ID_USELESS;
                    else
                        objectid = object->getID();

                    for (register unsigned char i = 0; i < 2; i++)
                    {
                        if ((*it)->rect.contains(testPoint[i]))
                        {
                            if (objectid == ID_SHELL && speed > 0)
                                enemykiller.left = testPoint[0].x - 8;
                            else
                                enemykiller.left = testPoint[0].x - 16;

                            enemykiller.top = testPoint[0].y + 8;

                            enemykiller.width = 16;
                            enemykiller.height = 22;
                            enemykiller_create = false;
                            enemykiller_fire = 2;

                            #ifdef DEBUGMODE
                            cout << "Hit" << endl;
                            #endif // DEBUGMODE

                            listEffect.emplace_back(new Effect_Fire(effectTxt[9], Vector2f(testPoint[0].x - 15, testPoint[0].y + 1)));

                            break;
                        }
                    }

                    continue;
                }
            }
        }

        LBL_NOTENEMY :

        if (type == C_STOMPABLE || type == C_UNSTOMPABLE)
            continue;

        if (dir != 2 && id == ID_WONTFALL)
        {
            if ((*it)->rect.contains(testPoint[2]) != (*it)->rect.contains(testPoint[3]))
                willfall = true;
        }

        if (dir == 0)
        {
            if (mov > 0)
            {
                for (register unsigned char i = 0; i < 2; i++)
                {
                    if ((*it)->rect.contains(testPoint[i]))
                    {
                        if (type == C_CENTIPEDE)
                        {
                            if (testPoint[0].x - (*it)->rect.left < 8)
                                testPoint[0].x = (*it)->rect.left;
                        }
                        else
                            testPoint[0].x = (*it)->rect.left;

                        collide = true;
                        goto LBL_PA2;
                    }
                }
            }
            else
            {
                for (register unsigned char i = 0; i < 2; i++)
                {
                    if ((*it)->rect.contains(testPoint[i]))
                    {
                        if (type == C_CENTIPEDE)
                        {
                            if ((*it)->rect.left + (*it)->rect.width - testPoint[0].x < 8)
                                testPoint[0].x = (*it)->rect.left + (*it)->rect.width;
                        }
                        else
                            testPoint[0].x = (*it)->rect.left + (*it)->rect.width;

                        collide = true;
                        goto LBL_PA2;
                    }
                }
            }
        }
    }

    if (id == ID_WONTFALL)
    {
        if (willfall)
            collide = true;
    }

    LBL_PA2 :

    if (mov > 0)
        m_aabb.left = testPoint[0].x - m_aabb.width;
    else
        m_aabb.left = testPoint[0].x;

    return collide;
}

void Pawn::makeMoveVerticalFake(float mov)
{
    Vector2f testPoint[3];

    if (mov > 0)
    {
        testPoint[0] = Vector2f(m_aabb.left, m_aabb.top + m_aabb.height + mov);

        m_aabb.top = testPoint[0].y - m_aabb.height;
    }
    else
    {
        testPoint[0] = Vector2f(m_aabb.left + 1, m_aabb.top + mov);
        testPoint[1] = Vector2f(m_aabb.left + 16, m_aabb.top + mov);
        testPoint[2] = Vector2f(m_aabb.left + m_aabb.width - 1, m_aabb.top + mov);

        if (player != NULL)
        {
            if (player->m_active)
            {
                if (testPoint[0].y - mov >= player->m_aabb.top + player->m_aabb.height)
                {
                    for (register unsigned char i = 0; i < 3; i++)
                    {
                        if (player->m_aabb.contains(testPoint[i]))
                        {
                            enemystomper.left = m_aabb.left - 1;
                            enemystomper.top = m_aabb.top - 6;
                            enemystomper_pt = dynamic_cast<GotCollision*>(this)->m_collider;

                            player->bounce(14);

                            break;
                        }
                    }
                }
            }
        }

        m_aabb.top = testPoint[0].y;
    }
}

bool Pawn::makeMoveVerticalHigh(float mov, const Vector2i& levelSize, Matrix* collision, list<Collider*>* more)
{
    ID id = getID();

    Vector2f testPoint[4];
    bool collide = false;

    if (mov > 0)
    {
        testPoint[0] = Vector2f(m_aabb.left + 1, m_aabb.top + m_aabb.height + mov);
        testPoint[1] = Vector2f(m_aabb.left + 24, m_aabb.top + m_aabb.height + mov);
        testPoint[2] = Vector2f(m_aabb.left + 48, m_aabb.top + m_aabb.height + mov);
        testPoint[3] = Vector2f(m_aabb.left + m_aabb.width - 1, m_aabb.top + m_aabb.height + mov);

        if (testPoint[0].y < 1)
            goto LBL_PA3;

        if (testPoint[0].y > (levelSize.y * 480) - 1)
            goto LBL_PA3;
    }
    else
    {
        testPoint[0] = Vector2f(m_aabb.left + 1, m_aabb.top + mov);
        testPoint[1] = Vector2f(m_aabb.left + 24, m_aabb.top + mov);
        testPoint[2] = Vector2f(m_aabb.left + 48, m_aabb.top + mov);
        testPoint[3] = Vector2f(m_aabb.left + m_aabb.width - 1, m_aabb.top + mov);

        if (id == ID_STOMPABLE && player != NULL)
        {
            if (player->m_active)
            {
                if (testPoint[0].y - mov >= player->m_aabb.top + player->m_aabb.height)
                {
                    for (register unsigned char i = 0; i < 4; i++)
                    {
                        if (player->m_aabb.contains(testPoint[i]))
                        {
                            enemystomper.left = m_aabb.left + 9;
                            enemystomper.top = m_aabb.top - 6;
                            enemystomper_pt = dynamic_cast<GotCollision*>(this)->m_collider;

                            player->bounce(14);

                            break;
                        }
                    }
                }
            }
        }

        if (testPoint[0].y < 1)
            goto LBL_PA3;
    }

    for (register unsigned char i = 0; i < 4; i++)
    {
        if (testPoint[i].x < 1)
            testPoint[i].x = 1;
        else if (testPoint[i].x > (levelSize.x * 640) - 1)
            testPoint[i].x = (levelSize.x * 640) - 1;
    }

    for (register unsigned char i = 0; i < 4; i++)
    {
        if (collision->getValue(testPoint[i].x / 32, testPoint[i].y / 32) == 1)
        {
            testPoint[0].y = roundf(testPoint[0].y / 32) * 32;

            collide = true;
            goto LBL_PA3;
        }
    }

    for (list<Collider*>::iterator it = more->begin(); it != more->end(); it++)
    {
        ColliderType type = (*it)->type;
        char dir = (*it)->dir;

        if (type == C_STOMPABLE || type == C_UNSTOMPABLE || type == C_CENTIPEDE)
            continue;

        if (mov > 0)
        {
            if (dir != 2)
            {
                if (dir == 1)
                {
                    if ((*it)->speed.y >= 0)
                    {
                        if (testPoint[0].y - mov >= (*it)->rect.top)
                            continue;
                    }
                    else
                    {
                        if (testPoint[0].y - mov >= (*it)->rect.top - (*it)->speed.y + 1)
                            continue;
                    }
                }

                for (register unsigned char i = 0; i < 4; i++)
                {
                    if ((*it)->rect.contains(testPoint[i]))
                    {
                        testPoint[0].y = (*it)->rect.top;

                        collide = true;
                        goto LBL_PA3;
                    }
                }
            }
        }
        else
        {
            if (dir == 0)
            {
                for (register unsigned char i = 0; i < 4; i++)
                {
                    if ((*it)->rect.contains(testPoint[i]))
                    {
                        testPoint[0].y = (*it)->rect.top + (*it)->rect.height;

                        collide = true;
                        goto LBL_PA3;
                    }
                }
            }
        }
    }

    LBL_PA3 :

    if (mov > 0)
        m_aabb.top = testPoint[0].y - m_aabb.height;
    else
        m_aabb.top = testPoint[0].y;

    return collide;
}

bool Pawn::makeMoveHorizontalHigh(float mov, const Vector2i& levelSize, Matrix* collision, list<Collider*>* more)
{
    Vector2f testPoint[3];

    bool collide = false;

    if (mov > 0)
    {
        testPoint[0] = Vector2f(m_aabb.left + m_aabb.width + mov, m_aabb.top + 1);
        testPoint[1] = Vector2f(m_aabb.left + m_aabb.width + mov, m_aabb.top + 24);
        testPoint[2] = Vector2f(m_aabb.left + m_aabb.width + mov, m_aabb.top + m_aabb.height - 1);

        if (testPoint[0].x > (levelSize.x * 640) - 1)
        {
            testPoint[0].x = (levelSize.x * 640) - 1;

            collide = true;
            goto LBL_PA4;
        }
    }
    else
    {
        testPoint[0] = Vector2f(m_aabb.left + mov, m_aabb.top + 1);
        testPoint[1] = Vector2f(m_aabb.left + mov, m_aabb.top + 24);
        testPoint[2] = Vector2f(m_aabb.left + mov, m_aabb.top + m_aabb.height - 1);

        if (testPoint[0].x < 1)
        {
            testPoint[0].x = 1;

            collide = true;
            goto LBL_PA4;
        }
    }

    for (register unsigned char i = 0; i < 3; i++)
    {
        if (testPoint[i].y > (levelSize.y * 480) - 1)
            testPoint[i].y = (levelSize.y * 480) - 1;
        else if (testPoint[i].y < 1)
            testPoint[i].y = 1;
    }

    for (register unsigned char i = 0; i < 3; i++)
    {
        if (collision->getValue(testPoint[i].x / 32, testPoint[i].y / 32) == 1)
        {
            testPoint[0].x = roundf(testPoint[0].x / 32) * 32;

            collide = true;
            goto LBL_PA4;
        }
    }

    for (list<Collider*>::iterator it = more->begin(); it != more->end(); it++)
    {
        ColliderType type = (*it)->type;

        if (getID() != ID_PLAYER)
        {
            if (type == C_CENTIPEDE)
                continue;
        }

        if (type == C_STOMPABLE || type == C_UNSTOMPABLE)
            continue;

        if ((*it)->dir == 0)
        {
            if (mov > 0)
            {
                for (register unsigned char i = 0; i < 3; i++)
                {
                    if ((*it)->rect.contains(testPoint[i]))
                    {
                        if (type == C_CENTIPEDE)
                        {
                            if (testPoint[0].x - (*it)->rect.left < 8)
                                testPoint[0].x = (*it)->rect.left;
                        }
                        else
                            testPoint[0].x = (*it)->rect.left;

                        collide = true;
                        goto LBL_PA4;
                    }
                }
            }
            else
            {
                for (register unsigned char i = 0; i < 3; i++)
                {
                    if ((*it)->rect.contains(testPoint[i]))
                    {
                        if (type == C_CENTIPEDE)
                        {
                            if ((*it)->rect.left + (*it)->rect.width - testPoint[0].x < 8)
                                testPoint[0].x = (*it)->rect.left + (*it)->rect.width;
                        }
                        else
                            testPoint[0].x = (*it)->rect.left + (*it)->rect.width;

                        collide = true;
                        goto LBL_PA4;
                    }
                }
            }
        }
    }

    LBL_PA4 :

    if (mov > 0)
        m_aabb.left = testPoint[0].x - m_aabb.width;
    else
        m_aabb.left = testPoint[0].x;

    return collide;
}

bool Pawn::makeMoveVerticalPlatform(float mov, const Vector2i& levelSize, Matrix* collision, list<Collider*>* more)
{
    ID id = getID();
    Vector2f testPoint[3];

    bool collide = false;

    if (mov > 0)
    {
        testPoint[0] = Vector2f(m_aabb.left + 1, m_aabb.top + m_aabb.height + mov);
        testPoint[1] = Vector2f(m_aabb.left + m_aabb.width - 1, m_aabb.top + m_aabb.height + mov);

        if (testPoint[0].y < 1)
            goto LBL_PA5;

        if (testPoint[0].y > (levelSize.y * 480) - 1)
            goto LBL_PA5;
    }
    else
    {
        testPoint[0] = Vector2f(m_aabb.left + 1, m_aabb.top + mov);
        testPoint[1] = Vector2f(m_aabb.left + m_aabb.width - 1, m_aabb.top + mov);
        testPoint[1] = Vector2f(m_aabb.left + 16, m_aabb.top + mov);

        if ((id == ID_STOMPABLE || id == ID_ENEMY) && player != NULL)
        {
            if (player->m_active)
            {
                Collider* tmp(dynamic_cast<GotCollision*>(this)->m_collider);

                if (tmp->type == C_STOMPABLE)
                {
                    if (testPoint[0].y - mov >= player->m_aabb.top + player->m_aabb.height)
                    {
                        for (register unsigned char i = 0; i < 3; i++)
                        {
                            if (player->m_aabb.contains(testPoint[i]))
                            {
                                enemystomper.left = m_aabb.left - 1;
                                enemystomper.top = m_aabb.top - 6;
                                enemystomper_pt = tmp;

                                player->bounce(14);

                                break;
                            }
                        }
                    }
                }
            }
        }

        if (testPoint[0].y < 1)
            goto LBL_PA5;
    }

    for (register unsigned char i = 0; i < 2; i++)
    {
        if (testPoint[i].x < 1)
            testPoint[i].x = 1;
        else if (testPoint[i].x > (levelSize.x * 640) - 1)
            testPoint[i].x = (levelSize.x * 640) - 1;
    }

    for (register unsigned char i = 0; i < 2; i++)
    {
        if (collision->getValue(testPoint[i].x / 32, testPoint[i].y / 32) == 1)
        {
            testPoint[0].y = roundf(testPoint[0].y / 32) * 32;

            m_platform = NULL;

            collide = true;
            goto LBL_PA5;
        }
    }

    for (list<Collider*>::iterator it = more->begin(); it != more->end(); it++)
    {
        if ((*it)->object == this)
            continue;

        ColliderType type = (*it)->type;
        char dir = (*it)->dir;

        if (id != ID_PLAYER)
        {
            if (type == C_CENTIPEDE)
                continue;
        }

        if (type == C_STOMPABLE || type == C_UNSTOMPABLE)
            continue;

        if (mov > 0)
        {
            if (dir == 0)
            {
                if (type == C_PLATFORM)
                    continue;

                for (register unsigned char i = 0; i < 2; i++)
                {
                    if ((*it)->rect.contains(testPoint[i]))
                    {
                        testPoint[0].y = (*it)->rect.top;

                        m_platform = NULL;

                        collide = true;
                        goto LBL_PA5;
                    }
                }
            }
            else if (dir == 1 && id == ID_PLAYER)
            {
                if (testPoint[0].y - mov - 4 >= (*it)->rect.top)
                    continue;

                if ((*it) == m_platform)
                    continue;

                for (register unsigned char i = 0; i < 2; i++)
                {
                    if ((*it)->rect.contains(testPoint[i]))
                    {
                        testPoint[0].y = (*it)->rect.top;

                        if (type == C_PLATFORM)
                            m_platform = (*it);
                        else
                            m_platform = NULL;

                        collide = true;
                        goto LBL_PA5;
                    }
                }
            }
        }
        else
        {
            if (type == C_PLATFORM)
                continue;

            if (dir == 0)
            {
                for (register unsigned char i = 0; i < 2; i++)
                {
                    if ((*it)->rect.contains(testPoint[i]))
                    {
                        testPoint[0].y = (*it)->rect.top + (*it)->rect.height;

                        m_platform = NULL;

                        collide = true;
                        goto LBL_PA5;
                    }
                }
            }
        }
    }

    LBL_PA5 :

    if (mov > 0)
        m_aabb.top = testPoint[0].y - m_aabb.height;
    else
        m_aabb.top = testPoint[0].y;

    return collide;
}

bool Pawn::makeMoveHorizontalPlatform(float mov, const Vector2i& levelSize, Matrix* collision, list<Collider*>* more)
{
    Vector2f testPoint[2];

    bool collide = false;

    if (mov > 0)
    {
        testPoint[0] = Vector2f(m_aabb.left + m_aabb.width + mov, m_aabb.top + 1);
        testPoint[1] = Vector2f(m_aabb.left + m_aabb.width + mov, m_aabb.top + m_aabb.height - 1);

        if (testPoint[0].x > (levelSize.x * 640) - 1)
        {
            testPoint[0].x = (levelSize.x * 640) - 1;

            collide = true;
            goto LBL_PA6;
        }
    }
    else
    {
        testPoint[0] = Vector2f(m_aabb.left + mov, m_aabb.top + 1);
        testPoint[1] = Vector2f(m_aabb.left + mov, m_aabb.top + m_aabb.height - 1);

        if (testPoint[0].x < 1)
        {
            testPoint[0].x = 1;

            collide = true;
            goto LBL_PA6;
        }
    }

    for (register unsigned char i = 0; i < 2; i++)
    {
        if (testPoint[i].y > (levelSize.y * 480) - 1)
            testPoint[i].y = (levelSize.y * 480) - 1;
        else if (testPoint[i].y < 1)
            testPoint[i].y = 1;
    }

    for (register unsigned char i = 0; i < 2; i++)
    {
        if (collision->getValue(testPoint[i].x / 32, testPoint[i].y / 32) == 1)
        {
            testPoint[0].x = roundf(testPoint[0].x / 32) * 32;

            collide = true;
            goto LBL_PA6;
        }
    }

    for (list<Collider*>::iterator it = more->begin(); it != more->end(); it++)
    {
        ColliderType type = (*it)->type;

        if ((*it)->object == this)
            continue;

        if (getID() != ID_PLAYER)
        {
            if (type == C_CENTIPEDE)
                continue;
        }

        if (type == C_STOMPABLE || type == C_UNSTOMPABLE || type == C_PLATFORM)
            continue;

        if ((*it)->dir == 0)
        {
            if (mov > 0)
            {
                for (register unsigned char i = 0; i < 2; i++)
                {
                    if ((*it)->rect.contains(testPoint[i]))
                    {
                        testPoint[0].x = (*it)->rect.left;

                        collide = true;
                        goto LBL_PA6;
                    }
                }
            }
            else
            {
                for (register unsigned char i = 0; i < 2; i++)
                {
                    if ((*it)->rect.contains(testPoint[i]))
                    {
                        testPoint[0].x = (*it)->rect.left + (*it)->rect.width;

                        collide = true;
                        goto LBL_PA6;
                    }
                }
            }
        }
    }

    LBL_PA6 :

    if (mov > 0)
        m_aabb.left = testPoint[0].x - m_aabb.width;
    else
        m_aabb.left = testPoint[0].x;

    return collide;
}

bool Pawn::makeMoveVerticalHighPlatform(float mov, const Vector2i& levelSize, Matrix* collision, list<Collider*>* more)
{
    ID id = getID();
    Vector2f testPoint[4];

    bool collide = false;

    if (mov > 0)
    {
        testPoint[0] = Vector2f(m_aabb.left + 1, m_aabb.top + m_aabb.height + mov);
        testPoint[1] = Vector2f(m_aabb.left + 24, m_aabb.top + m_aabb.height + mov);
        testPoint[2] = Vector2f(m_aabb.left + 48, m_aabb.top + m_aabb.height + mov);
        testPoint[3] = Vector2f(m_aabb.left + m_aabb.width - 1, m_aabb.top + m_aabb.height + mov);

        if (testPoint[0].y < 1)
            goto LBL_PA7;

        if (testPoint[0].y > (levelSize.y * 480) - 1)
            goto LBL_PA7;
    }
    else
    {
        testPoint[0] = Vector2f(m_aabb.left + 1, m_aabb.top + mov);
        testPoint[1] = Vector2f(m_aabb.left + 24, m_aabb.top + mov);
        testPoint[2] = Vector2f(m_aabb.left + 48, m_aabb.top + mov);
        testPoint[3] = Vector2f(m_aabb.left + m_aabb.width - 1, m_aabb.top + mov);

        if (id == ID_STOMPABLE && player != NULL)
        {
            if (player->m_active)
            {
                if (testPoint[0].y - mov >= player->m_aabb.top + player->m_aabb.height)
                {
                    for (register unsigned char i = 0; i < 4; i++)
                    {
                        if (player->m_aabb.contains(testPoint[i]))
                        {
                            enemystomper.left = m_aabb.left - 1;
                            enemystomper.top = m_aabb.top - 6;
                            enemystomper_pt = dynamic_cast<GotCollision*>(this)->m_collider;

                            player->bounce(14);

                            break;
                        }
                    }
                }
            }
        }

        if (testPoint[0].y < 1)
            goto LBL_PA7;
    }

    for (register unsigned char i = 0; i < 4; i++)
    {
        if (testPoint[i].x < 1)
            testPoint[i].x = 1;
        else if (testPoint[i].x > (levelSize.x * 640) - 1)
            testPoint[i].x = (levelSize.x * 640) - 1;
    }

    for (register unsigned char i = 0; i < 4; i++)
    {
        if (collision->getValue(testPoint[i].x / 32, testPoint[i].y / 32) == 1)
        {
            testPoint[0].y = roundf(testPoint[0].y / 32) * 32;

            m_platform = NULL;

            collide = true;
            goto LBL_PA7;
        }
    }

    for (list<Collider*>::iterator it = more->begin(); it != more->end(); it++)
    {
        if ((*it)->object == this)
            continue;

        ColliderType type = (*it)->type;
        char dir = (*it)->dir;

        if (type == C_STOMPABLE || type == C_UNSTOMPABLE || type == C_CENTIPEDE)
            continue;

        if (mov > 0)
        {
            if (dir == 0)
            {
                if (type == C_PLATFORM)
                    continue;

                for (register unsigned char i = 0; i < 4; i++)
                {
                    if ((*it)->rect.contains(testPoint[i]))
                    {
                        testPoint[0].y = (*it)->rect.top;

                        m_platform = NULL;

                        collide = true;
                        goto LBL_PA7;
                    }
                }
            }
            else if (dir == 1 && getID() == ID_PLAYER)
            {
                if (testPoint[0].y - mov >= (*it)->rect.top)
                    continue;

                if ((*it) == m_platform)
                    continue;

                for (register unsigned char i = 0; i < 4; i++)
                {
                    if ((*it)->rect.contains(testPoint[i]))
                    {
                        testPoint[0].y = (*it)->rect.top;

                        if (type == C_PLATFORM)
                            m_platform = (*it);
                        else
                            m_platform = NULL;

                        collide = true;
                        goto LBL_PA7;
                    }
                }
            }
        }
        else
        {
            if (type == C_PLATFORM)
                continue;

            if (dir == 0)
            {
                for (register unsigned char i = 0; i < 4; i++)
                {
                    if ((*it)->rect.contains(testPoint[i]))
                    {
                        testPoint[0].y = (*it)->rect.top + (*it)->rect.height;

                        m_platform = NULL;

                        collide = true;
                        goto LBL_PA7;
                    }
                }
            }
        }
    }

    LBL_PA7 :

    if (mov > 0)
        m_aabb.top = testPoint[0].y - m_aabb.height;
    else
        m_aabb.top = testPoint[0].y;

    return collide;
}

bool Pawn::makeMoveHorizontalHighPlatform(float mov, const Vector2i& levelSize, Matrix* collision, list<Collider*>* more)
{
    Vector2f testPoint[3];

    bool collide = false;

    if (mov > 0)
    {
        testPoint[0] = Vector2f(m_aabb.left + m_aabb.width + mov, m_aabb.top + 1);
        testPoint[1] = Vector2f(m_aabb.left + m_aabb.width + mov, m_aabb.top + 24);
        testPoint[2] = Vector2f(m_aabb.left + m_aabb.width + mov, m_aabb.top + m_aabb.height - 1);

        if (testPoint[0].x > (levelSize.x * 640) - 1)
        {
            testPoint[0].x = (levelSize.x * 640) - 1;

            collide = true;
            goto LBL_PA8;
        }
    }
    else
    {
        testPoint[0] = Vector2f(m_aabb.left + mov, m_aabb.top + 1);
        testPoint[1] = Vector2f(m_aabb.left + mov, m_aabb.top + 24);
        testPoint[2] = Vector2f(m_aabb.left + mov, m_aabb.top + m_aabb.height - 1);

        if (testPoint[0].x < 1)
        {
            testPoint[0].x = 1;

            collide = true;
            goto LBL_PA8;
        }
    }

    for (register unsigned char i = 0; i < 3; i++)
    {
        if (testPoint[i].y > (levelSize.y * 480) - 1)
            testPoint[i].y = (levelSize.y * 480) - 1;
        else if (testPoint[i].y < 1)
            testPoint[i].y = 1;
    }

    for (register unsigned char i = 0; i < 3; i++)
    {
        if (collision->getValue(testPoint[i].x / 32, testPoint[i].y / 32) == 1)
        {
            testPoint[0].x = roundf(testPoint[0].x / 32) * 32;

            collide = true;
            goto LBL_PA8;
        }
    }

    for (list<Collider*>::iterator it = more->begin(); it != more->end(); it++)
    {
        ColliderType type = (*it)->type;

        if ((*it)->object == this)
            continue;

        if (getID() != ID_PLAYER)
        {
            if (type == C_CENTIPEDE)
                continue;
        }

        if (type == C_STOMPABLE || type == C_UNSTOMPABLE || type == C_PLATFORM)
            continue;

        if ((*it)->dir == 0)
        {
            if (mov > 0)
            {
                for (register unsigned char i = 0; i < 3; i++)
                {
                    if ((*it)->rect.contains(testPoint[i]))
                    {
                        testPoint[0].x = (*it)->rect.left;

                        collide = true;
                        goto LBL_PA8;
                    }
                }
            }
            else
            {
                for (register unsigned char i = 0; i < 3; i++)
                {
                    if ((*it)->rect.contains(testPoint[i]))
                    {
                        testPoint[0].x = (*it)->rect.left + (*it)->rect.width;

                        collide = true;
                        goto LBL_PA8;
                    }
                }
            }
        }
    }

    LBL_PA8 :

    if (mov > 0)
        m_aabb.left = testPoint[0].x - m_aabb.width;
    else
        m_aabb.left = testPoint[0].x;

    return collide;
}

bool Pawn::makeMoveHorizontalNR(float mov, const Vector2i& levelSize, Matrix* collision, list<Collider*>* more)
{
    Vector2f testPoint[2];

    bool collide = false;

    testPoint[0] = Vector2f(m_aabb.left + m_aabb.width + mov, m_aabb.top + 1);
    testPoint[1] = Vector2f(m_aabb.left + m_aabb.width + mov, m_aabb.top + m_aabb.height - 1);

    if (testPoint[0].x > (levelSize.x * 640) - 1)
        goto LBL_PA9;

    for (register unsigned char i = 0; i < 2; i++)
    {
        if (testPoint[i].y > (levelSize.y * 480) - 1)
            testPoint[i].y = (levelSize.y * 480) - 1;
        else if (testPoint[i].y < 1)
            testPoint[i].y = 1;
    }

    for (register unsigned char i = 0; i < 2; i++)
    {
        if (collision->getValue(testPoint[i].x / 32, testPoint[i].y / 32) == 1)
        {
            testPoint[0].x = roundf(testPoint[0].x / 32) * 32;

            collide = true;
            goto LBL_PA9;
        }
    }

    for (list<Collider*>::iterator it = more->begin(); it != more->end(); it++)
    {
        ColliderType type = (*it)->type;

        if (type == C_STOMPABLE || type == C_UNSTOMPABLE || type == C_CENTIPEDE)
            continue;

        if ((*it)->dir == 0)
        {
            for (register unsigned char i = 0; i < 2; i++)
            {
                if ((*it)->rect.contains(testPoint[i]))
                {
                    testPoint[0].x = (*it)->rect.left;

                    collide = true;
                    goto LBL_PA9;
                }
            }
        }
    }

    LBL_PA9 :

    m_aabb.left = testPoint[0].x - m_aabb.width;

    return collide;
}

bool Pawn::makeMoveHorizontalHighNR(float mov, const Vector2i& levelSize, Matrix* collision, list<Collider*>* more)
{
    Vector2f testPoint[3];

    bool collide = false;

    testPoint[0] = Vector2f(m_aabb.left + m_aabb.width + mov, m_aabb.top + 1);
    testPoint[1] = Vector2f(m_aabb.left + m_aabb.width + mov, m_aabb.top + 24);
    testPoint[2] = Vector2f(m_aabb.left + m_aabb.width + mov, m_aabb.top + m_aabb.height - 1);

    if (testPoint[0].x > (levelSize.x * 640) - 1)
        goto LBL_PA10;

    for (register unsigned char i = 0; i < 3; i++)
    {
        if (testPoint[i].y > (levelSize.y * 480) - 1)
            testPoint[i].y = (levelSize.y * 480) - 1;
        else if (testPoint[i].y < 1)
            testPoint[i].y = 1;
    }

    for (register unsigned char i = 0; i < 3; i++)
    {
        if (collision->getValue(testPoint[i].x / 32, testPoint[i].y / 32) == 1)
        {
            testPoint[0].x = roundf(testPoint[0].x / 32) * 32;

            collide = true;
            goto LBL_PA10;
        }
    }

    for (list<Collider*>::iterator it = more->begin(); it != more->end(); it++)
    {
        ColliderType type = (*it)->type;

        if (type == C_STOMPABLE || type == C_UNSTOMPABLE || type == C_CENTIPEDE)
            continue;

        if ((*it)->dir == 0)
        {
            for (register unsigned char i = 0; i < 3; i++)
            {
                if ((*it)->rect.contains(testPoint[i]))
                {
                    testPoint[0].x = (*it)->rect.left;

                    collide = true;
                    goto LBL_PA10;
                }
            }
        }
    }

    LBL_PA10 :

    m_aabb.left = testPoint[0].x - m_aabb.width;

    return collide;
}

bool Pawn::makeMoveVerticalNR(float mov, const Vector2i& levelSize, Matrix* collision, list<Collider*>* more)
{
    Vector2f testPoint[2];

    bool collide = false;

    if (mov > 0)
    {
        testPoint[0] = Vector2f(m_aabb.left + 1, m_aabb.top + m_aabb.height + mov);
        testPoint[1] = Vector2f(m_aabb.left + m_aabb.width - 1, m_aabb.top + m_aabb.height + mov);

        if (testPoint[0].y < 1)
            goto LBL_PA11;

        if (testPoint[0].y > (levelSize.y * 480) - 1)
            goto LBL_PA11;
    }
    else
    {
        testPoint[0] = Vector2f(m_aabb.left + 1, m_aabb.top + mov);
        testPoint[1] = Vector2f(m_aabb.left + m_aabb.width - 1, m_aabb.top + mov);

        if (testPoint[0].y < 1)
            goto LBL_PA11;
    }

    for (register unsigned char i = 0; i < 2; i++)
    {
        if (testPoint[i].x > (levelSize.x * 640) - 1)
            testPoint[i].x = (levelSize.x * 640) - 1;

        if (collision->getValue(testPoint[i].x / 32, testPoint[i].y / 32) == 1)
        {
            testPoint[0].y = roundf(testPoint[0].y / 32) * 32;

            collide = true;
            goto LBL_PA11;
        }
    }

    for (list<Collider*>::iterator it = more->begin(); it != more->end(); it++)
    {
        ColliderType type = (*it)->type;
        char dir = (*it)->dir;

        if (type == C_STOMPABLE || type == C_UNSTOMPABLE || type == C_CENTIPEDE)
            continue;

        if (mov > 0)
        {
            if (dir != 2)
            {
                for (register unsigned char i = 0; i < 2; i++)
                {
                    if ((*it)->rect.contains(testPoint[i]))
                    {
                        testPoint[0].y = (*it)->rect.top;

                        if (type == C_PLATFORM)
                            m_platform = *it;

                        collide = true;
                        goto LBL_PA11;
                    }
                }
            }
        }
        else
        {
            if (getID() != ID_PLAYER)
                continue;

            if (dir != 1)
            {
                for (register unsigned char i = 0; i < 2; i++)
                {
                    if ((*it)->rect.contains(testPoint[i]))
                    {
                        testPoint[0].y = (*it)->rect.top + (*it)->rect.height;

                        collide = true;
                        goto LBL_PA11;
                    }
                }
            }
        }
    }

    LBL_PA11 :

    if (mov > 0)
        m_aabb.top = testPoint[0].y - m_aabb.height;
    else
        m_aabb.top = testPoint[0].y;

    return collide;
}

bool Pawn::testVertical(float distance, const Vector2i& levelSize, Matrix* collision, list<Collider*>* more)
{
    Vector2f testPoint[2];

    if (distance > 0)
    {
        testPoint[0] = Vector2f(m_aabb.left + 1, m_aabb.top + m_aabb.height + distance);
        testPoint[1] = Vector2f(m_aabb.left + m_aabb.width - 1, m_aabb.top + m_aabb.height + distance);

        if (testPoint[0].y < 1)
        {
            testPoint[0].y = 1;
            testPoint[1].y = 1;
        }

        /*if (testPoint[0].y > (levelSize.y * 480) - 1)
            return false;*/

        m_platform = NULL;
    }
    else
    {
        testPoint[0] = Vector2f(m_aabb.left + 1, m_aabb.top + distance);
        testPoint[1] = Vector2f(m_aabb.left + m_aabb.width - 1, m_aabb.top + distance);

        if (testPoint[0].y < 1)
        {
            testPoint[0].y = 1;
            testPoint[1].y = 1;
        }
    }

    for (register unsigned char i = 0; i < 2; i++)
    {
        if (testPoint[i].x < 1)
            testPoint[i].x = 1;
        else if (testPoint[i].x > (levelSize.x * 640) - 1)
            testPoint[i].x = (levelSize.x * 640) - 1;
    }

    for (register unsigned char i = 0; i < 2; i++)
    {
        if (collision->getValue(testPoint[i].x / 32, testPoint[i].y / 32) == 1)
            return true;
    }

    for (list<Collider*>::iterator it = more->begin(); it != more->end(); it++)
    {
        ColliderType type = (*it)->type;
        char dir = (*it)->dir;

        if (getID() != ID_PLAYER)
        {
            if (type == C_CENTIPEDE)
                continue;
        }

        if (type == C_STOMPABLE || type == C_UNSTOMPABLE)
            continue;

        if (distance > 0)
        {
            if (dir != 2)
            {
                for (register unsigned char i = 0; i < 2; i++)
                {
                    if ((*it)->rect.contains(testPoint[i]))
                    {
                        if (type == C_PLATFORM)
                            m_platform = *it;

                        return true;
                    }
                }
            }
        }
        else
        {
            if (dir != 1)
            {
                for (register unsigned char i = 0; i < 2; i++)
                {
                    if ((*it)->rect.contains(testPoint[i]))
                        return true;
                }
            }
        }
    }

    return false;
}

bool Pawn::testHorizontal(float distance, const Vector2i& levelSize, Matrix* collision, list<Collider*>* more)
{
    Vector2f testPoint[2];

    if (distance > 0)
    {
        testPoint[0] = Vector2f(m_aabb.left + m_aabb.width + distance, m_aabb.top + 1);
        testPoint[1] = Vector2f(m_aabb.left + m_aabb.width + distance, m_aabb.top + m_aabb.height - 1);

        if (testPoint[0].x > (levelSize.x * 640) - 1)
            return false;
    }
    else
    {
        testPoint[0] = Vector2f(m_aabb.left + distance, m_aabb.top + 1);
        testPoint[1] = Vector2f(m_aabb.left + distance, m_aabb.top + m_aabb.height - 1);

        if (testPoint[0].x < 1)
            return false;
    }

    for (register unsigned char i = 0; i < 2; i++)
    {
        if (testPoint[i].y > (levelSize.y * 480) - 1)
            testPoint[i].y = (levelSize.y * 480) - 1;
        else if (testPoint[i].y < 1)
            testPoint[i].y = 1;
    }

    for (register unsigned char i = 0; i < 2; i++)
    {
        if (collision->getValue(testPoint[i].x / 32, testPoint[i].y / 32) == 1)
            return true;
    }

    for (list<Collider*>::iterator it = more->begin(); it != more->end(); it++)
    {
        ColliderType type = (*it)->type;

        if (getID() != ID_PLAYER)
        {
            if (type == C_CENTIPEDE)
                continue;
        }

        if (type == C_STOMPABLE || type == C_UNSTOMPABLE)
            continue;

        if ((*it)->dir == 0)
        {
            for (register unsigned char i = 0; i < 2; i++)
            {
                if ((*it)->rect.contains(testPoint[i]))
                    return true;
            }
        }
    }

    return false;
}

bool Pawn::testVerticalHigh(float distance, const Vector2i& levelSize, Matrix* collision, list<Collider*>* more)
{
    Vector2f testPoint[4];

    testPoint[0] = Vector2f(m_aabb.left + 1, m_aabb.top + m_aabb.height + distance);
    testPoint[1] = Vector2f(m_aabb.left + 24, m_aabb.top + m_aabb.height + distance);
    testPoint[2] = Vector2f(m_aabb.left + 48, m_aabb.top + m_aabb.height + distance);
    testPoint[3] = Vector2f(m_aabb.left + m_aabb.width - 1, m_aabb.top + m_aabb.height + distance);

    /*if (testPoint[0].y > (levelSize.y * 480) - 1)
        return false;*/

    m_platform = NULL;

    for (register unsigned char i = 0; i < 4; i++)
    {
        if (testPoint[i].x < 1)
            testPoint[i].x = 1;
        else if (testPoint[i].x > (levelSize.x * 640) - 1)
            testPoint[i].x = (levelSize.x * 640) - 1;
    }

    for (register unsigned char i = 0; i < 4; i++)
    {
        if (collision->getValue(testPoint[i].x / 32, testPoint[i].y / 32) == 1)
            return true;
    }

    for (list<Collider*>::iterator it = more->begin(); it != more->end(); it++)
    {
        ColliderType type = (*it)->type;
        char dir = (*it)->dir;

        if (type == C_STOMPABLE || type == C_UNSTOMPABLE || type == C_CENTIPEDE)
            continue;

        if (dir != 2)
        {
            for (register unsigned char i = 0; i < 4; i++)
            {
                if ((*it)->rect.contains(testPoint[i]))
                {
                    if (type == C_PLATFORM)
                        m_platform = *it;

                    return true;
                }
            }
        }
    }

    return false;
}

bool Pawn::testHorizontalHigh(float distance, bool right, const Vector2i& levelSize, Matrix* collision, list<Collider*>* more)
{
    Vector2f testPoint[3];

    if (right)
    {
        testPoint[0] = Vector2f(m_aabb.left + m_aabb.width + distance, m_aabb.top + 1);
        testPoint[1] = Vector2f(m_aabb.left + m_aabb.width + distance, m_aabb.top + 24);
        testPoint[2] = Vector2f(m_aabb.left + m_aabb.width + distance, m_aabb.top + m_aabb.height - 1);

        if (testPoint[0].x > (levelSize.x * 640) - 1)
            return false;
    }
    else
    {
        testPoint[0] = Vector2f(m_aabb.left + distance, m_aabb.top + 1);
        testPoint[1] = Vector2f(m_aabb.left + distance, m_aabb.top + 24);
        testPoint[2] = Vector2f(m_aabb.left + distance, m_aabb.top + m_aabb.height - 1);

        if (testPoint[0].x < 1)
            return false;
    }

    for (register unsigned char i = 0; i < 3; i++)
    {
        if (testPoint[i].y > (levelSize.y * 480) - 1)
            testPoint[i].y = (levelSize.y * 480) - 1;
        else if (testPoint[i].y < 1)
            testPoint[i].y = 1;
    }

    for (register unsigned char i = 0; i < 3; i++)
    {
        if (collision->getValue(testPoint[i].x / 32, testPoint[i].y / 32) == 1)
            return true;
    }

    for (list<Collider*>::iterator it = more->begin(); it != more->end(); it++)
    {
        ColliderType type = (*it)->type;

        if (getID() != ID_PLAYER)
        {
            if (type == C_CENTIPEDE)
                continue;
        }

        if (type == C_STOMPABLE || type == C_UNSTOMPABLE)
            continue;

        if ((*it)->dir == 0)
        {
            for (register unsigned char i = 0; i < 3; i++)
            {
                if ((*it)->rect.contains(testPoint[i]))
                    return true;
            }
        }
    }

    return false;
}
