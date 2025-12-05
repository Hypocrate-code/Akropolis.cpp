#pragma once
#include <string>
// #include "Tuile.hpp"


enum class Couleur;
enum class Type;
namespace Utils
{
    char get_color_char(Couleur color);

    std::string get_color_code(Couleur color);
    std::string get_short_type(Type type);

    std::string get_short_color(Couleur color);

    std::string type_to_string(Type type);

    std::string color_to_string(Couleur color);
    std::string center_string(const std::string &str, int width);

    std::string colorize_line(const std::string &line);
    std::string colorize_string(const std::string& str, Couleur color);
    std::string replace_all(std::string str, const std::string &from, const std::string &to);
    int indiceDeDroite(int direction);
    int indiceDeGauche(int direction);
}