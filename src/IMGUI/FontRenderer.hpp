#pragma once
#include "Common.hpp"
#include <iterator>
#include <memory>

namespace font {
    enum Font {
        UI = 100,
        HUD = 200,
        Console = 300,

        Default = UI + 12,
        UI12 = UI + 12,
        UI16 = UI + 16,
    };}

namespace UI {
extern float LastTextLength;
extern float LastTextHeight;

struct UnicodeSymbolInfo {
    glm::vec3 uv; // [0-1]
    glm::vec2 uvSize; // [0-1]
    glm::vec2 pxSize; // px
    glm::vec2 pxOffset; // px
    float pxAdvance;// px
};

struct RenderedData
{
    int ID;
    int m_size;
    glm::vec2 size;
    /// TODO: zamienić na AoS
    std::vector <glm::vec2> positions; // pozycja na ekranie
    std::vector <glm::vec2> sizes; // wymiar na ekranie
    std::vector <glm::vec3> uv; // pozycja uv
    std::vector <glm::vec2> uvSize; // wymiar uv
    std::vector <HexColor> colors; // kolor
    std::vector <float> total; // wszystko *reinterpret_cast<float*>(&v)
};

class Font
{
private:
    void loadCharacter(const std::string &word, float U, float V, int pageOffset);
    void loadKerning(const std::string &word);
public:
    std::unordered_map<char16_t, UnicodeSymbolInfo> symbols;
    std::unordered_map<uint32_t, float> kerning;/// a.id<<16 & b.id

    void load(const std::string &name, std::vector<std::string> &imagesToLoad);

    float height;
};

int convertFontFilenameToId(const std::string&);

class FontRenderer
{
private:
    void placeCaret(const std::string &text, int fontId, glm::vec2 position, HexColor color, int caretPosition);

public:
    float render(const std::string &text, int fontId, glm::vec2 position, HexColor color, int caretPosition = -1);
    float render(const std::u16string &text, int fontId, glm::vec2 position, HexColor color, int caretPosition = -1);
    float getLen(const std::string &text);
    void clear(int layer);
    void move(int x, int y);

    std::string name;
    RenderedData renderedSymbols;
    int height;
    int lastTextSize{ 0 };
    bool empty {true};
};

glm::vec2 splitVec2(std::string toSplit);
glm::vec4 splitVec4(std::string toSplit);

}
