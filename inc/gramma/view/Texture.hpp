#pragma once
#include <glad.h>

#include <string>

namespace gr {

/** Simple texture loader using stb_image. */
class Texture {
   public:
    Texture() = default;
    ~Texture();

    /** Load texture from file. */
    bool Load(const std::string& path);

    /** Get OpenGL texture ID. */
    GLuint Id() const {
        return m_Id;
    }

    /** Get texture dimensions. */
    int Width() const {
        return m_Width;
    }
    int Height() const {
        return m_Height;
    }

   private:
    GLuint m_Id = 0;
    int m_Width = 0, m_Height = 0;
};

}  // namespace gr