/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#ifndef LEVELEND_ENTITY_INCLUDED
#define LEVELEND_ENTITY_INCLUDED

#include <SFML/Graphics.hpp>
#include "entity.hpp"

class LevelEnd_Entity : public Entity
{
    public :

        LevelEnd_Entity(sf::Texture* texture, sf::Texture* textureb, sf::Texture* moveableTexture, float x, float y);

        sf::Vector2f getPosition() const;
        sf::Vector2u getSize() const;

        unsigned int getType() const;

        void save(std::vector<float>& entData);
        void load(std::vector<float>& entData);

        void update();
        void drawb(sf::RenderTarget* target) const;

    private :

        void draw(sf::RenderTarget& target, sf::RenderStates) const;

        sf::Sprite m_spriteb;
        sf::Sprite m_moveable;

        bool m_grab;
};

#endif
