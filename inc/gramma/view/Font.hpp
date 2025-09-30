#pragma once
#include <glad.h>
#include <stb_truetype.h>

#include <unordered_map>
#include <vector>

namespace gr {

/** Simple font renderer using stb_truetype. */
class Font {
   public:
    Font();
    ~Font();

    /** Load font from TTF file path. */
    bool Load(const std::string& path, float fontSize);

    /** Get texture ID for a character. */
    GLuint GetCharTexture(char c);

    /** Get character metrics. */
    stbtt_bakedchar* GetCharData(char c);

    /** Get font size. */
    float GetFontSize() const {
        return m_fontSize;
    }

   private:
    stbtt_fontinfo m_fontInfo;
    std::vector<unsigned char> m_ttfBuffer;
    const unsigned char* m_ttfData;
    std::unordered_map<char, GLuint> m_charTextures;
    std::unordered_map<char, stbtt_bakedchar> m_charData;
    float m_fontSize;
};

}  // namespace gr
