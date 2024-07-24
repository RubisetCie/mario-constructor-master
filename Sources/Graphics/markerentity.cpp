/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#include <SFML/Graphics.hpp>
#include <fstream>

extern "C"
{
    #include <fmod.h>
    #ifndef LINUX
    #include <shlwapi.h>
    #include <shlobj.h>
    #else
    #include <unistd.h>
    #endif
}

#ifdef LINUX
#include <QFileDialog>
#include <QMessageBox>
#include <QDir>
#endif

#ifdef DEBUGMODE
#include <iostream>
#endif

#include "../../Headers/Graphics/markerentity.hpp"

using namespace sf;
using namespace std;

Marker_Entity::Marker_Entity(Texture* markertexture, Vector2f position, bool isLevel, unsigned int number)
{
    extern string worldDir;

    m_sprite.setTexture(*markertexture);
    m_sprite.setPosition(position);
    m_sprite.setTextureRect(IntRect(0, 0, 25, 21));

    m_levelurl[0] = '\0';

    m_islevel = false;

    if (isLevel)
    {
        extern char procPath[MAX_PATH];
        extern bool showCursor;

        mainWindow->setMouseCursorVisible(true);
#ifndef LINUX
        OPENFILENAME dialogParms;
        TCHAR personalPath[MAX_PATH];
        TCHAR filename[MAX_PATH] = "";

        SHGetFolderPath(mainWindow->getSystemHandle(), CSIDL_PERSONAL | CSIDL_FLAG_CREATE, NULL, 0, personalPath);

        strcat(personalPath, "/Mario Constructor Master/Levels");

        ZeroMemory(&dialogParms, sizeof(OPENFILENAME));

        dialogParms.lStructSize = sizeof(OPENFILENAME);
        dialogParms.hwndOwner = mainWindow->getSystemHandle();
        dialogParms.hInstance = *mainInstance;
        dialogParms.lpstrFile = filename;
        dialogParms.nFilterIndex = 1;
        dialogParms.nMaxFile = sizeof(filename);
        dialogParms.lpstrInitialDir = personalPath;
        dialogParms.lpstrTitle = "Choose a level to link :";
        dialogParms.lpstrFilter = "Constructor Master Levels (.cml)\0*.cml\0";
        dialogParms.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY | OFN_ENABLESIZING | OFN_NOCHANGEDIR;

        if (GetOpenFileName(&dialogParms))
        {
            ifstream levelFile;

            levelFile.open(filename, ios::binary);

            if (!levelFile.good())
            {
                MessageBox(NULL, "Failed to open the Level file !", "Error !", MB_OK | MB_TASKMODAL | MB_ICONERROR);
                goto LBL_EXITCONDITION;
            }

            {
                char CMLid[4];

                levelFile.read(CMLid, 4);

                if (CMLid[0] != 'C' || CMLid[1] != 'M' || CMLid[2] != 'L')
                {
                    MessageBox(NULL, "This file is not a valid CML Level !", "Error !", MB_OK | MB_TASKMODAL | MB_ICONERROR);
                    levelFile.close();
                    goto LBL_EXITCONDITION;
                }

                if (CMLid[3] != EDITOR_VERSION)
                {
                    MessageBox(NULL, "This Level was made with an Higher Version of Mario Constructor Master Editor !", "Error !", MB_OK | MB_TASKMODAL | MB_ICONERROR);
                    levelFile.close();
                    goto LBL_EXITCONDITION;
                }
            }

            {
                TCHAR filePath[MAX_PATH];
                TCHAR getString[MAX_PATH];

                strcpy(filePath, filename);

                PathRemoveFileSpec(filePath);
                SetCurrentDirectory(filePath);

                for (register unsigned int i = 0; i < 5; i++)
                {
                    ifstream checkFile;

                    levelFile.read(getString, 1);

                    if (getString[0] != '\0')
                    {
                        levelFile.seekg(-1, ios::cur);

                        for (register unsigned int j = 0; true; j++)
                        {
                            levelFile.read(&getString[j], 1);

                            if (getString[j] == '\0')
                                break;
                        }

                        checkFile.open(getString);

                        if (!checkFile.good())
                        {
                            TCHAR messageText[512];

                            sprintf(messageText, "The resource is not found :\n%s !", getString);
                            MessageBox(NULL, messageText, "Resource not found !", MB_TASKMODAL | MB_ICONERROR | MB_OK);
                            levelFile.close();
                            goto LBL_EXITCONDITION;
                        }
                        else
                            checkFile.close();
                    }
                }

                // Reset the current directory :
                SetCurrentDirectory(procPath);
            }

            levelFile.close();

            if (!worldDir.empty())
            {
                TCHAR relativePath[MAX_PATH] = "";
                TCHAR dirPath[MAX_PATH];

                strcpy(dirPath, worldDir.c_str());

                PathRemoveFileSpec(dirPath);

                if (PathRelativePathTo(relativePath, dirPath, FILE_ATTRIBUTE_DIRECTORY, filename, FILE_ATTRIBUTE_NORMAL))
                    strcpy(m_levelurl, relativePath);
                else
                {
                    MessageBox(NULL, "Failed to create Relative Path !\nYou must copy the file manually then re-link it !", "Can't create Relative Path !", MB_OK | MB_ICONERROR | MB_TASKMODAL);
                    strcpy(m_levelurl, filename);
                }
            }
            else
                strcpy(m_levelurl, filename);
#else
        const QString filename(QFileDialog::getOpenFileName(NULL, QStringLiteral("Choose a level to link :"), QDir::homePath() + QStringLiteral("/Mario Constructor Master/Levels"), QStringLiteral("Constructor Master Levels (*.cml)")));

        if (!filename.isEmpty())
        {
            ifstream levelFile;

            levelFile.open(filename.toStdString(), ios::binary);

            if (!levelFile.good())
            {
                QMessageBox messageBox(QMessageBox::Critical, QStringLiteral("Error !"), QStringLiteral("Failed to open the Level file !"), QMessageBox::Ok);
                messageBox.exec();
                goto LBL_EXITCONDITION;
            }

            {
                char CMLid[4];

                levelFile.read(CMLid, 4);

                if (CMLid[0] != 'C' || CMLid[1] != 'M' || CMLid[2] != 'L')
                {
                    QMessageBox messageBox(QMessageBox::Critical, QStringLiteral("Error !"), QStringLiteral("This file is not a valid CML Level !"), QMessageBox::Ok);
                    messageBox.exec();
                    levelFile.close();
                    goto LBL_EXITCONDITION;
                }

                if (CMLid[3] != EDITOR_VERSION)
                {
                    QMessageBox messageBox(QMessageBox::Critical, QStringLiteral("Error !"), QStringLiteral("This Level was made with an Higher Version of Mario Constructor Master Editor !"), QMessageBox::Ok);
                    messageBox.exec();
                    levelFile.close();
                    goto LBL_EXITCONDITION;
                }
            }

            {
                char getString[MAX_PATH];

                chdir(filename.left(filename.lastIndexOf(QChar('/'))).toLocal8Bit().constData());

                for (register unsigned int i = 0; i < 5; i++)
                {
                    ifstream checkFile;

                    levelFile.read(getString, 1);

                    if (getString[0] != '\0')
                    {
                        levelFile.seekg(-1, ios::cur);

                        for (register unsigned int j = 0; true; j++)
                        {
                            levelFile.read(&getString[j], 1);

                            if (getString[j] == '\0')
                                break;
                        }

                        checkFile.open(getString);

                        if (!checkFile.good())
                        {
                            QMessageBox messageBox(QMessageBox::Critical, QStringLiteral("Error !"), QString("The resource is not found :\n%1 !").arg(getString), QMessageBox::Ok);
                            messageBox.exec();
                            chdir(procPath);
                            levelFile.close();
                            goto LBL_EXITCONDITION;
                        }
                        else
                            checkFile.close();
                    }
                }

                // Reset the current directory :
                chdir(procPath);
            }

            levelFile.close();

            if (!worldDir.empty())
            {
                QDir dirPath(worldDir.c_str());
                QString relativePath(dirPath.relativeFilePath(filename));

                if (relativePath.isEmpty())
                {
                    QMessageBox messageBox(QMessageBox::Warning, QStringLiteral("Can't create Relative Path !"), QStringLiteral("Failed to create Relative Path !\nYou must copy the file manually then re-link it !"), QMessageBox::Ok);
                    messageBox.exec();
                    strcpy(m_levelurl, filename.toLocal8Bit().constData());
                }
                strcpy(m_levelurl, relativePath.toLocal8Bit().constData());
            }
            else
                strcpy(m_levelurl, filename.toLocal8Bit().constData());
#endif

            m_sprite.setTextureRect(IntRect(0, 21, 25, 21));

            m_islevel = true;

            #ifdef DEBUGMODE
            cout << m_levelurl << endl;
            #endif
        }
        mainWindow->setMouseCursorVisible(showCursor);
    }

    LBL_EXITCONDITION :

    if (number > 0)
        m_firstone = NULL;
    else
    {
        m_firstone = new Sprite(*markertexture);
        m_firstone->setPosition(position.x, position.y - 21);
        m_firstone->setTextureRect(IntRect(0, 42, 25, 19));
    }

    m_number = number;

    identifier = new char;
}

Marker_Entity::~Marker_Entity()
{
    delete identifier;

    if (m_firstone != NULL)
        delete m_firstone;
}

Vector2f Marker_Entity::getPosition() const
{
    return m_sprite.getPosition();
}

void Marker_Entity::setIsLevel(bool isLevel)
{
    extern string worldDir;

    m_sprite.setTextureRect(IntRect(0, 0, 25, 21));

    m_levelurl[0] = '\0';
    m_islevel = false;

    if (isLevel)
    {
        extern char procPath[MAX_PATH];
        extern bool showCursor;

        mainWindow->setMouseCursorVisible(true);
#ifndef LINUX
        OPENFILENAME dialogParms;
        TCHAR personalPath[MAX_PATH];
        TCHAR filename[MAX_PATH] = "";

        SHGetFolderPath(mainWindow->getSystemHandle(), CSIDL_PERSONAL | CSIDL_FLAG_CREATE, NULL, 0, personalPath);

        strcat(personalPath, "/Mario Constructor Master/Levels");

        ZeroMemory(&dialogParms, sizeof(OPENFILENAME));

        dialogParms.lStructSize = sizeof(OPENFILENAME);
        dialogParms.hwndOwner = mainWindow->getSystemHandle();
        dialogParms.hInstance = *mainInstance;
        dialogParms.lpstrFile = filename;
        dialogParms.nFilterIndex = 1;
        dialogParms.nMaxFile = sizeof(filename);
        dialogParms.lpstrInitialDir = personalPath;
        dialogParms.lpstrTitle = "Choose a level to link :";
        dialogParms.lpstrFilter = "Constructor Master Levels (.cml)\0*.cml\0";
        dialogParms.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY | OFN_ENABLESIZING | OFN_NOCHANGEDIR;

        if (GetOpenFileName(&dialogParms))
        {
            ifstream levelFile;

            levelFile.open(filename, ios::binary);

            if (!levelFile.good())
            {
                MessageBox(NULL, "Failed to open the Level file !", "Error !", MB_OK | MB_TASKMODAL | MB_ICONERROR);
                return;
            }

            {
                char CMLid[4];

                levelFile.read(CMLid, 4);

                if (CMLid[0] != 'C' || CMLid[1] != 'M' || CMLid[2] != 'L')
                {
                    MessageBox(NULL, "This file is not a valid CML Level !", "Error !", MB_OK | MB_TASKMODAL | MB_ICONERROR);
                    levelFile.close();
                    return;
                }

                if (CMLid[3] != EDITOR_VERSION)
                {
                    MessageBox(NULL, "This Level was made with an Higher Version of Mario Constructor Master Editor !", "Error !", MB_OK | MB_TASKMODAL | MB_ICONERROR);
                    levelFile.close();
                    return;
                }
            }

            {
                TCHAR filePath[MAX_PATH];
                TCHAR getString[MAX_PATH];

                strcpy(filePath, filename);

                PathRemoveFileSpec(filePath);
                SetCurrentDirectory(filePath);

                for (register unsigned int i = 0; i < 5; i++)
                {
                    ifstream checkFile;

                    levelFile.read(getString, 1);

                    if (getString[0] != '\0')
                    {
                        levelFile.seekg(-1, ios::cur);

                        for (register unsigned int j = 0; true; j++)
                        {
                            levelFile.read(&getString[j], 1);

                            if (getString[j] == '\0')
                                break;
                        }

                        checkFile.open(getString);

                        if (!checkFile.good())
                        {
                            TCHAR messageText[512];

                            sprintf(messageText, "The resource is not found :\n%s !", getString);
                            MessageBox(NULL, messageText, "Resource not found !", MB_TASKMODAL | MB_ICONERROR | MB_OK);
                            levelFile.close();
                            return;
                        }
                        else
                            checkFile.close();
                    }
                }

                // Reset the current directory :
                SetCurrentDirectory(procPath);
            }

            levelFile.close();

            if (!worldDir.empty())
            {
                TCHAR relativePath[MAX_PATH] = "";
                TCHAR dirPath[MAX_PATH];

                strcpy(dirPath, worldDir.c_str());

                PathRemoveFileSpec(dirPath);

                if (PathRelativePathTo(relativePath, dirPath, FILE_ATTRIBUTE_DIRECTORY, filename, FILE_ATTRIBUTE_NORMAL))
                    strcpy(m_levelurl, relativePath);
                else
                {
                    MessageBox(NULL, "Failed to create Relative Path !\nYou must copy the file manually then re-link it !", "Can't create Relative Path !", MB_OK | MB_ICONERROR | MB_TASKMODAL);
                    strcpy(m_levelurl, filename);
                }
            }
            else
                strcpy(m_levelurl, filename);
#else
        const QString filename(QFileDialog::getOpenFileName(NULL, QStringLiteral("Choose a level to link :"), QDir::homePath() + QStringLiteral("/Mario Constructor Master/Levels"), QStringLiteral("Constructor Master Levels (*.cml)")));

        if (!filename.isEmpty())
        {
            ifstream levelFile;

            levelFile.open(filename.toStdString(), ios::binary);

            if (!levelFile.good())
            {
                QMessageBox messageBox(QMessageBox::Critical, QStringLiteral("Error !"), QStringLiteral("Failed to open the Level file !"), QMessageBox::Ok);
                messageBox.exec();
                return;
            }

            {
                char CMLid[4];

                levelFile.read(CMLid, 4);

                if (CMLid[0] != 'C' || CMLid[1] != 'M' || CMLid[2] != 'L')
                {
                    QMessageBox messageBox(QMessageBox::Critical, QStringLiteral("Error !"), QStringLiteral("This file is not a valid CML Level !"), QMessageBox::Ok);
                    messageBox.exec();
                    levelFile.close();
                    return;
                }

                if (CMLid[3] != EDITOR_VERSION)
                {
                    QMessageBox messageBox(QMessageBox::Critical, QStringLiteral("Error !"), QStringLiteral("This Level was made with an Higher Version of Mario Constructor Master Editor !"), QMessageBox::Ok);
                    messageBox.exec();
                    levelFile.close();
                    return;
                }
            }

            {
                char getString[MAX_PATH];

                chdir(filename.left(filename.lastIndexOf(QChar('/'))).toLocal8Bit().constData());

                for (register unsigned int i = 0; i < 5; i++)
                {
                    ifstream checkFile;

                    levelFile.read(getString, 1);

                    if (getString[0] != '\0')
                    {
                        levelFile.seekg(-1, ios::cur);

                        for (register unsigned int j = 0; true; j++)
                        {
                            levelFile.read(&getString[j], 1);

                            if (getString[j] == '\0')
                                break;
                        }

                        checkFile.open(getString);

                        if (!checkFile.good())
                        {
                            QMessageBox messageBox(QMessageBox::Critical, QStringLiteral("Error !"), QString("The resource is not found :\n%1 !").arg(getString), QMessageBox::Ok);
                            messageBox.exec();
                            chdir(procPath);
                            levelFile.close();
                            return;
                        }
                        else
                            checkFile.close();
                    }
                }

                // Reset the current directory :
                chdir(procPath);
            }

            levelFile.close();

            if (!worldDir.empty())
            {
                QDir dirPath(worldDir.c_str());
                QString relativePath(dirPath.relativeFilePath(filename));

                if (relativePath.isEmpty())
                {
                    QMessageBox messageBox(QMessageBox::Warning, QStringLiteral("Can't create Relative Path !"), QStringLiteral("Failed to create Relative Path !\nYou must copy the file manually then re-link it !"), QMessageBox::Ok);
                    messageBox.exec();
                    strcpy(m_levelurl, filename.toLocal8Bit().constData());
                }
                strcpy(m_levelurl, relativePath.toLocal8Bit().constData());
            }
            else
                strcpy(m_levelurl, filename.toLocal8Bit().constData());
#endif

            m_sprite.setTextureRect(IntRect(0, 21, 25, 21));

            m_islevel = true;

            #ifdef DEBUGMODE
            cout << m_levelurl << endl;
            #endif
        }
        mainWindow->setMouseCursorVisible(showCursor);
    }
}

void Marker_Entity::setNumber()
{
    m_number--;

    #ifdef DEBUGMODE
    cout << "Number set" << endl;
    #endif

    if (m_number == 0)
    {
        #ifdef DEBUGMODE
        cout << "Zero detected" << endl;
        #endif

        const Texture* tempTex(m_sprite.getTexture());
        Vector2f tempPos(m_sprite.getPosition());

        m_firstone = new Sprite(*tempTex);
        m_firstone->setPosition(tempPos.x, tempPos.y - 21);
        m_firstone->setTextureRect(IntRect(0, 42, 25, 19));
    }
}

void Marker_Entity::setNumber(unsigned int num)
{
    m_number = num;

    if (m_number == 0)
    {
        const Texture* tempTex(m_sprite.getTexture());
        Vector2f tempPos(m_sprite.getPosition());

        m_firstone = new Sprite(*tempTex);
        m_firstone->setPosition(tempPos.x, tempPos.y - 21);
        m_firstone->setTextureRect(IntRect(0, 42, 25, 19));
    }
}

void Marker_Entity::setAlpha(sf::Uint8 alpha)
{
    m_sprite.setColor(Color(255, 255, 255, alpha));
}

void Marker_Entity::save(MarkerData& markerData)
{
    extern string worldDir;

    Vector2f position(m_sprite.getPosition());

    if (!worldDir.empty())
    {
#ifndef LINUX
        TCHAR relativePath[MAX_PATH] = "";
        TCHAR dirPath[MAX_PATH];

        strcpy(dirPath, worldDir.c_str());

        PathRemoveFileSpec(dirPath);

        if (PathRelativePathTo(relativePath, dirPath, FILE_ATTRIBUTE_DIRECTORY, m_levelurl, FILE_ATTRIBUTE_NORMAL))
            strcpy(m_levelurl, relativePath);
#else
        QDir dirPath(worldDir.c_str());
        QString relativePath(dirPath.relativeFilePath(m_levelurl));

        if (!relativePath.isEmpty())
            strcpy(m_levelurl, relativePath.toLocal8Bit().constData());
#endif
    }

    if (m_islevel)
        strcpy(markerData.levelURL, m_levelurl);
    else
        memset(markerData.levelURL, 0, MAX_PATH);

    markerData.x = position.x;
    markerData.y = position.y;

    markerData.isLevel = m_islevel;
}

void Marker_Entity::load(MarkerData& markerData)
{
    m_sprite.setPosition(markerData.x, markerData.y);

    m_islevel = markerData.isLevel;

    if (m_islevel)
    {
        strcpy(m_levelurl, markerData.levelURL);

        #ifdef DEBUGMODE
        cout << m_levelurl << endl;
        #endif // DEBUGMODE

        m_sprite.setTextureRect(IntRect(0, 21, 25, 21));
    }
}

void Marker_Entity::draw(RenderTarget& target, RenderStates) const
{
    extern int currentEditing;

    target.draw(m_sprite);

    if (m_firstone != NULL && currentEditing == 4)
        target.draw(*m_firstone);
}
