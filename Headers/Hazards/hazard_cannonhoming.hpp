/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#ifndef HAZARDCANNONHOMING_INCLUDED
#define HAZARDCANNONHOMING_INCLUDED

#include <SFML/Graphics.hpp>
#include <list>
#include "../Core/placeable.hpp"
#include "../Miscs/projectile_bulletbill.hpp"

class Hazard_CannonHoming : public Placeable
{
    public :

        Hazard_CannonHoming(const sf::Texture& texture);
        virtual ~Hazard_CannonHoming();

        void setPosition(const sf::Vector2f& pos);

        void update();
        void secureUpdate();
        void afterUpdate();

        virtual void shot() = 0;

        ID getID() const;

    protected :

        sf::Sprite m_sprite;

        Projectile_Bulletbill* m_bullet;

        unsigned char m_timer;
};

#endif
