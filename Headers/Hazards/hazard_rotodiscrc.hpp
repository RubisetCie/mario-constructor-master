/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#ifndef HAZARDROTODISCRC_INCLUDED
#define HAZARDROTODISCRC_INCLUDED

#include <SFML/Graphics.hpp>
#include "hazard_rotodisc.hpp"

class Hazard_RotodiscRC : public Hazard_Rotodisc
{
    public :

        Hazard_RotodiscRC(const sf::Texture& texture, const sf::Vector2f& disk_pos, float speed);

        void setPosition(const sf::Vector2f& pos);

        void secureUpdate();

    private :

        void draw(sf::RenderTarget& target, sf::RenderStates) const;

        float m_distance, m_quarterspeed;
        bool m_grow;
};

#endif
