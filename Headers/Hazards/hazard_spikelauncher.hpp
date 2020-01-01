/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#ifndef HAZARDSPIKELAUNCHER_INCLUDED
#define HAZARDSPIKELAUNCHER_INCLUDED

#include <SFML/Graphics.hpp>
#include <list>
#include "../Core/placeable.hpp"
#include "../Miscs/projectile_spike.hpp"

class Hazard_Spikelauncher : public Placeable
{
    public :

        Hazard_Spikelauncher(const sf::Texture& texture);
        virtual ~Hazard_Spikelauncher();

        void setPosition(const sf::Vector2f& pos);

        void update();
        void secureUpdate();
        void afterUpdate();

        virtual void shot() = 0;

        ID getID() const;

    protected :

        sf::Sprite m_sprite;

        std::list<Projectile_Spike*> m_spike;

        unsigned char m_timer;
};

#endif
