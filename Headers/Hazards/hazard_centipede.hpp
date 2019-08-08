/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#ifndef HAZARDCENTIPEDE_INCLUDED
#define HAZARDCENTIPEDE_INCLUDED

#include <SFML/Graphics.hpp>
#include <list>
#include <vector>
#include "../Core/collider.hpp"
#include "../Core/placeable.hpp"
#include "../globals.hpp"

class Hazard_Centipede : public Placeable
{
    public :

        Hazard_Centipede(sf::Vector2f* nodes, short size, std::list<Collider*>::iterator collision);
        ~Hazard_Centipede();

        void setPosition(sf::Vector2f pos);

        void update();
        void secureUpdate();
        void afterUpdate();

        sf::Vector2f getPosition() const;
        sf::Vector2f getSize() const;

        ID getID() const;

    private :

        void draw(sf::RenderTarget& target, sf::RenderStates) const;

        std::vector<sf::Vector2f> m_nodes;

        sf::VertexArray m_array;

        Collider* m_collisions[6];

        sf::Vector2f m_resolver[2];

        FMOD_CHANNEL* m_channel;

        short m_currentnode[6], m_currenttimer[6], m_maxnodes;

        char m_start;
};

#endif
