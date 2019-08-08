/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#ifndef EFFECTCOIN_INCLUDED
#define EFFECTCOIN_INCLUDED

#include <SFML/Graphics.hpp>
#include "../Core/effect.hpp"

class Effect_Coin : public Effect
{
    public :

        Effect_Coin(sf::Texture* coinTexture, sf::Texture* vanishTexture, sf::Vector2f position);

        void update();

    private :

        void draw(sf::RenderTarget& target, sf::RenderStates) const;

        sf::Texture* m_vanishtex;

        float m_movedistance;

        unsigned int m_vanishframe;
};

#endif
