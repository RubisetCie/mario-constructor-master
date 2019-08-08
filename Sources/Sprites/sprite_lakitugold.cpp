/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#include <SFML/Graphics.hpp>
#include "../../Headers/Sprites/sprite_lakitugold.hpp"
#include "../../Headers/Sprites/sprite_throwedcoin.hpp"
#include "../../Headers/globals.hpp"

#ifdef DEBUGMODE
#include <iostream>
#endif

using namespace std;
using namespace sf;

Sprite_LakituGold::Sprite_LakituGold(const Texture& texture, Collider* collision) : Sprite_Lakitu(texture, collision)
{
}

void Sprite_LakituGold::thruw()
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
        Sprite_ThrowedCoin* coin[2] = {NULL, NULL};

        switch (rand() % 3)
        {
            case 0 :
                coin[0] = new Sprite_ThrowedCoin(*itemsTxt[2], Vector2f(1, -3),  this);
                coin[0]->setPosition(Vector2f(m_aabb.left - 3, m_aabb.top - 14));
                break;
            case 1 :
                coin[0] = new Sprite_ThrowedCoin(*itemsTxt[2], Vector2f(-1, -3),  this);
                coin[0]->setPosition(Vector2f(m_aabb.left - 3, m_aabb.top - 14));
                break;
            case 2 :
                coin[0] = new Sprite_ThrowedCoin(*itemsTxt[2], Vector2f(-1, -3),  this);
                coin[1] = new Sprite_ThrowedCoin(*itemsTxt[2], Vector2f(1, -3),  this);

                coin[0]->setPosition(Vector2f(m_aabb.left - 12, m_aabb.top - 14));
                coin[1]->setPosition(Vector2f(m_aabb.left + 6, m_aabb.top - 14));

                break;
        }

        if (zoneb)
        {
            if (coin[0] != NULL)
                listPlaceablesb.emplace(spriteIteratorb, coin[0]);

            if (coin[1] != NULL)
                listPlaceablesb.emplace(spriteIteratorb, coin[1]);
        }
        else
        {
            if (coin[0] != NULL)
                listPlaceables.emplace(spriteIterator, coin[0]);

            if (coin[1] != NULL)
                listPlaceables.emplace(spriteIterator, coin[1]);
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

void Sprite_LakituGold::draw(RenderTarget& target, RenderStates) const
{
    target.draw(m_array, m_texture);
}
