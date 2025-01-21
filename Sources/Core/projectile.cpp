/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#include <SFML/Graphics.hpp>
#include <cmath>

#include "../../Headers/Core/projectile.hpp"
#include "../../Headers/globals.hpp"

using namespace std;
using namespace sf;

bool Projectile::makeMoveVertical(float mov, const Vector2u& levelSize, Matrix* collision, list<Collider*>* more, bool hitenemies)
{
    Vector2f testPoint[2];

    bool collide = false;

    if (mov > 0)
    {
        testPoint[0] = Vector2f(m_aabb.left + 1, m_aabb.top + m_aabb.height + mov);
        testPoint[1] = Vector2f(m_aabb.left + m_aabb.width - 1, m_aabb.top + m_aabb.height + mov);

        if (testPoint[0].y > (levelSize.y * 480) - 1)
            goto LBL_PENDLOOP;
    }
    else
    {
        testPoint[0] = Vector2f(m_aabb.left + 1, m_aabb.top + mov);
        testPoint[1] = Vector2f(m_aabb.left + m_aabb.width - 1, m_aabb.top + mov);

        if (testPoint[0].y < 1)
            goto LBL_PENDLOOP;
    }

    for (register unsigned char i = 0; i < 2; i++)
    {
        if (testPoint[i].x > (levelSize.x * 640) - 1 || testPoint[i].x < 1)
            goto LBL_PENDLOOP;
    }

    for (register unsigned char i = 0; i < 2; i++)
    {
        if (collision->getValue(testPoint[i].x / 32, testPoint[i].y / 32) == 1)
        {
            testPoint[0].y = roundf(testPoint[i].y / 32) * 32;

            collide = true;
            goto LBL_PENDLOOP;
        }
    }

    for (list<Collider*>::iterator it = more->begin(); it != more->end(); it++)
    {
        ColliderType type = (*it)->type;
        char dir = (*it)->dir;

        if (!hitenemies)
        {
            if (type == C_STOMPABLE || type == C_UNSTOMPABLE)
                continue;
        }

        if (dir == 1)
        {
            if (mov > 0)
            {
                for (register unsigned char i = 0; i < 2; i++)
                {
                    if (testPoint[i].y - mov > (*it)->rect.top)
                        continue;

                    if ((*it)->rect.contains(testPoint[i]))
                    {
                        testPoint[0].y = (*it)->rect.top;

                        collide = true;
                        goto LBL_PENDLOOP;
                    }
                }
            }
        }
        else if (dir == 0)
        {
            if (mov > 0)
            {
                for (register unsigned char i = 0; i < 2; i++)
                {
                    if ((*it)->rect.contains(testPoint[i]))
                    {
                        if (type == C_STOMPABLE || type == C_UNSTOMPABLE)
                        {
                            enemykiller.left = testPoint[0].x-2;
                            enemykiller.top = testPoint[0].y-4;
                            enemykiller.width = 27;
                            enemykiller.height = 8;
                            enemykiller_fire = 1;
                            enemykiller_create = false;
                        }
                        else
                            testPoint[0].y = (*it)->rect.top;

                        collide = true;
                        goto LBL_PENDLOOP;
                    }
                }
            }
            else
            {
                for (register unsigned char i = 0; i < 2; i++)
                {
                    if ((*it)->rect.contains(testPoint[i]))
                    {
                        if (type == C_STOMPABLE || type == C_UNSTOMPABLE)
                        {
                            enemykiller.left = testPoint[0].x-2;
                            enemykiller.top = testPoint[0].y-4;
                            enemykiller.width = 27;
                            enemykiller.height = 8;
                            enemykiller_fire = 1;
                            enemykiller_create = false;
                        }
                        else
                            testPoint[0].y = (*it)->rect.top + (*it)->rect.height;

                        collide = true;
                        goto LBL_PENDLOOP;
                    }
                }
            }
        }
    }

    LBL_PENDLOOP :

    if (mov > 0)
        m_aabb.top = testPoint[0].y - m_aabb.height;
    else
        m_aabb.top = testPoint[0].y;

    return collide;
}

bool Projectile::makeMoveHorizontal(float mov, const Vector2u& levelSize, Matrix* collision, list<Collider*>* more, bool hitenemies)
{
    Vector2f testPoint[2];

    bool collide = false;

    if (mov > 0)
    {
        testPoint[0] = Vector2f(m_aabb.left + m_aabb.width + mov, m_aabb.top + 1);
        testPoint[1] = Vector2f(m_aabb.left + m_aabb.width + mov, m_aabb.top + m_aabb.height - 1);

        if (testPoint[0].x > (levelSize.x * 640) - 1)
            goto LBL_PENDLOOPH;
    }
    else
    {
        testPoint[0] = Vector2f(m_aabb.left + mov, m_aabb.top + 1);
        testPoint[1] = Vector2f(m_aabb.left + mov, m_aabb.top + m_aabb.height - 1);

        if (testPoint[0].x < 1)
            goto LBL_PENDLOOPH;
    }

    for (register unsigned char i = 0; i < 2; i++)
    {
        if (testPoint[i].y > (levelSize.y * 480) - 1 || testPoint[i].y < 1)
            goto LBL_PENDLOOPH;
    }

    for (register unsigned char i = 0; i < 2; i++)
    {
        if (collision->getValue(testPoint[i].x / 32, testPoint[i].y / 32) == 1)
        {
            testPoint[0].x = roundf(testPoint[i].x / 32) * 32;

            collide = true;
            goto LBL_PENDLOOPH;
        }
    }

    for (list<Collider*>::iterator it = more->begin(); it != more->end(); it++)
    {
        ColliderType type = (*it)->type;

        if (!hitenemies)
        {
            if (type == C_STOMPABLE || type == C_UNSTOMPABLE)
                continue;
        }

        if ((*it)->dir == 0)
        {
            if (mov > 0)
            {
                for (register unsigned char i = 0; i < 2; i++)
                {
                    if ((*it)->rect.contains(testPoint[i]))
                    {
                        testPoint[0].x = (*it)->rect.left;

                        if (type == C_STOMPABLE || type == C_UNSTOMPABLE)
                        {
                            enemykiller.left = testPoint[0].x-24;
                            enemykiller.top = testPoint[0].y-8;
                            enemykiller.width = 32;
                            enemykiller.height = 32;
                            enemykiller_fire = 0;
                            enemykiller_create = false;
                        }

                        collide = true;
                        goto LBL_PENDLOOPH;
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

                        if (type == C_STOMPABLE || type == C_UNSTOMPABLE)
                        {
                            enemykiller.left = testPoint[0].x-8;
                            enemykiller.top = testPoint[0].y-8;
                            enemykiller.width = 32;
                            enemykiller.height = 32;
                            enemykiller_fire = 0;
                            enemykiller_create = false;
                        }

                        collide = true;
                        goto LBL_PENDLOOPH;
                    }
                }
            }
        }
    }

    LBL_PENDLOOPH :

    if (mov > 0)
        m_aabb.left = testPoint[0].x - m_aabb.width;
    else
        m_aabb.left = testPoint[0].x;

    return collide;
}
