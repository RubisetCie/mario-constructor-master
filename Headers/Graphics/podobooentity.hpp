/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#ifndef PODOBOO_ENTITY_INCLUDED
#define PODOBOO_ENTITY_INCLUDED

#include <SFML/Graphics.hpp>
#include "entity.hpp"

class Podoboo_Entity : public Entity
{
    public :

        Podoboo_Entity(sf::Texture* texture, sf::Texture* platformEndTex, sf::Texture* editorMoveable, float x, float y, float originx, float originy);

        sf::Vector2f getPosition() const;
        sf::Vector2u getSize() const;

        unsigned int getType() const;

        void save(std::vector<float>& entData);
        void load(std::vector<float>& entData);

        void update();
        void drawb(sf::RenderTarget* target) const;

    private :

        void draw(sf::RenderTarget& target, sf::RenderStates) const;

        sf::Sprite m_sprites[2];
        sf::Vertex m_vertex[2];

        bool m_grabend;
};

#endif
