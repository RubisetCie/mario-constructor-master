/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#include <SFML/Graphics.hpp>
#include <cmath>
#include <vector>

#ifdef DEBUGMODE
#include <iostream>
#endif

#include "../../Headers/Graphics/centipedeentity.hpp"
#include "../../Headers/Graphics/entity.hpp"
#include "../../Headers/globals.hpp"

using namespace sf;
using namespace std;

Centipede_Entity::Centipede_Entity(Texture* texture, Texture* editorMoveable, Texture* nodeTex, float x, float y, float originx, float originy) : Entity(texture, x, y, originx, originy, 212)
{
    m_sprite.setTextureRect(IntRect(152, 0, 192, 44));

    m_nodeButton[0].setTexture(*nodeTex);
    m_nodeButton[1].setTexture(*nodeTex);

    m_nodeButton[0].setTextureRect(IntRect(0, 0, 32, 32));
    m_nodeButton[1].setTextureRect(IntRect(32, 0, 32, 32));

    m_nodeButton[0].setPosition(x + 239, y - 36);
    m_nodeButton[1].setPosition(x + 273, y - 36);

    m_nodes.emplace_back(Sprite());

    m_nodes[0].setTexture(*editorMoveable);
    m_nodes[0].setColor(Color(20, 20, 20, 255));
    m_nodes[0].setPosition(x + 256, y);

    m_nodesVerts.emplace_back(Vertex());
    m_nodesVerts.emplace_back(Vertex());

    m_nodesVerts[0].position = Vector2f(x+192, y+16);
    m_nodesVerts[0].color = Color::Red;

    m_nodesVerts[1].position = Vector2f(x+272, y+16);

    m_grabnode = 0;

    m_position = Vector2f(x, y);
    m_positionb = Vector2f(x + 288, y + 32);
}

Vector2f Centipede_Entity::getPosition() const
{
    return m_position;
}

Vector2u Centipede_Entity::getSize() const
{
    return static_cast<Vector2u>(m_positionb - m_position);
}

void Centipede_Entity::roomResized(Vector2i newsize)
{
    vector<vector<Sprite>::iterator> nodesToErase;

    bool continueChecking = true;

    for (vector<Sprite>::iterator it = m_nodes.begin(); it != m_nodes.end(); it++)
    {
        Vector2f nodePos(it->getPosition());

        if (continueChecking)
        {
            if (nodePos.x >= newsize.x * 640 || nodePos.y >= newsize.y * 480)
            {
                nodesToErase.emplace_back(it);
                continueChecking = false;
            }
        }
        else
            nodesToErase.emplace_back(it);
    }

    for (vector<vector<Sprite>::iterator>::iterator it = nodesToErase.begin(); it != nodesToErase.end(); it++)
    {
        m_nodes.erase(*it);
        m_nodesVerts.pop_back();
    }

    Vector2f lastNodePos(m_nodes.back().getPosition());

    m_nodeButton[0].setPosition(lastNodePos.x - 17, lastNodePos.y - 36);
    m_nodeButton[1].setPosition(lastNodePos.x + 17, lastNodePos.y - 36);
}

unsigned int Centipede_Entity::getType() const
{
    return 212;
}

void Centipede_Entity::save(vector<float>& entData)
{
    Vector2f position(m_sprite.getPosition());

    entData.emplace_back(position.x);
    entData.emplace_back(position.y);

    entData.emplace_back(m_nodes.size());

    for (vector<Sprite>::iterator it = m_nodes.begin(); it != m_nodes.end(); it++)
    {
        Vector2f nodePos(it->getPosition());

        entData.emplace_back(nodePos.x);
        entData.emplace_back(nodePos.y);
    }
}

void Centipede_Entity::load(vector<float>& entData)
{
    extern Texture* editorMoveable;

    Vector2f lastNodePos;

    m_nodes.back().setPosition(entData[3], entData[4]);
    m_nodesVerts.back().position = Vector2f(entData[3] + 16, entData[4] + 16);

    for (register unsigned int i = 1; i < entData[2]; i++)
    {
        Sprite* lastNode;
        Vertex* lastVertex;

        Vector2f nodePos(entData[3 + (i * 2)], entData[4 + (i * 2)]);

        m_nodes.emplace_back(Sprite());
        m_nodesVerts.emplace_back(Vertex());

        lastNode = &m_nodes.back();
        lastVertex = &m_nodesVerts.back();

        lastNode->setTexture(*editorMoveable);
        lastNode->setColor(Color(20, 20, 20, 255));

        lastNode->setPosition(nodePos);

        lastVertex->position = Vector2f(nodePos.x+16, nodePos.y+16);
        lastVertex->color = Color::Red;

        lastNodePos = lastNode->getPosition();
    }

    m_nodeButton[0].setPosition(lastNodePos.x - 17, lastNodePos.y - 36);
    m_nodeButton[1].setPosition(lastNodePos.x + 17, lastNodePos.y - 36);

    m_positionb = Vector2f(lastNodePos.x + 32, lastNodePos.y + 32);
}

void Centipede_Entity::update()
{
    extern int currentSelection;
    extern bool sectionb;

    extern Texture* editorMoveable;
    extern FMOD_SOUND* buttonClick;

    Sprite* currentNode = &m_nodes.back();

    Vector2f moveablePos(currentNode->getPosition());
    Vector2f nodeButtonPos(m_nodeButton[0].getPosition());

    m_nodeButton[0].setColor(Color::White);
    m_nodeButton[1].setColor(Color::White);

    currentNode->setColor(Color(20, 20, 20, 255));

    if (currentSelection == 0)
    {
        extern bool toolbarVisible;

        if (toolbarVisible)
        {
            if (!(mpos_absolute.y < 150 || (mpos_absolute.x < 133 && mpos_absolute.y > 448) || (mpos_absolute.x > 459 && mpos_absolute.y > 448)))
            {
                if (FloatRect(moveablePos.x, moveablePos.y, 32, 32).contains(mpos_relative))
                {
                    currentNode->setColor(Color(255, 255, 128, 255));

                    if (mousePressed == 1)
                        m_grabnode = m_nodes.size();
                }

                if (FloatRect(nodeButtonPos.x, nodeButtonPos.y, 32, 32).contains(mpos_relative))
                {
                    m_nodeButton[0].setColor(Color::Yellow);

                    if (mousePressed == 1)
                    {
                        if (m_nodes.size() > 1)
                        {
                            m_nodes.pop_back();
                            m_nodesVerts.pop_back();

                            Vector2f lastNodePos(m_nodes.back().getPosition());

                            m_nodeButton[0].setPosition(lastNodePos.x - 17, lastNodePos.y - 36);
                            m_nodeButton[1].setPosition(lastNodePos.x + 17, lastNodePos.y - 36);
                        }

                        FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(0), buttonClick, 0, NULL);
                    }
                }

                if (FloatRect(nodeButtonPos.x + 34, nodeButtonPos.y, 32, 32).contains(mpos_relative))
                {
                    m_nodeButton[1].setColor(Color::Yellow);

                    if (mousePressed == 1)
                    {
                        if (m_nodes.size() < 64)
                        {
                            Sprite* lastNode = &m_nodes.back();
                            Vector2f lastNodePos(lastNode->getPosition());

                            m_nodes.emplace_back(Sprite());
                            m_nodesVerts.emplace_back(Vertex());

                            lastNode = &m_nodes.back();

                            lastNode->setTexture(*editorMoveable);
                            lastNode->setColor(Color(20, 20, 20, 255));

                            if (m_nodes.size() % 2 == 0)
                            {
                                Vertex* lastVertex = &m_nodesVerts.back();

                                lastNode->setPosition(lastNodePos.x, lastNodePos.y-32);

                                lastVertex->position = Vector2f(lastNodePos.x+16, lastNodePos.y-16);
                                lastVertex->color = Color::Red;

                                m_nodeButton[0].move(0, -32);
                                m_nodeButton[1].move(0, -32);
                            }
                            else
                            {
                                Vertex* lastVertex = &m_nodesVerts.back();

                                lastNode->setPosition(lastNodePos.x+32, lastNodePos.y);

                                lastVertex->position = Vector2f(lastNodePos.x+48, lastNodePos.y+16);
                                lastVertex->color = Color::Red;

                                m_nodeButton[0].move(32, 0);
                                m_nodeButton[1].move(32, 0);
                            }
                        }

                        FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(0), buttonClick, 0, NULL);
                    }
                }
            }
        }
        else
        {
            if (FloatRect(moveablePos.x, moveablePos.y, 32, 32).contains(mpos_relative))
            {
                currentNode->setColor(Color(255, 255, 128, 255));

                if (mousePressed == 1)
                    m_grabnode = m_nodes.size();
            }

            if (FloatRect(nodeButtonPos.x, nodeButtonPos.y, 32, 32).contains(mpos_relative))
            {
                m_nodeButton[0].setColor(Color::Yellow);

                if (mousePressed == 1)
                {
                    if (m_nodes.size() > 1)
                    {
                        m_nodes.pop_back();
                        m_nodesVerts.pop_back();

                        Vector2f lastNodePos(m_nodes.back().getPosition());

                        m_nodeButton[0].setPosition(lastNodePos.x - 17, lastNodePos.y - 36);
                        m_nodeButton[1].setPosition(lastNodePos.x + 17, lastNodePos.y - 36);
                    }

                    FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(0), buttonClick, 0, NULL);
                }
            }

            if (FloatRect(nodeButtonPos.x + 34, nodeButtonPos.y, 32, 32).contains(mpos_relative))
            {
                m_nodeButton[1].setColor(Color::Yellow);

                if (mousePressed == 1)
                {
                    if (m_nodes.size() < 64)
                    {
                        Sprite* lastNode = &m_nodes.back();
                        Vector2f lastNodePos(lastNode->getPosition());

                        m_nodes.emplace_back(Sprite());
                        m_nodesVerts.emplace_back(Vertex());

                        lastNode = &m_nodes.back();

                        lastNode->setTexture(*editorMoveable);
                        lastNode->setColor(Color(20, 20, 20, 255));

                        if (m_nodes.size() % 2 == 0)
                        {
                            Vertex* lastVertex = &m_nodesVerts.back();

                            lastNode->setPosition(lastNodePos.x, lastNodePos.y-32);

                            lastVertex->position = Vector2f(lastNodePos.x+16, lastNodePos.y-16);
                            lastVertex->color = Color::Red;

                            m_nodeButton[0].move(0, -32);
                            m_nodeButton[1].move(0, -32);
                        }
                        else
                        {
                            Vertex* lastVertex = &m_nodesVerts.back();

                            lastNode->setPosition(lastNodePos.x+32, lastNodePos.y);

                            lastVertex->position = Vector2f(lastNodePos.x+48, lastNodePos.y+16);
                            lastVertex->color = Color::Red;

                            m_nodeButton[0].move(32, 0);
                            m_nodeButton[1].move(32, 0);
                        }
                    }

                    FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(0), buttonClick, 0, NULL);
                }
            }
        }

        if (mousePressed == 0)
            m_grabnode = 0;
    }
    else
    {
        extern bool toolbarVisible;

        if (toolbarVisible)
        {
            if (!(mpos_absolute.y < 150 || (mpos_absolute.x < 133 && mpos_absolute.y > 448) || (mpos_absolute.x > 459 && mpos_absolute.y > 448)))
            {
                if (FloatRect(moveablePos.x, moveablePos.y, 32, 32).contains(mpos_relative))
                {
                    currentNode->setColor(Color(255, 255, 128, 255));

                    if (middlPressed == 1)
                        m_grabnode = m_nodes.size();
                }

                if (FloatRect(nodeButtonPos.x, nodeButtonPos.y, 32, 32).contains(mpos_relative))
                {
                    m_nodeButton[0].setColor(Color::Yellow);

                    if (middlPressed == 1)
                    {
                        if (m_nodes.size() > 1)
                        {
                            m_nodes.pop_back();
                            m_nodesVerts.pop_back();

                            Vector2f lastNodePos(m_nodes.back().getPosition());

                            m_nodeButton[0].setPosition(lastNodePos.x - 17, lastNodePos.y - 36);
                            m_nodeButton[1].setPosition(lastNodePos.x + 17, lastNodePos.y - 36);
                        }

                        FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(0), buttonClick, 0, NULL);
                    }
                }

                if (FloatRect(nodeButtonPos.x + 34, nodeButtonPos.y, 32, 32).contains(mpos_relative))
                {
                    m_nodeButton[1].setColor(Color::Yellow);

                    if (middlPressed == 1)
                    {
                        if (m_nodes.size() < 64)
                        {
                            Sprite* lastNode = &m_nodes.back();
                            Vector2f lastNodePos(lastNode->getPosition());

                            m_nodes.emplace_back(Sprite());
                            m_nodesVerts.emplace_back(Vertex());

                            lastNode = &m_nodes.back();

                            lastNode->setTexture(*editorMoveable);
                            lastNode->setColor(Color(20, 20, 20, 255));

                            if (m_nodes.size() % 2 == 0)
                            {
                                Vertex* lastVertex = &m_nodesVerts.back();

                                lastNode->setPosition(lastNodePos.x, lastNodePos.y-32);

                                lastVertex->position = Vector2f(lastNodePos.x+16, lastNodePos.y-16);
                                lastVertex->color = Color::Red;

                                m_nodeButton[0].move(0, -32);
                                m_nodeButton[1].move(0, -32);
                            }
                            else
                            {
                                Vertex* lastVertex = &m_nodesVerts.back();

                                lastNode->setPosition(lastNodePos.x+32, lastNodePos.y);

                                lastVertex->position = Vector2f(lastNodePos.x+48, lastNodePos.y+16);
                                lastVertex->color = Color::Red;

                                m_nodeButton[0].move(32, 0);
                                m_nodeButton[1].move(32, 0);
                            }
                        }

                        FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(0), buttonClick, 0, NULL);
                    }
                }
            }
            else
                m_grabnode = 0;
        }
        else
        {
            if (FloatRect(moveablePos.x, moveablePos.y, 32, 32).contains(mpos_relative))
            {
                currentNode->setColor(Color(255, 255, 128, 255));

                if (middlPressed == 1)
                    m_grabnode = m_nodes.size();
            }

            if (FloatRect(nodeButtonPos.x, nodeButtonPos.y, 32, 32).contains(mpos_relative))
            {
                m_nodeButton[0].setColor(Color::Yellow);

                if (middlPressed == 1)
                {
                    if (m_nodes.size() > 1)
                    {
                        m_nodes.pop_back();
                        m_nodesVerts.pop_back();

                        Vector2f lastNodePos(m_nodes.back().getPosition());

                        m_nodeButton[0].setPosition(lastNodePos.x - 17, lastNodePos.y - 36);
                        m_nodeButton[1].setPosition(lastNodePos.x + 17, lastNodePos.y - 36);
                    }

                    FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(0), buttonClick, 0, NULL);
                }
            }

            if (FloatRect(nodeButtonPos.x + 34, nodeButtonPos.y, 32, 32).contains(mpos_relative))
            {
                m_nodeButton[1].setColor(Color::Yellow);

                if (middlPressed == 1)
                {
                    if (m_nodes.size() < 64)
                    {
                        Sprite* lastNode = &m_nodes.back();
                        Vector2f lastNodePos(lastNode->getPosition());

                        m_nodes.emplace_back(Sprite());
                        m_nodesVerts.emplace_back(Vertex());

                        lastNode = &m_nodes.back();

                        lastNode->setTexture(*editorMoveable);
                        lastNode->setColor(Color(20, 20, 20, 255));

                        if (m_nodes.size() % 2 == 0)
                        {
                            Vertex* lastVertex = &m_nodesVerts.back();

                            lastNode->setPosition(lastNodePos.x, lastNodePos.y-32);

                            lastVertex->position = Vector2f(lastNodePos.x+16, lastNodePos.y-16);
                            lastVertex->color = Color::Red;

                            m_nodeButton[0].move(0, -32);
                            m_nodeButton[1].move(0, -32);
                        }
                        else
                        {
                            Vertex* lastVertex = &m_nodesVerts.back();

                            lastNode->setPosition(lastNodePos.x+32, lastNodePos.y);

                            lastVertex->position = Vector2f(lastNodePos.x+48, lastNodePos.y+16);
                            lastVertex->color = Color::Red;

                            m_nodeButton[0].move(32, 0);
                            m_nodeButton[1].move(32, 0);
                        }
                    }

                    FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(0), buttonClick, 0, NULL);
                }
            }
        }

        if (middlPressed == 0)
            m_grabnode = 0;
    }

    if (m_grabnode != 0)
    {
        extern Vector2u roomScaleb;
        extern Vector2u roomScale;

        Vertex* currentVertex = &m_nodesVerts.back();

        if (m_grabnode == 1)
        {
            Vector2f originalPos = getOriginalPosition();

            float cursorPos = mpos_relative.x - 16;

            if (Keyboard::isKeyPressed(Keyboard::LShift))
                cursorPos = roundf(cursorPos / 16) * 16;

            if (cursorPos < originalPos.x + 256)
                cursorPos = originalPos.x + 256;

            if (sectionb)
            {
                if (cursorPos > (roomScaleb.x * 640) - 32)
                    cursorPos = (roomScaleb.x * 640) - 32;
            }
            else
            {
                if (cursorPos > (roomScale.x * 640) - 32)
                    cursorPos = (roomScale.x * 640) - 32;
            }

            currentNode->setColor(Color::White);
            currentNode->setPosition(cursorPos, originalPos.y);

            currentVertex->position = Vector2f(cursorPos+16, originalPos.y+16);
        }
        else if (m_grabnode % 2 == 0)
        {
            Vector2f originalPos(m_nodes[m_grabnode-2].getPosition());

            float cursorPos = mpos_relative.y - 16;

            if (Keyboard::isKeyPressed(Keyboard::LShift))
                cursorPos = roundf(cursorPos / 16) * 16;

            if (cursorPos < 0)
                cursorPos = 0;

            if (cursorPos > originalPos.y - 32 && cursorPos < originalPos.y + 32)
            {
                if (cursorPos > originalPos.y)
                    cursorPos = originalPos.y + 32;
                else
                    cursorPos = originalPos.y - 32;
            }

            if (sectionb)
            {
                if (cursorPos > (roomScaleb.y * 480) - 32)
                    cursorPos = (roomScaleb.y * 480) - 32;
            }
            else
            {
                if (cursorPos > (roomScale.y * 480) - 32)
                    cursorPos = (roomScale.y * 480) - 32;
            }

            currentNode->setColor(Color::White);
            currentNode->setPosition(originalPos.x, cursorPos);

            currentVertex->position = Vector2f(originalPos.x+16, cursorPos+16);
        }
        else
        {
            Vector2f originalPos(m_nodes[m_grabnode-2].getPosition());

            float cursorPos = mpos_relative.x - 16;

            if (Keyboard::isKeyPressed(Keyboard::LShift))
                cursorPos = roundf(cursorPos / 16) * 16;

            if (cursorPos < 0)
                cursorPos = 0;

            if (cursorPos > originalPos.x - 32 && cursorPos < originalPos.x + 32)
            {
                if (cursorPos > originalPos.x)
                    cursorPos = originalPos.x + 32;
                else
                    cursorPos = originalPos.x - 32;
            }

            if (sectionb)
            {
                if (cursorPos > (roomScaleb.x * 640) - 32)
                    cursorPos = (roomScaleb.x * 640) - 32;
            }
            else
            {
                if (cursorPos > (roomScale.x * 640) - 32)
                    cursorPos = (roomScale.x * 640) - 32;
            }

            currentNode->setColor(Color::White);
            currentNode->setPosition(cursorPos, originalPos.y);

            currentVertex->position = Vector2f(cursorPos+16, originalPos.y+16);
        }

        Vector2f lastNodePos(m_nodes.back().getPosition());

        m_nodeButton[0].setPosition(lastNodePos.x - 17, lastNodePos.y - 36);
        m_nodeButton[1].setPosition(lastNodePos.x + 17, lastNodePos.y - 36);

        m_position = getOriginalPosition();
        m_positionb = getOriginalPosition() + Vector2f(288, 32);

        for (vector<Sprite>::iterator it = m_nodes.begin(); it != m_nodes.end(); it++)
        {
            Vector2f nodePos(it->getPosition());

            if (nodePos.x < m_position.x)
                m_position.x = nodePos.x;

            if (nodePos.y < m_position.y)
                m_position.y = nodePos.y;

            if (nodePos.x > m_positionb.x)
                m_positionb.x = nodePos.x + 32;

            if (nodePos.y > m_positionb.y)
                m_positionb.y = nodePos.y + 32;
        }
    }
}

void Centipede_Entity::drawb(RenderTarget* target) const
{
    target->draw(m_nodeButton[0]);
    target->draw(m_nodeButton[1]);

    for (register unsigned int i = 0; i < m_nodes.size(); i++)
        target->draw(m_nodes[i]);
}

void Centipede_Entity::draw(RenderTarget& target, RenderStates) const
{
    target.draw(m_sprite);
    target.draw(&m_nodesVerts.front(), m_nodesVerts.size(), LinesStrip);
}
