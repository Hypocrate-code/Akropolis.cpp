#include "Utils.hpp"

char get_color_char(Couleur color) {
    switch(color) {
        case Couleur::Rouge: return 'R';
        case Couleur::Vert: return 'V';
        case Couleur::Bleu: return 'B';
        case Couleur::Jaune: return 'J';
        case Couleur::Violet: return 'P';
        case Couleur::nulle: return 'N';
        default: return '?';
    }
}

std::string get_color_code(Couleur color) {
    switch(color) {
        case Couleur::Rouge: return "\033[31m";  // Red
        case Couleur::Vert: return "\033[32m";   // Green
        case Couleur::Bleu: return "\033[34m";   // Blue
        case Couleur::Jaune: return "\033[33m";  // Yellow
        case Couleur::Violet: return "\033[35m"; // Magenta
        case Couleur::nulle: return "\033[37m";  // White
        default: return "\033[37m";              // White
    }
}

std::string get_short_type(Type type) {
    switch(type) {
        case Type::Carriere: return "CAR";
        case Type::Quartier: return "QUA";
        case Type::Place: return "PLA";
        case Type::Fantome: return "FAN";
        default: return "UNK";
    }
}

std::string get_short_color(Couleur color) {
    switch(color) {
        case Couleur::Rouge: return "ROU";
        case Couleur::Vert: return "VER";
        case Couleur::Bleu: return "BLE";
        case Couleur::Jaune: return "JAU";
        case Couleur::Violet: return "VIO";
        case Couleur::nulle: return "NUL";
        default: return "UNK";
    }
}

// Rest of the helper functions remain the same...
std::string type_to_string(Type type) {
    switch(type) {
        case Type::Carriere: return "Carriere";
        case Type::Quartier: return "Quartier";
        case Type::Place: return "Place";
        case Type::Fantome: return "Fantome";
        default: return "Unknown";
    }
}

std::string color_to_string(Couleur color) {
    switch(color) {
        case Couleur::Rouge: return "Rouge";
        case Couleur::Vert: return "Vert";
        case Couleur::Bleu: return "Bleu";
        case Couleur::Jaune: return "Jaune";
        case Couleur::Violet: return "Violet";
        case Couleur::nulle: return "nulle";
        default: return "Unknown";
    }
}



std::string center_string(const std::string& str, int width) {
    std::cout << "IN CENTER STRING"<<str<< "."<< std::endl;
    std::string cleanStr = str;
    size_t pos = 0;
    int colorCodeLength = 0;
    
    // Remove ANSI color codes for length calculation
    while ((pos = cleanStr.find("\033[", pos)) != std::string::npos) {
        size_t end = cleanStr.find('m', pos);
        if (end != std::string::npos) {
            colorCodeLength += (end - pos + 1);
            cleanStr.erase(pos, end - pos + 1);
        }
    }
    
    int visibleLength = cleanStr.length();
    if (visibleLength >= width) {
        return str.substr(0, width + colorCodeLength);
    }
    
    int padding = width - visibleLength;
    int leftPadding = padding / 2;
    int rightPadding = padding - leftPadding;
    
    return std::string(leftPadding, '|') + str + std::string(rightPadding, '|');
}