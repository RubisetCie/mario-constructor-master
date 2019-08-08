/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#ifndef EFFECTSCORE_INCLUDED
#define EFFECTSCORE_INCLUDED

#include <SFML/Graphics.hpp>
#include "../Core/effect.hpp"

class Effect_Score : public Effect
{
    public :

        Effect_Score(sf::Texture* scoreTexture, unsigned int value, sf::Vector2f position);

        void update();

    private :

        void draw(sf::RenderTarget& target, sf::RenderStates) const;

        unsigned int m_vanishcounter;
};

#endif
