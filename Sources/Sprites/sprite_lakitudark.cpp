/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#include <SFML/Graphics.hpp>
#include "../../Headers/Sprites/sprite_lakitudark.hpp"
#include "../../Headers/Sprites/sprite_spinygreyball.hpp"
#include "../../Headers/globals.hpp"

#ifdef DEBUGMODE
#include <iostream>
#endif

using namespace std;
using namespace sf;

Sprite_LakituDark::Sprite_LakituDark(const Texture& texture, Collider* collision) : Sprite_Lakitu(texture, collision)
{
}

void Sprite_LakituDark::thruw()
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

    if (!testVertical(-16, *currentScale, currentMatrix, currentList))
    {
        Collider* collid = new Collider{FloatRect(Vector2f(m_aabb.left, m_aabb.top - 15), Vector2f(32, 32)), Vector2f(0, 0), 0, C_UNSTOMPABLE, false};
        Sprite_SpinyGreyBall* ball = new Sprite_SpinyGreyBall(*enemiesTxt[36], *enemiesTxt[3], collid, this);

        ball->setPosition(Vector2f(m_aabb.left + 16, m_aabb.top));
        collid->object = ball;

        if (zoneb)
        {
            listColliderb.emplace_back(collid);
            listPlaceablesb.emplace(spriteIteratorb, ball);
        }
        else
        {
            listCollider.emplace_back(collid);
            listPlaceables.emplace(spriteIterator, ball);
        }

        m_alarm[1] = rand() % 60 + 45;
        m_alarm[2] = rand() % 50 - 120;

        m_enemies--;

        switch (rand() % 3)
        {
            case 0 : FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(11), sfxSamples[14], 0, NULL); break;
            case 1 : FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(11), sfxSamples[15], 0, NULL); break;
            case 2 : FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(11), sfxSamples[16], 0, NULL); break;
        }
    }
}

void Sprite_LakituDark::draw(RenderTarget& target, RenderStates) const
{
    target.draw(m_array, m_texture);
}
