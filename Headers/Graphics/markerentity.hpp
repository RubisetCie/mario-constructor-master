/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#ifndef MARKER_ENTITY_INCLUDED
#define MARKER_ENTITY_INCLUDED

#include <SFML/Graphics.hpp>
#include <string>

#include "../globals.hpp"

typedef struct MarkerData
{
    char levelURL[MAX_PATH];

    float x;
    float y;

    bool isLevel;
} MarkerData;

class Marker_Entity : public sf::Drawable
{
    public :

        Marker_Entity(sf::Texture* marktexture, sf::Vector2f position = sf::Vector2f(0, 0), bool isLevel = false, unsigned int number = 0);
        virtual ~Marker_Entity();

        sf::Vector2f getPosition() const;

        void setIsLevel(bool isLevel);
        void setNumber();
        void setNumber(unsigned int num);
        void setAlpha(sf::Uint8 alpha);

        void save(MarkerData& markerData);
        void load(MarkerData& markerData);

        char m_levelurl[MAX_PATH];

        bool m_islevel;

        char* identifier;

    private :

        void draw(sf::RenderTarget& target, sf::RenderStates) const;

        sf::Sprite m_sprite;
        sf::Sprite* m_firstone;

        unsigned int m_number;
};

#endif
