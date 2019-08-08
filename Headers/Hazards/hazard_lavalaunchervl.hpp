/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#ifndef HAZARDLAVALAUNCHERVL_INCLUDED
#define HAZARDLAVALAUNCHERVL_INCLUDED

#include <SFML/Graphics.hpp>
#include "hazard_lavalauncher.hpp"

class Hazard_LavalauncherVL : public Hazard_Lavalauncher
{
    public :

        Hazard_LavalauncherVL(const sf::Texture& texture);

        void shot();

        sf::Vector2f getPosition() const;
        sf::Vector2f getSize() const;

    private :

        void draw(sf::RenderTarget& target, sf::RenderStates) const;
};

#endif
