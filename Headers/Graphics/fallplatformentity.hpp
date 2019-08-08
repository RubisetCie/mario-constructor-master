/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#ifndef FALLPLATFORM_ENTITY_INCLUDED
#define FALLPLATFORM_ENTITY_INCLUDED

#include <SFML/Graphics.hpp>
#include "entity.hpp"

class FallPlatform_Entity : public Entity
{
    public :

        FallPlatform_Entity(sf::Texture* texture, sf::Texture* platformText, float x, float y, float originx, float originy, float textPos, unsigned int type, bool iscloud);

        sf::Vector2f getPosition() const;
        sf::Vector2u getSize() const;

        void drawb(sf::RenderTarget* target) const;
        void update();

    private :

        void draw(sf::RenderTarget& target, sf::RenderStates) const;

        sf::Sprite m_platformtext;
};

#endif
