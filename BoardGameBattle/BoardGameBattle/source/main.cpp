/*
#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <irrKlang.h>
*/

#include "Game/Game.h"

int main()
{
    /*
    FT_Library ft;
    if (FT_Init_FreeType(&ft))
    {
        std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
        return -1;
    }

    FT_Face face;
    //path-ul e relativ la proiect, nu la sln! trebuie un pas inapoi
    if (FT_New_Face(ft, "assets/fonts/arial.ttf", 0, &face))
    {
        std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
        return -1;
    }


    irrklang::ISoundEngine* soundEngine = irrklang::createIrrKlangDevice();
    soundEngine->play2D("assets/sounds/test.mp3", false);

    char junk;
    std::cin >> junk;
    */

    Game::get().start();

    // TODO: de testat daca pot desena
    // TODO: trebuie GlobalClock sa mai implementez? voi avea animatii vreodata?
    // TODO: de implementat Entity, TexturableEntity care mosteneste Entity
    // TODO: de vazut cum iau coordonatele la cursor
    // TODO: texturable entity ar trb sa aiba ceva update unde se schimba culoarea la buton in functie de situatie
    // TODO: un boolean global care spune daca player-ul a eliberat click-ul frame-ul anterior

    // TODO: de desenat tabla de sah, vazut cum o tin in memorie etc.
    // TODO: conexiunea cu serverul?

	return 0;
}



