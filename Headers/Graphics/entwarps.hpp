/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#ifndef ENTWARPS_INCLUDED
#define ENTWARPS_INCLUDED

#include <SFML/Graphics.hpp>
#include <vector>

typedef struct WarpsData
{
    float entrancex;
    float entrancey;

    char entrancesection;

    unsigned char entrancedir;

    float exitx;
    float exity;

    char exitsection;

    unsigned char exitdir;
} WarpsData;

class EntWarps : public sf::Drawable
{
    public :

        EntWarps(sf::Texture* texture, sf::Texture* moveableTex, sf::Texture* noteTex, float x, float y, unsigned char warpId, bool section, bool entrance);

        sf::Vector2f getPosition() const;
        unsigned char getDirection() const;

        bool getSection();
        bool getEntrance();

        void setId(unsigned char id);
        void setDirection(unsigned char dir);

        WarpsData save();

        void update();

        EntWarps* m_companionWarps;

    protected :

        void draw(sf::RenderTarget& target, sf::RenderStates) const;

        sf::Sprite m_sprite[4];

        bool m_entrance;
        bool m_section;

        bool m_grabdir;

        unsigned char m_wid;
        unsigned char m_direction;
};

#endif
