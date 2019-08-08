/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#ifndef HAZARDLAVALAUNCHERVR_INCLUDED
#define HAZARDLAVALAUNCHERVR_INCLUDED

#include <SFML/Graphics.hpp>
#include "hazard_lavalauncher.hpp"

class Hazard_LavalauncherVR : public Hazard_Lavalauncher
{
    public :

        Hazard_LavalauncherVR(const sf::Texture& texture);

        void shot();

        sf::Vector2f getPosition() const;
        sf::Vector2f getSize() const;

    private :

        void draw(sf::RenderTarget& target, sf::RenderStates) const;
};

#endif
