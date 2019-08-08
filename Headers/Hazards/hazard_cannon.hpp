/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#ifndef HAZARDCANNON_INCLUDED
#define HAZARDCANNON_INCLUDED

#include <SFML/Graphics.hpp>
#include <list>
#include "../Core/placeable.hpp"
#include "../Miscs/projectile_bulletbill.hpp"

class Hazard_Cannon : public Placeable
{
    public :

        Hazard_Cannon(const sf::Texture& texture);
        virtual ~Hazard_Cannon();

        void setPosition(sf::Vector2f pos);

        void update();
        void secureUpdate();
        void afterUpdate();

        virtual void shot() = 0;

        ID getID() const;

    protected :

        sf::Sprite m_sprite;

        std::list<Projectile_Bulletbill*> m_bullets;

        unsigned char m_timer;
};

#endif
