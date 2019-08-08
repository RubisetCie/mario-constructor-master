/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#ifndef HAZARDSPIKELAUNCHERB_INCLUDED
#define HAZARDSPIKELAUNCHERB_INCLUDED

#include <SFML/Graphics.hpp>
#include "hazard_spikelauncher.hpp"

class Hazard_SpikelauncherB : public Hazard_Spikelauncher
{
    public :

        Hazard_SpikelauncherB(const sf::Texture& texture);

        void shot();

        sf::Vector2f getPosition() const;
        sf::Vector2f getSize() const;

    private :

        void draw(sf::RenderTarget& target, sf::RenderStates) const;
};

#endif
