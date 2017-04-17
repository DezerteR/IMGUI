#pragma once
#include "Common.hpp"
#include <iterator>
#include <memory>

namespace UI {
extern float LastTextLength;
extern float LastTextHeight;

struct LetterInfo {
    glm::vec4 uv;         // [0-1]
    glm::vec2 size;     // px
    glm::vec2 offset;    // px
    float advance;// px
    std::unordered_map <char, float> kerning;
};

struct UnicodeSymbolInfo {
    glm::vec4 uv;         // [0-1]
    glm::vec2 size;     // px
    glm::vec2 offset;    // px
    float advance;// px
};

struct FontInfo
{
    uint32_t texWithLetters;
    uint32_t texWithSymbols;
    LetterInfo m_letters[256];
    std::unordered_map<char16_t, UnicodeSymbolInfo>m_symbols;
};

struct RenderedData
{
    int ID;
    int m_size;
    glm::vec2 size;
    std::vector <glm::vec2> positions; // dolny lewy r�g znaku
    std::vector <glm::vec2> sizes; // dolny lewy r�g znaku
    std::vector <glm::vec4> uvs; // pozycja uv + d�ugo�� w uv
    std::vector <HexColor> colors; // kolor
    std::vector <float> total; // wszystko *reinterpret_cast<float*>(&v)
};

/**
 *  Class to rendering fonts.
 *  Contains rendered data for each layer separately, font data are shared between renderers
 *
 *  Fonts are rendered dynamically to allow moving them
 *
 */
class Font
{
private:
    float genSingleSymbol(char16_t symbol, glm::vec2 position, HexColor color);
    void insertFont(const std::string &word, float U, float V);
    void insertSymbol(const std::string &word, float U, float V);
    void loadKerning(const std::string &word);
    void placeCaret(const std::string &text, glm::vec2 position, HexColor color, int caretPosition);

public:
    float render(const std::string &text, glm::vec2 position, HexColor color, int caretPosition = -1);
    float render(const std::u16string &text, glm::vec2 position, HexColor color, int caretPosition = -1);
    float getLen(const std::string &text);
    void clear(int layer);
    void loadLetters(std::string name);
    void loadSymbols(std::string name);
    void move(int x, int y);

    std::string name;
    RenderedData renderedFonts[3];
    RenderedData renderedSymbols[3];
    int height;
    int lastTextSize{ 0 };
    bool empty[3] = { true, true, true};
    std::shared_ptr<FontInfo> fontInfo;
};

glm::vec2 splitVec2(std::string toSplit);
glm::vec4 splitVec4(std::string toSplit);

}
