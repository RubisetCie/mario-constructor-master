/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#ifndef TRACE_INCLUDED
#define TRACE_INCLUDED

#include <vector>
#include <SFML/Graphics.hpp>

class Trace
{
    public :

        Trace();

        void addTrace(sf::Vector2f position, sf::IntRect textureRect, bool right);
        void updateTrace();

    protected :

        std::vector<sf::Vertex> m_tracing;
        std::vector<sf::Vertex> m_tracingb;

        bool m_current;
};

#endif
