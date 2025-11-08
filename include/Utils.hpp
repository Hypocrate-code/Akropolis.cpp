#pragma once
#include <string>
#include "Tuile.hpp"


char get_color_char(Couleur color) ;

std::string get_color_code(Couleur color) ;
std::string get_short_type(Type type) ;

std::string get_short_color(Couleur color);

std::string type_to_string(Type type) ;

std::string color_to_string(Couleur color);
std::string center_string(const std::string& str, int width) ;