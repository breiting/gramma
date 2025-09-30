#include <fstream>
#include <gramma/view/Font.hpp>
#include <iostream>

namespace gr {

Font::Font() = default;

Font::~Font() {
    for (auto& pair : m_charTextures) {
        glDeleteTextures(1, &pair.second);
    }
}

bool Font::Load(const std::string& path, float fontSize) {
    m_fontSize = fontSize;
    std::ifstream file(path, std::ios::binary | std::ios::ate);
    if (!file) {
        std::cerr << "Failed to open font file: " << path << "\n";
        return false;
    }
    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);
    m_ttfBuffer.resize(size);
    if (!file.read(reinterpret_cast<char*>(m_ttfBuffer.data()), size)) {
        std::cerr << "Failed to read font file\n";
        return false;
    }
    m_ttfData = m_ttfBuffer.data();
    if (!stbtt_InitFont(&m_fontInfo, m_ttfData, stbtt_GetFontOffsetForIndex(m_ttfData, 0))) {
        std::cerr << "Failed to load font\n";
        return false;
    }
    return true;
}

GLuint Font::GetCharTexture(char c) {
    if (m_charTextures.find(c) != m_charTextures.end()) {
        return m_charTextures[c];
    }

    // Bake character
    unsigned char bitmap[512 * 512];
    stbtt_bakedchar data;
    int result = stbtt_BakeFontBitmap(m_ttfData, 0, m_fontSize, bitmap, 512, 512, c, 1, &data);
    if (result < 0) {
        std::cerr << "Failed to bake char " << c << "\n";
        return 0;
    }

    GLuint tex;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, 512, 512, 0, GL_RED, GL_UNSIGNED_BYTE, bitmap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    m_charTextures[c] = tex;
    m_charData[c] = data;
    return tex;
}

stbtt_bakedchar* Font::GetCharData(char c) {
    return &m_charData[c];
}

}  // namespace gr
