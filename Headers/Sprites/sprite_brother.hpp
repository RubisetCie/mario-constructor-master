/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#ifndef SPRITEBROTHER_INCLUDED
#define SPRITEBROTHER_INCLUDED

#include <SFML/Graphics.hpp>
#include "gotcollision.hpp"
#include "../Core/pawn.hpp"

class Sprite_Brother : public GotCollision, public Pawn
{
    public :

        Sprite_Brother(const sf::Texture& texture, Collider* collision);

        void setPosition(const sf::Vector2f& pos);

        void update();
        void secureUpdate();

        virtual void thruw() = 0;
        virtual void dead() = 0;

        ID getID() const;

        sf::Vector2f m_movedistance;

    protected :

        sf::Sprite m_sprite;

        float m_limit[2];
        float m_jumppoints[2];

        short m_alarm[5];

        bool m_killed, m_onfloor, m_upper, m_passthrough;
        unsigned char m_frame, m_timer, m_animspeed;
};

#endif
