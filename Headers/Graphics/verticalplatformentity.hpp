/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#ifndef VERTICALPLATFORM_ENTITY_INCLUDED
#define VERTICALPLATFORM_ENTITY_INCLUDED

#include <SFML/Graphics.hpp>
#include "entity.hpp"

class VerticalPlatform_Entity : public Entity
{
    public :

        VerticalPlatform_Entity(sf::Texture* texture, sf::Texture* speedRegulatorTexture, sf::Texture* sliderTexture, sf::Texture* platformEndTex, sf::Texture* editorMoveable, sf::Texture* platformText, sf::IntRect textRect, float x, float y, float originx, float originy, float regulatorPos, unsigned int type, bool iscloud);

        sf::Vector2f getPosition() const;
        sf::Vector2u getSize() const;

        void roomResized(sf::Vector2i newsize);

        void save(std::vector<float>& entData);
        void load(std::vector<float>& entData);

        void update();
        void drawb(sf::RenderTarget* target) const;

    private :

        void draw(sf::RenderTarget& target, sf::RenderStates) const;

        sf::Sprite m_sprites[6];
        sf::Vertex m_vertex[2];

        bool m_grabslider;
        bool m_grabend;
};

#endif
