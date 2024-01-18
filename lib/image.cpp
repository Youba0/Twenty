#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include "./stb_image.h"
#include <GL/gl.h>
#include <GL/glut.h>

GLuint backgroundTexture;

void loadTexture(const char *filename, int &width, int &height)
{
    int channels;
    unsigned char *image = stbi_load(filename, &width, &height, &channels, STBI_rgb_alpha);

    if (!image)
    {
        std::cerr << "Error loading texture." << std::endl;
        exit(1);
    }

    glGenTextures(1, &backgroundTexture);
    glBindTexture(GL_TEXTURE_2D, backgroundTexture);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);

    stbi_image_free(image);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, 0);
}

void drawBackground()
{
    glEnable(GL_TEXTURE_2D);
    int width = 20;
    int height = 25;
    glBegin(GL_QUADS);
    glTexCoord2i(0, 1);
    glVertex2i(0, height);
    glTexCoord2i(1, 1);
    glVertex2i(width, height);
    glTexCoord2i(1, 0);
    glVertex2i(width, 0);
    glTexCoord2i(0, 0);
    glVertex2i(0, 0);
    glEnd();

    glDisable(GL_TEXTURE_2D);
}