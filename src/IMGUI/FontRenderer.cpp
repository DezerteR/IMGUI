#include "FontRenderer.hpp"
#include <regex>
#include <fstream>

namespace UI {
float LastTextLength;
float LastTextHeight;
int g_UILayer = 0;

float Font::getLen(const std::string &text){
    float len = 0.f;
    u8 letter;
    for (u32 i = 0; i < text.size(); i++){
        letter = text[i];
        len += fontInfo->m_letters[letter].size.x;
    }
    return len;
}

void Font::clear(int layer){
    empty[layer] = true;
    LastTextHeight = 0;
    renderedFonts[layer].m_size = 0;
    renderedFonts[layer].positions.clear();
    renderedFonts[layer].uvs.clear();
    renderedFonts[layer].sizes.clear();
    renderedFonts[layer].colors.clear();

    renderedFonts[layer].positions.reserve(1000);
    renderedFonts[layer].uvs.reserve(1000);
    renderedFonts[layer].sizes.reserve(1000);
    renderedFonts[layer].colors.reserve(1000);
    renderedSymbols[layer].m_size = 0;
    renderedSymbols[layer].positions.clear();
    renderedSymbols[layer].uvs.clear();
    renderedSymbols[layer].sizes.clear();
    renderedSymbols[layer].colors.clear();

    renderedSymbols[layer].positions.reserve(100);
    renderedSymbols[layer].uvs.reserve(100);
    renderedSymbols[layer].sizes.reserve(100);
    renderedSymbols[layer].colors.reserve(100);
}
void Font::move(int x, int y){
    u32 len = renderedFonts[g_UILayer].positions.size();
    glm::vec2 offset(x, y);
    for (u32 i = len - lastTextSize; i < len; i++){
        renderedFonts[g_UILayer].positions[i] += offset;
    }
}

/// Rendering
float Font::render(const std::string &text, glm::vec2 position, HexColor color, int caretPosition){
    empty[g_UILayer] = false;
    LastTextHeight = 0;
    LastTextLength = 0;
    lastTextSize = text.size();
    glm::vec2 currentPosition = position;
    u8 letter;

    for(u32 i = 0; i < text.size(); i++){
        letter = text[i];
        auto &letterInfo = fontInfo->m_letters[letter];
        if(letter == '\n'){ // new line
            LastTextLength = std::max(LastTextLength, currentPosition[0] - position.x);
            LastTextHeight += height;
            currentPosition = position - glm::vec2(0, LastTextHeight);
            continue;
        }

        if (i > 0){ // kerning
            currentPosition[0] += letterInfo.kerning[text[i - 1]];
        }

        renderedFonts[g_UILayer].positions.push_back(currentPosition + letterInfo.offset);
        renderedFonts[g_UILayer].sizes.push_back(letterInfo.size);
        renderedFonts[g_UILayer].uvs.push_back(letterInfo.uv);
        renderedFonts[g_UILayer].colors.push_back(color);
        currentPosition.x += letterInfo.advance;
    }

    if (lastTextSize > 0) // compute len
        LastTextLength = currentPosition[0] - position.x;

    placeCaret(text, position, color, caretPosition);
    renderedFonts[g_UILayer].m_size = renderedFonts[g_UILayer].uvs.size();
    return LastTextLength + 1.f;
}
float Font::render(const std::u16string &text, glm::vec2 position, HexColor color, int caretPosition){
    empty[g_UILayer] = false;
    LastTextHeight = 0;
    LastTextLength = 0;
    lastTextSize = text.size();
    glm::vec2 currentPosition = position;
    char16_t letter;

    for (u32 i = 0; i < text.size(); i++){
        letter = text[i];
        auto &letterInfo = fontInfo->m_letters[letter];
        if (letter == '\n'){ // new line
            LastTextLength = std::max(LastTextLength, currentPosition[0] - position.x);
            LastTextHeight += height;
            currentPosition = position - glm::vec2(0, LastTextHeight);
            continue;
        }
        if (letter > L'\u00ff'){
            currentPosition.x += genSingleSymbol(letter, currentPosition, color);
            continue;
        }
        else if (i > 0){ // kerning
            currentPosition[0] += letterInfo.kerning[text[i - 1]];
        }
        renderedFonts[g_UILayer].m_size++;

        renderedFonts[g_UILayer].positions.push_back(currentPosition + letterInfo.offset);
        renderedFonts[g_UILayer].sizes.push_back(letterInfo.size);
        renderedFonts[g_UILayer].uvs.push_back(letterInfo.uv);
        renderedFonts[g_UILayer].colors.push_back(color);
        currentPosition.x += letterInfo.advance;
    }

    if (lastTextSize > 0) // compute len
        LastTextLength = currentPosition[0] - position.x;

    // placeCaret(text, position, color, caretPosition);
    renderedFonts[g_UILayer].m_size = renderedFonts[g_UILayer].uvs.size();
    return LastTextLength + 1.f;
}
float Font::genSingleSymbol(char16_t symbol, glm::vec2 position, HexColor color){
    empty[g_UILayer] = false;
    auto &symbolInfo = fontInfo->m_symbols[symbol];
    renderedSymbols[g_UILayer].positions.push_back(position + symbolInfo.offset);
    renderedSymbols[g_UILayer].sizes.push_back(symbolInfo.size);
    renderedSymbols[g_UILayer].uvs.push_back(symbolInfo.uv);
    renderedSymbols[g_UILayer].colors.push_back(color);
    renderedSymbols[g_UILayer].m_size = renderedSymbols[g_UILayer].uvs.size();
    return symbolInfo.advance;
}
void Font::placeCaret(const std::string &text, glm::vec2 position, HexColor color, int caretPosition){
    auto &letters = fontInfo->m_letters;
    if(caretPosition >= 0){ // key
        u8 letter = '|';
        if((u32)caretPosition >= text.size() && text.size() > 0)
            renderedFonts[g_UILayer].positions.push_back(renderedFonts[g_UILayer].positions.back() + renderedFonts[g_UILayer].sizes.back()*glm::vec2(1, 0));
        else if(caretPosition == 0)
            renderedFonts[g_UILayer].positions.push_back(position - glm::vec2(0, letters[letter].size.y - height));
        else
            renderedFonts[g_UILayer].positions.push_back(renderedFonts[g_UILayer].positions[caretPosition]);
        renderedFonts[g_UILayer].uvs.push_back(letters[letter].uv);
        renderedFonts[g_UILayer].sizes.push_back(letters[letter].size);
        renderedFonts[g_UILayer].colors.push_back(color);
    }
}

/// Loading utils
glm::vec2 splitVec2(std::string toSplit){
    glm::vec2 out;
    std::istringstream buf(toSplit);
    std::istream_iterator<std::string> beg(buf), end;
    int i = 0;
    for (; beg != end; beg++){
        out[i++] = (float)std::stoi(*beg);
    }
    return out;
}
glm::vec4 splitVec4(std::string toSplit){
    glm::vec4 out;
    std::istringstream buf(toSplit);
    std::istream_iterator<std::string> beg(buf), end;
    int i = 0;
    for (; beg != end; beg++){
        out[i++] = (float)std::stoi(*beg);
    }
    return out;
}

int rgxIntSearch(const std::string &word, const std::string &rgx){
    std::smatch match;
    std::regex regex(rgx);
    std::regex_search(word, match, regex);
    return std::stoi(match[1]);
}
std::vector<float> rgxVecFloatSearch(const std::string &_word){
    std::vector<float> out;
    std::smatch match;
    std::regex regex(R"(([\-]?[0-9]+))");
    std::string word = _word;
    while (std::regex_search(word, match, regex)){
        out.push_back(std::stof(match[1]));
        word = match.suffix().str();
    }
    return out;
}
std::vector<int> rgxVecIntSearch(const std::string &_word){
    std::vector<int> out;
    std::smatch match;
    std::regex regex(R"(([\-]?[0-9]+))");
    std::string word = _word;
    while (std::regex_search(word, match, regex)){\
        out.push_back(std::stoi(match[1]));
        word = match.suffix().str();
    }
    return out;
}

void Font::insertFont(const std::string &word, float U, float V){
    auto &&vec = rgxVecFloatSearch(word);
    int16_t id = vec[0];
    float x = vec[1]; // ~
    float y = vec[2]; // distance from top to top of letter
    float z = vec[3]; // width
    float w = vec[4]; // height
    float offset_x = vec[5];
    float offset_y = vec[6];
    float advance = vec[7];
    auto &letters = fontInfo->m_letters;
    letters[id].uv = glm::vec4(x / U, 1.f - y / V - w / V, z / U, w / V);
    letters[id].size = glm::vec2(z, w);
    letters[id].offset = glm::vec2(offset_x, -offset_y);
    letters[id].advance = advance;
}
void Font::insertSymbol(const std::string &word, float U, float V){
    auto &&vec = rgxVecFloatSearch(word);
    int16_t id = vec[0];
    float x = vec[1]; // ~
    float y = vec[2]; // distance from top to top of letter
    float z = vec[3]; // width
    float w = vec[4]; // height
    float offset_x = vec[5];
    float offset_y = vec[6];
    float advance = vec[7];
    auto &symbols = fontInfo->m_symbols;
    symbols[id];
    symbols[id].uv = glm::vec4(x / U, 1.f - y / V - w / V, z / U, w / V);
    symbols[id].size = glm::vec2(z, w);
    symbols[id].offset = glm::vec2(offset_x, -offset_y);
    symbols[id].advance = advance;

}
void Font::loadKerning(const std::string &word){
    auto &&vec = rgxVecIntSearch(word);
    fontInfo->m_letters[vec[1]].kerning[vec[0]] = vec[2];
}
void Font::loadLetters(std::string name){
    std::string path = "../res/fonts/";
    std::ifstream file;
    file.open(path + name + ".fnt", std::ios::in);
    std::string infoFaceLine, commonLine, countLine, buff;
    getline(file, infoFaceLine);
    getline(file, commonLine);
    getline(file, buff); // non used
    getline(file, countLine);
    //
    float sizeU = (float)rgxIntSearch(commonLine, R"(scaleW=([\-]?[0-9]+))");
    float sizeV = (float)rgxIntSearch(commonLine, R"(scaleH=([\-]?[0-9]+))");
    int charCount = rgxIntSearch(countLine, R"(count=([\-]?[0-9]+))");

    std::vector<std::string> lines;
    lines.resize(charCount);

    this->height = rgxIntSearch(commonLine, R"(lineHeight=([\-]?[0-9]+))");

    for (int i = 0; i < charCount; i++)
        getline(file, lines[i]);
    for(auto &it : lines)
        insertFont(it, sizeU, sizeV);
    getline(file, buff);
    /// TODO: kerning
    int    kerningCount = rgxIntSearch(commonLine, R"(count=([\-]?[0-9]+))");
    lines.resize(kerningCount);

    for (int i = 0; i < kerningCount; i++)
        getline(file, lines[i]);
    for(auto &it : lines)
        loadKerning(it);


    file.close();
}
void Font::loadSymbols(std::string name){
    std::string path = "../res/fonts/";
    std::ifstream file;
    file.open(path + name + ".fnt", std::ios::in);
    std::string infoFaceLine, commonLine, countLine, buff;
    getline(file, infoFaceLine);
    getline(file, commonLine);
    getline(file, buff); // non used
    getline(file, countLine);
    //
    float sizeU = (float)rgxIntSearch(commonLine, R"(scaleW=([\-]?[0-9]+))");
    float sizeV = (float)rgxIntSearch(commonLine, R"(scaleH=([\-]?[0-9]+))");
    int charCount = rgxIntSearch(countLine, R"(count=([\-]?[0-9]+))");
    std::vector<std::string> lines;
    lines.resize(charCount);
    this->height = rgxIntSearch(commonLine, R"(lineHeight=([\-]?[0-9]+))");;

    for (int i = 0; i < charCount; i++)
        getline(file, lines[i]);
    for(auto &it : lines)
        insertSymbol(it, sizeU, sizeV);
}

}
