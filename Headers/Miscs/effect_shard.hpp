/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#ifndef EFFECTSHARD_INCLUDED
#define EFFECTSHARD_INCLUDED

#include <SFML/Graphics.hpp>
#include "../Core/effect.hpp"

class Effect_Shard : public Effect
{
    public :

        Effect_Shard(sf::Texture* shardTexture, sf::Vector2f speed, sf::Vector2f position, float startangle, float height);

        void update();

    private :

        void draw(sf::RenderTarget& target, sf::RenderStates) const;

        sf::Vector2f m_movedistance;
        float m_startheight;
};

#endif
