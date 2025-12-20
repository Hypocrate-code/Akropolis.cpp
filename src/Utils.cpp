#include "Utils.hpp"
#include "Tuile.hpp"
#include <QDebug>
#include <QVector2D>

#include <iostream>

namespace Utils
{
    int indiceDeDroite(int direction) {


        if (direction < 0 || direction > 5) return -1;
        return (direction == 5 ? 0 : direction + 1);
    }
    int indiceDeGauche(int direction) {
        if (direction < 0 || direction > 5) return -1;
        return (direction == 0 ? 5 : direction - 1);
    }

    char get_color_char(Couleur color)
    {
        switch (color)
        {
        case Couleur::Rouge:
            return 'R';
        case Couleur::Vert:
            return 'V';
        case Couleur::Bleu:
            return 'B';
        case Couleur::Jaune:
            return 'J';
        case Couleur::Violet:
            return 'P';
        case Couleur::nulle:
            return 'N';
        default:
            return '?';
        }
    }

    std::string get_color_code(Couleur color)
    {
        switch (color)
        {
        case Couleur::Rouge:
            return "\033[31m"; // Red
        case Couleur::Vert:
            return "\033[32m"; // Green
        case Couleur::Bleu:
            return "\033[34m"; // Blue
        case Couleur::Jaune:
            return "\033[33m"; // Yellow
        case Couleur::Violet:
            return "\033[35m"; // Magenta
        case Couleur::nulle:
            return "\033[37m"; // White
        default:
            return "\033[37m"; // White
        }
    }

    std::string get_short_type(Type type)
    {
        switch (type)
        {
        case Type::Carriere:
            return "CAR";
        case Type::Quartier:
            return "QUA";
        case Type::Place:
            return "PLA";
        case Type::Fantome:
            return "   ";
        default:
            return "UNK";
        }
    }

    std::string get_short_color(Couleur color)
    {
        switch (color)
        {
        case Couleur::Rouge:
            return "ROU";
        case Couleur::Vert:
            return "VER";
        case Couleur::Bleu:
            return "BLE";
        case Couleur::Jaune:
            return "JAU";
        case Couleur::Violet:
            return "VIO";
        case Couleur::nulle:
            return "   ";
        default:
            return "UNK";
        }
    }

    std::string type_to_string(Type type)
    {
        switch (type)
        {
        case Type::Carriere:
            return "Carriere";
        case Type::Quartier:
            return "Quartier";
        case Type::Place:
            return "Place";
        case Type::Fantome:
            return "Fantome";
        default:
            return "Unknown";
        }
    }

    std::string color_to_string(Couleur color)
    {
        switch (color)
        {
        case Couleur::Rouge:
            return "Rouge";
        case Couleur::Vert:
            return "Vert";
        case Couleur::Bleu:
            return "Bleu";
        case Couleur::Jaune:
            return "Jaune";
        case Couleur::Violet:
            return "Violet";
        case Couleur::nulle:
            return "nulle";
        default:
            return "Unknown";
        }
    }

    std::string center_string(const std::string &str, int width)
    {
        std::string cleanStr = str;
        int colorCodeCount = 0;
        size_t pos = 0;

        // Compter les codes couleur
        while ((pos = cleanStr.find("\033[", pos)) != std::string::npos)
        {
            size_t end = cleanStr.find('m', pos);
            if (end != std::string::npos)
            {
                colorCodeCount += (end - pos + 1);
                cleanStr.erase(pos, end - pos + 1);
            }
        }

        int visibleLength = cleanStr.length();
        int totalLength = visibleLength + colorCodeCount;

        if (visibleLength >= width)
        {
            return str.substr(0, width + colorCodeCount);
        }

        int padding = width - visibleLength;
        int leftPadding = padding / 2;
        int rightPadding = padding - leftPadding;

        return std::string(leftPadding, ' ') + str + std::string(rightPadding, ' ');
    }

    std::string colorize_line(const std::string &line)
    {
        std::string result = line;

        // Remplacements simples
        result = replace_all(result, "ROU", "\033[31mROU\033[0m");
        result = replace_all(result, "VER", "\033[32mVER\033[0m");
        result = replace_all(result, "BLE", "\033[34mBLE\033[0m");
        result = replace_all(result, "JAU", "\033[33mJAU\033[0m");
        result = replace_all(result, "VIO", "\033[35mVIO\033[0m");
        result = replace_all(result, "NUL", "\033[37mNUL\033[0m");

        return result;
    }
    std::string colorize_string(const std::string& str, const Couleur color)
    {
        std::string result = get_color_code(color) + str + get_color_code(color);
        return result;
    }

    std::string replace_all(std::string str, const std::string &from, const std::string &to)
    {
        size_t pos = 0;
        while ((pos = str.find(from, pos)) != std::string::npos)
        {
            str.replace(pos, from.length(), to);
            pos += to.length();
        }
        return str;
    }

    QPoint getCentreVoisin(QPoint originalCenter, int direction, int radius)
    {
        int height = 2 * cos(M_PI / 6) * radius;
        int width = 2 * radius;
        int delta = 1;
        std::array<QVector2D, 8> vects {
            QVector2D(0, height - delta), 
            QVector2D(-1.5 * radius + delta, (height / 2) - delta), 
            QVector2D(-1.5 * radius + delta, -1 * (height / 2) + delta), 
            QVector2D(0, -1 * height + delta),  
            QVector2D(1.5 * radius - delta, -1 * (height / 2) + delta), 
            QVector2D(1.5 * radius - delta, height / 2 - delta), 
            QVector2D(0,0),
            QVector2D(0,0)
        };
        return originalCenter + vects[direction].toPoint();
    };


    
    int radiusTexture = 231;
    float heightHexTexture = cos(M_PI/6) * 2 * radiusTexture;
    int widthHexTexture = 2 * radiusTexture;
    QPixmap get_texture(const Hexagone* hex) {
        QPixmap baseTexture("../assets.png");
        Type type = hex->getType();
        Couleur couleur = hex->getCouleur();
        QPixmap finalTexture;
        switch (type)
        {
            case Type::Quartier:
                switch (couleur)
                {
                    case Couleur::Rouge:
                        return baseTexture.copy(widthHexTexture - (widthHexTexture - radiusTexture)/2, heightHexTexture/2, widthHexTexture, heightHexTexture);
                    case Couleur::Bleu:
                        return baseTexture.copy(widthHexTexture + radiusTexture, 0, widthHexTexture, heightHexTexture);
                    case Couleur::Jaune:
                        return baseTexture.copy(widthHexTexture + radiusTexture, heightHexTexture, widthHexTexture, heightHexTexture);
                    case Couleur::Violet:
                        return baseTexture.copy(0, heightHexTexture*2, widthHexTexture, heightHexTexture);
                    case Couleur::Vert:
                        return baseTexture.copy(0, heightHexTexture, widthHexTexture, heightHexTexture);
                    default:
                        return baseTexture;
                }
                break;
            case Type::Place:
                switch (couleur)
                {
                    case Couleur::Rouge:
                        return baseTexture.copy(0, 0, widthHexTexture, heightHexTexture);
                    case Couleur::Bleu:
                        return baseTexture.copy(widthHexTexture + (radiusTexture * 5/2), heightHexTexture/2, widthHexTexture, heightHexTexture);
                    case Couleur::Jaune:
                        return baseTexture.copy(widthHexTexture  + (radiusTexture * 5/2), heightHexTexture*3/2, widthHexTexture, heightHexTexture);
                    case Couleur::Violet:
                        return baseTexture.copy(radiusTexture * 3/2, heightHexTexture * 5 / 2, widthHexTexture, heightHexTexture);
                    case Couleur::Vert:
                        return baseTexture.copy(radiusTexture * 3/2, heightHexTexture * 3/2, widthHexTexture, heightHexTexture);
                    default:
                        return baseTexture.copy(widthHexTexture + radiusTexture, heightHexTexture*4, widthHexTexture, heightHexTexture);
                }
            case Type::Fantome:
                return baseTexture.copy(widthHexTexture + radiusTexture, heightHexTexture*4, widthHexTexture, heightHexTexture);
            case Type::Carriere:
                return baseTexture.copy(widthHexTexture + radiusTexture, heightHexTexture*2, widthHexTexture, heightHexTexture);
            default:
                return baseTexture.copy(widthHexTexture + radiusTexture, heightHexTexture*4, widthHexTexture, heightHexTexture);
        }
    }


    int opposite_index(int idx) {
        switch (idx)
        {
        case 0: return 3; // NE <-> SO
        case 3: return 0;
        case 1: return 4; // S  <-> N
        case 4: return 1;
        case 2: return 5; // SE <-> NO
        case 5: return 2;
        case 6: return 7; // TOP <-> BOT
        case 7: return 6;
        default: return idx;
        }
    }
}
