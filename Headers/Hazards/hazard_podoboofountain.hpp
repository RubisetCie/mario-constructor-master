/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#ifndef HAZARDPODOBOOFONTAIN_INCLUDED
#define HAZARDPODOBOOFONTAIN_INCLUDED

#include <SFML/Graphics.hpp>
#include <list>
#include "../Miscs/projectile_podoboo.hpp"
#include "../Core/placeable.hpp"

class Hazard_PodobooFountain : public Placeable
{
    public :

        Hazard_PodobooFountain();
        virtual ~Hazard_PodobooFountain();

        void setPosition(const sf::Vector2f& pos);

        void update();
        void secureUpdate();
        void afterUpdate();

        sf::Vector2f getPosition() const;
        sf::Vector2f getSize() const;

        ID getID() const;

    protected :

        sf::Vector2f m_position;

        std::list<Projectile_Podoboo*> m_podoboo;

        unsigned short m_timer;
};

#endif
