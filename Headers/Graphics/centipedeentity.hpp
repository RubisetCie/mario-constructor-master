/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#ifndef CENTIPEDE_ENTITY_INCLUDED
#define CENTIPEDE_ENTITY_INCLUDED

#include <SFML/Graphics.hpp>
#include <vector>
#include "entity.hpp"

class Centipede_Entity : public Entity
{
    public :

        Centipede_Entity(sf::Texture* texture, sf::Texture* editorMoveable, sf::Texture* nodeTex, float x, float y, float originx, float originy);

        sf::Vector2f getPosition() const;
        sf::Vector2u getSize() const;

        unsigned int getType() const;

        void roomResized(sf::Vector2i newsize);

        void save(std::vector<float>& entData);
        void load(std::vector<float>& entData);

        void update();
        void drawb(sf::RenderTarget* target) const;

    private :

        void draw(sf::RenderTarget& target, sf::RenderStates) const;

        std::vector<sf::Sprite> m_nodes;
        std::vector<sf::Vertex> m_nodesVerts;

        sf::Sprite m_nodeButton[2];

        sf::Vector2f m_position;
        sf::Vector2f m_positionb;

        unsigned int m_grabnode;
};

#endif
