/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#ifndef HAZARDLAVALAUNCHER_INCLUDED
#define HAZARDLAVALAUNCHER_INCLUDED

#include <SFML/Graphics.hpp>
#include "../Core/placeable.hpp"
#include "../Miscs/projectile_lava.hpp"

class Hazard_Lavalauncher : public Placeable
{
    public :

        Hazard_Lavalauncher(const sf::Texture& texture);
        virtual ~Hazard_Lavalauncher();

        void setPosition(const sf::Vector2f& pos);

        void update();
        void secureUpdate();
        void afterUpdate();

        virtual void shot() = 0;

        ID getID() const;

    protected :

        std::vector<Projectile_Lava*> m_lava;

        sf::Sprite m_sprite;

        unsigned char m_delay, m_timer;
};

#endif
