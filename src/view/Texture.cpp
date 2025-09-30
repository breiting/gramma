#include <stb_image.h>

#include <gramma/view/Texture.hpp>
#include <iostream>

namespace gr {

Texture::~Texture() {
    if (m_Id) glDeleteTextures(1, &m_Id);
}

bool Texture::Load(const std::string& path) {
    int channels;
    unsigned char* data = stbi_load(path.c_str(), &m_Width, &m_Height, &channels, 0);
    if (!data) {
        std::cerr << "Failed to load texture: " << path << "\n";
        return false;
    }

    glGenTextures(1, &m_Id);
    glBindTexture(GL_TEXTURE_2D, m_Id);
    glTexImage2D(GL_TEXTURE_2D, 0, channels == 4 ? GL_RGBA : GL_RGB, m_Width, m_Height, 0,
                 channels == 4 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_image_free(data);
    return true;
}

}  // namespace gr
