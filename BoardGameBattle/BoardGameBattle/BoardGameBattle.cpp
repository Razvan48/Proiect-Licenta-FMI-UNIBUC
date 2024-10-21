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

int main()
{
    FT_Library ft;
    if (FT_Init_FreeType(&ft))
    {
        std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
        return -1;
    }

    FT_Face face;
    //path-ul e relativ la proiect, nu la sln! trebuie un pas inapoi
    if (FT_New_Face(ft, "../assets/fonts/arial.ttf", 0, &face))
    {
        std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
        return -1;
    }


    irrklang::ISoundEngine* soundEngine = irrklang::createIrrKlangDevice();
    soundEngine->play2D("../assets/sounds/test.mp3", false);

    char junk;
    std::cin >> junk;

	return 0;
}



