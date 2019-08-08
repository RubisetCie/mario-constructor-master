/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#include <SFML/Graphics.hpp>
#include <iostream>
#include "../../Headers/Graphics/trace.hpp"

using namespace sf;
using namespace std;

Trace::Trace()
{
    m_current = false;
}

void Trace::addTrace(Vector2f position, IntRect textureRect, bool right)
{
    if (m_current)
    {
        if (!m_tracing.empty())
        {
            if (m_tracing.back().color.a <= 0)
                m_tracing.clear();
        }

        if (right)
        {
            m_tracingb.emplace_back(Vertex(position, Color::White, Vector2f(textureRect.left, textureRect.top)));
            m_tracingb.emplace_back(Vertex(Vector2f(position.x + textureRect.width, position.y), Color::White, Vector2f(textureRect.left + textureRect.width, textureRect.top)));
            m_tracingb.emplace_back(Vertex(Vector2f(position.x + textureRect.width, position.y + textureRect.height), Color::White, Vector2f(textureRect.left + textureRect.width, textureRect.top + textureRect.height)));
            m_tracingb.emplace_back(Vertex(Vector2f(position.x, position.y + textureRect.height), Color::White, Vector2f(textureRect.left, textureRect.top + textureRect.height)));
        }
        else
        {
            m_tracingb.emplace_back(Vertex(position, Color::White, Vector2f(textureRect.left + textureRect.width, textureRect.top)));
            m_tracingb.emplace_back(Vertex(Vector2f(position.x + textureRect.width, position.y), Color::White, Vector2f(textureRect.left, textureRect.top)));
            m_tracingb.emplace_back(Vertex(Vector2f(position.x + textureRect.width, position.y + textureRect.height), Color::White, Vector2f(textureRect.left, textureRect.top + textureRect.height)));
            m_tracingb.emplace_back(Vertex(Vector2f(position.x, position.y + textureRect.height), Color::White, Vector2f(textureRect.left + textureRect.width, textureRect.top + textureRect.height)));
        }

        if (m_tracingb.size() >= 48)
            m_current = false;
    }
    else
    {
        if (!m_tracingb.empty())
        {
            if (m_tracingb.back().color.a <= 0)
                m_tracingb.clear();
        }

        if (right)
        {
            m_tracing.emplace_back(Vertex(position, Color::White, Vector2f(textureRect.left, textureRect.top)));
            m_tracing.emplace_back(Vertex(Vector2f(position.x + textureRect.width, position.y), Color::White, Vector2f(textureRect.left + textureRect.width, textureRect.top)));
            m_tracing.emplace_back(Vertex(Vector2f(position.x + textureRect.width, position.y + textureRect.height), Color::White, Vector2f(textureRect.left + textureRect.width, textureRect.top + textureRect.height)));
            m_tracing.emplace_back(Vertex(Vector2f(position.x, position.y + textureRect.height), Color::White, Vector2f(textureRect.left, textureRect.top + textureRect.height)));
        }
        else
        {
            m_tracing.emplace_back(Vertex(position, Color::White, Vector2f(textureRect.left + textureRect.width, textureRect.top)));
            m_tracing.emplace_back(Vertex(Vector2f(position.x + textureRect.width, position.y), Color::White, Vector2f(textureRect.left, textureRect.top)));
            m_tracing.emplace_back(Vertex(Vector2f(position.x + textureRect.width, position.y + textureRect.height), Color::White, Vector2f(textureRect.left, textureRect.top + textureRect.height)));
            m_tracing.emplace_back(Vertex(Vector2f(position.x, position.y + textureRect.height), Color::White, Vector2f(textureRect.left + textureRect.width, textureRect.top + textureRect.height)));
        }

        if (m_tracing.size() >= 48)
            m_current = true;
    }
}

void Trace::updateTrace()
{
    size_t s = m_tracing.size();
    size_t sb = m_tracingb.size();

    for (register unsigned int i = 0; i < s; i += 4)
    {
        if (m_tracing[i].color.a > 8)
        {
            m_tracing[i].color.a -= 8;
            m_tracing[i+1].color.a -= 8;
            m_tracing[i+2].color.a -= 8;
            m_tracing[i+3].color.a -= 8;
        }
        else
        {
            m_tracing[i].color.a = 0;
            m_tracing[i+1].color.a = 0;
            m_tracing[i+2].color.a = 0;
            m_tracing[i+3].color.a = 0;
        }
    }

    for (register unsigned int i = 0; i < sb; i += 4)
    {
        if (m_tracingb[i].color.a > 8)
        {
            m_tracingb[i].color.a -= 8;
            m_tracingb[i+1].color.a -= 8;
            m_tracingb[i+2].color.a -= 8;
            m_tracingb[i+3].color.a -= 8;
        }
        else
        {
            m_tracingb[i].color.a = 0;
            m_tracingb[i+1].color.a = 0;
            m_tracingb[i+2].color.a = 0;
            m_tracingb[i+3].color.a = 0;
        }
    }
}
