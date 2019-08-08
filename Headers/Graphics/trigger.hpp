/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#ifndef TRIGGER_INCLUDED
#define TRIGGER_INCLUDED

#include <SFML/Graphics.hpp>

typedef struct TriggerData
{
    float x;
    float y;

    float width;
    float height;

    float targetHeight;
} TriggerData;

class Trigger : public sf::Drawable
{
    public :

        Trigger(sf::Vector2f position, sf::Vector2f size, unsigned char number, sf::Texture* textTexture, sf::Texture* moveableTex, sf::Texture* iconTex, float height = -1);

        void update();
        void safeUpdate();

        void setMoveablePosition(float pos);

        void decreaseNumb();

        sf::Vector2f getPosition() const;
        sf::Vector2u getSize();

        float getMoveablePosition();

        sf::Vector2f getFullPosition();
        sf::Vector2f getFullSize();

        TriggerData save();

    private :

        sf::Sprite m_text[3];
        sf::Sprite m_height[2];

        sf::RectangleShape m_rectangle;

        bool m_grabheight;

        void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};

#endif
