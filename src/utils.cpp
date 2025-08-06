#include "utils.hpp"

SDL_Keycode CharToSDLKeycode(char c) {
    if (std::isalpha(static_cast<unsigned char>(c))) {
        char up = static_cast<char>(std::toupper(static_cast<unsigned char>(c)));
        std::string name(1, up);
        return SDL_GetKeyFromName(name.c_str());
    }

    if (std::isdigit(static_cast<unsigned char>(c))) {
        std::string name(1, c);
        return SDL_GetKeyFromName(name.c_str());
    }

    switch (c) {
        case ' ' : return SDLK_SPACE;
        case '-' : return SDLK_MINUS;
        case '=' : return SDLK_EQUALS;
        case '[' : return SDLK_LEFTBRACKET;
        case ']' : return SDLK_RIGHTBRACKET;
        case '\\': return SDLK_BACKSLASH;
        case ';' : return SDLK_SEMICOLON;
        case '\'' : return SDLK_QUOTE;
        case ',' : return SDLK_COMMA;
        case '.' : return SDLK_PERIOD;
        case '/' : return SDLK_SLASH;
        case '`' : return SDLK_BACKQUOTE;
        default:
            // Unknown or unsupported key
            return SDLK_UNKNOWN;
    }
}

SDL_Scancode CharToSDLScancode(char c) {
    unsigned char uc = static_cast<unsigned char>(c);

    // Letters: map 'a'–'z' or 'A'–'Z' to SDL_SCANCODE_A–SDL_SCANCODE_Z
    if (std::isalpha(uc)) {
        char up = static_cast<char>(std::toupper(uc));
        return static_cast<SDL_Scancode>(SDL_SCANCODE_A + (up - 'A'));
    }

    // Digits: map '0'–'9' to SDL_SCANCODE_0–SDL_SCANCODE_9
    if (std::isdigit(uc)) {
        return static_cast<SDL_Scancode>(SDL_SCANCODE_0 + (uc - '0'));
    }

    // Punctuation and special characters
    switch (c) {
        case ' ':  return SDL_SCANCODE_SPACE;
        case '-':  return SDL_SCANCODE_MINUS;
        case '=':  return SDL_SCANCODE_EQUALS;
        case '[':  return SDL_SCANCODE_LEFTBRACKET;
        case ']':  return SDL_SCANCODE_RIGHTBRACKET;
        case '\\': return SDL_SCANCODE_BACKSLASH;
        case ';':  return SDL_SCANCODE_SEMICOLON;
        case '\'': return SDL_SCANCODE_APOSTROPHE;
        case ',':  return SDL_SCANCODE_COMMA;
        case '.':  return SDL_SCANCODE_PERIOD;
        case '/':  return SDL_SCANCODE_SLASH;
        case '`':  return SDL_SCANCODE_GRAVE;
        default:
            return SDL_SCANCODE_UNKNOWN;
    }
}

