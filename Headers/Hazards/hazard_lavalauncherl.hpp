/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#ifndef HAZARDLAVALAUNCHERL_INCLUDED
#define HAZARDLAVALAUNCHERL_INCLUDED

#include <SFML/Graphics.hpp>
#include "hazard_lavalauncher.hpp"

class Hazard_LavalauncherL : public Hazard_Lavalauncher
{
    public :

        Hazard_LavalauncherL(const sf::Texture& texture);

        void shot();

        sf::Vector2f getPosition() const;
        sf::Vector2f getSize() const;

    private :

        void draw(sf::RenderTarget& target, sf::RenderStates) const;
};

#endif
