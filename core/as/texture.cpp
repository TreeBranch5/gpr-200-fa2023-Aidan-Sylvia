//texture.cpp
#include "texture.h"
#include "../ew/external/stb_image.h"
#include "../ew/external/glad.h"

unsigned int loadTexture(
    const char* filePath,
    GLint filterMode = GL_LINEAR,       // Default filter mode
    GLint wrapMode = GL_REPEAT          // Default wrap mode
) {

    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    stbi_set_flip_vertically_on_load(true);

    int width, height, numComponents;
    unsigned char* data = stbi_load(filePath, &width, &height, &numComponents, 0);
    if (data == NULL) {
        printf("Failed to load image %s", filePath);
        stbi_image_free(data);
        return 0;
    }
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

    // Set wrap mode for both S (horizontal) and T (vertical) coordinates
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapMode);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapMode);

    // Set filter mode for both minification and magnification
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filterMode);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filterMode);

    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(data);
    return texture;
}