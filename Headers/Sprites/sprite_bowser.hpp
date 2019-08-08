/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#ifndef SPRITEBOWSER_INCLUDED
#define SPRITEBOWSER_INCLUDED

#include <SFML/Graphics.hpp>
#include <queue>
#include <list>
#include "gotcollision.hpp"
#include "../Core/pawn.hpp"
#include "../Graphics/trace.hpp"
#include "../Miscs/projectile_flame.hpp"
#include "../Miscs/projectile_bulletbill.hpp"

class Sprite_Bowser : public Pawn, public GotCollision, public Trace
{
    public :

        Sprite_Bowser(const sf::Texture& texture, Collider* collision);
        virtual ~Sprite_Bowser();

        void setPosition(sf::Vector2f pos);

        void update();
        void secureUpdate();
        void afterUpdate();

        sf::Vector2f getPosition() const;
        sf::Vector2f getSize() const;

        ID getID() const;

        sf::Vector2f m_movedistance;

    private :

        void draw(sf::RenderTarget& target, sf::RenderStates) const;

        sf::Sprite m_sprite, m_cannon;

        std::list<Projectile_Flame*> m_flames;
        std::list<Projectile_Bulletbill*> m_bullets;

        std::queue<unsigned char> m_pendings;

        short m_alarm[6];

        float m_limit[2];
        float m_cannonspeed;

        short m_deathtimer;

        unsigned char m_frame, m_timer, m_firehealth, m_currentattack, m_invincible;
        char m_fireready, m_tracetimer;

        bool m_onfloor, m_fadestate;
};

#endif
