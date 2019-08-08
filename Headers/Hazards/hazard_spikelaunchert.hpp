/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#ifndef HAZARDSPIKELAUNCHERT_INCLUDED
#define HAZARDSPIKELAUNCHERT_INCLUDED

#include <SFML/Graphics.hpp>
#include "hazard_spikelauncher.hpp"

class Hazard_SpikelauncherT : public Hazard_Spikelauncher
{
    public :

        Hazard_SpikelauncherT(const sf::Texture& texture);

        void shot();

        sf::Vector2f getPosition() const;
        sf::Vector2f getSize() const;

    private :

        void draw(sf::RenderTarget& target, sf::RenderStates) const;
};

#endif
