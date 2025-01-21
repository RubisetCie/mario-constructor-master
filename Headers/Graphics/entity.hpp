/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#ifndef ENTITY_INCLUDED
#define ENTITY_INCLUDED

#include <SFML/Graphics.hpp>
#include <vector>

class Entity : public sf::Drawable
{
    public :

        Entity(sf::Texture* texture, float x, float y, float originx, float originy, unsigned int type);

        virtual sf::Vector2f getPosition() const;
        sf::Vector2f getOriginalPosition() const;

        virtual sf::Vector2u getSize() const;

        virtual unsigned int getType() const;

        virtual void roomResized(sf::Vector2u newsize);

        virtual void save(std::vector<float>& entData);
        virtual void load(std::vector<float>& entData);

        virtual void update();
        virtual void drawb(sf::RenderTarget* target) const;

    protected :

        void draw(sf::RenderTarget& target, sf::RenderStates) const;

        sf::Sprite m_sprite;

        unsigned int m_enttype;
};

#endif
