/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#ifndef ROTODISC_ENTITY_INCLUDED
#define ROTODISC_ENTITY_INCLUDED

#include <SFML/Graphics.hpp>
#include "entity.hpp"

class Rotodisc_Entity : public Entity
{
    public :

        Rotodisc_Entity(sf::Texture* texture, sf::Texture* discText, sf::Texture* editorMoveable, sf::Texture* speedRegulatorTexture, sf::Texture* sliderTexture, sf::Texture* counterTex, float x, float y, float originx, float originy, unsigned int type, bool showcircle);

        sf::Vector2f getPosition() const;
        sf::Vector2u getSize() const;

        void roomResized(sf::Vector2i newsize);

        void save(std::vector<float>& entData);
        void load(std::vector<float>& entData);

        void update();
        void drawb(sf::RenderTarget* target) const;

    private :

        void draw(sf::RenderTarget& target, sf::RenderStates) const;

        sf::Sprite m_sprites[5];

        sf::CircleShape m_shape;

        float m_distance;

        bool m_grabend;
        bool m_grabslider;

        bool m_showcircle;
};

#endif
