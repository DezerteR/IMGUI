#include "FontRenderer.hpp"
#include "Assets.hpp"
#include "Logging.hpp"
#include <regex>
#include <fstream>

namespace UI {
float LastTextLength;
float LastTextHeight;

int convertFontFilenameToId(const std::string&){
    return 0;
}

float FontRenderer::getLen(const std::string &text){
    float len = 0.f;
    u8 letter;
    for (u32 i = 0; i < text.size(); i++){
        letter = text[i];
        // len += fontInfo->m_letters[letter].size.x; /// TODO:
    }
    return len;
}

void FontRenderer::clear(int layer){
    renderedSymbols.clear();
}
void FontRenderer::move(int x, int y){
    u32 len = renderedSymbols.size();
    glm::vec2 offset(x, y);
    for (u32 i = len - lastTextSize; i < len; i++){
        renderedSymbols[i].pxPosition += offset;
    }
}

/// Rendering
float FontRenderer::render(const std::string &text, int fontId, glm::vec2 position, HexColor color, int caretPosition){
    auto &font = assets::getFont(fontId);
    empty = false;
    LastTextHeight = 0;
    LastTextLength = 0;
    lastTextSize = text.size();
    glm::vec2 currentPosition = position;
    u8 character;

    for(u32 i = 0; i < text.size(); i++){
        character = text[i];
        auto &symbol = font.symbols[character];
        if(character == '\n'){ // new line
            LastTextLength = std::max(LastTextLength, currentPosition[0] - position.x);
            LastTextHeight += height;
            currentPosition = position - glm::vec2(0, LastTextHeight);
            continue;
        }

        if (i > 0){ // kerning
            currentPosition[0] += font.kerning[int(text[i - 1])<<16 & character];
        }

        renderedSymbols.push_back(RenderedSymbol{
            .pxPosition = currentPosition + symbol.pxOffset,
            .pxSize = symbol.pxSize,
            .uv = symbol.uv,
            .uvSize = symbol.uvSize,
            .color = color
        });
        currentPosition.x += symbol.pxAdvance;
    }

    if (lastTextSize > 0) // compute len
        LastTextLength = currentPosition[0] - position.x;

    placeCaret(text, fontId, position, color, caretPosition);
    return LastTextLength + 1.f;
}
float FontRenderer::render(const std::u16string &text, int fontId, glm::vec2 position, HexColor color, int caretPosition){
    auto &font = assets::getFont(fontId);
    empty = false;
    LastTextHeight = 0;
    LastTextLength = 0;
    lastTextSize = text.size();
    glm::vec2 currentPosition = position;
    char16_t character;

    for (u32 i = 0; i < text.size(); i++){
        character = text[i];
        auto &symbol = font.symbols[character];
        if (character == '\n'){ // new line
            LastTextLength = std::max(LastTextLength, currentPosition[0] - position.x);
            LastTextHeight += height;
            currentPosition = position - glm::vec2(0, LastTextHeight);
            continue;
        }
        else if (i > 0 and character < 256){ // kerning
            currentPosition[0] += font.kerning[int(text[i - 1])<<16 & character];
        }

        renderedSymbols.push_back(RenderedSymbol{
            .pxPosition = currentPosition + symbol.pxOffset,
            .pxSize = symbol.pxSize,
            .uv = symbol.uv,
            .uvSize = symbol.uvSize,
            .color = color
        });
        currentPosition.x += symbol.pxAdvance;
    }

    if (lastTextSize > 0) // compute len
        LastTextLength = currentPosition[0] - position.x;

    // placeCaret(text, position, color, caretPosition);
    return LastTextLength + 1.f;
}
void FontRenderer::placeCaret(const std::string &text, int fontId, glm::vec2 position, HexColor color, int caretPosition){
    auto &font = assets::getFont(fontId);
    auto &letters = font.symbols;
    if(caretPosition >= 0){ // key
        u8 character = '|';
        if((u32)caretPosition >= text.size() && text.size() > 0)
            position = renderedSymbols.back().pxPosition + renderedSymbols.back().pxSize*glm::vec2(1, 0);
        else if(caretPosition == 0)
            position = position - glm::vec2(0, letters[character].pxSize.y - height);
        else
            position = renderedSymbols[caretPosition].pxPosition;
        renderedSymbols.push_back(RenderedSymbol{
            .pxPosition = position,
            .pxSize = letters[character].pxSize,
            .uv = letters[character].uv,
            .uvSize = letters[character].uvSize,
            .color = color
        });
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
    log("regex:", rgx);
    return std::stoi(match[1]);
}
std::string regexStringSearch(const std::string &word, const std::string &rgx){
    std::smatch match;
    std::regex regex(rgx);
    std::regex_search(word, match, regex);
    return match[1];
}
std::vector<float> rgxVecFloatSearch(const std::string &line){
    std::vector<float> out;
    std::smatch match;
    std::regex regex(R"(([\-]?[0-9]+))");
    std::string word = line;
    while (std::regex_search(word, match, regex)){
        out.push_back(std::stof(match[1]));
        word = match.suffix().str();
    }
    return out;
}
std::vector<int> rgxVecIntSearch(const std::string &line){
    std::vector<int> out;
    std::smatch match;
    std::regex regex(R"(([\-]?[0-9]+))");
    std::string word = line;
    while (std::regex_search(word, match, regex)){\
        out.push_back(std::stoi(match[1]));
        word = match.suffix().str();
    }
    return out;
}

void Font::loadCharacter(const std::string &line, float U, float V, int pageOffset){
    auto &&vec = rgxVecFloatSearch(line);
    int16_t id = vec[0];
    float x = vec[1]; // ~
    float y = vec[2]; // distance from top to top of letter
    float width = vec[3]; // width
    float height = vec[4]; // height
    float xoffset = vec[5];
    float yoffset = vec[6];
    float xadvance = vec[7];
    float page = vec[8];
    symbols[id].uv = glm::vec3(x / U, 1.f - y / V - width / V, page + pageOffset);
    symbols[id].uvSize = glm::vec2(width / U, height / V);
    symbols[id].pxSize = glm::vec2(width, height);
    symbols[id].pxOffset = glm::vec2(xoffset, -yoffset);
    symbols[id].pxAdvance = xadvance;
}
void Font::loadKerning(const std::string &word){
    auto &&vec = rgxVecIntSearch(word);
    kerning[int(vec[0]<<16) & vec[1]] = vec[2];
}
void Font::load(const std::string &name, std::vector<std::string> &imagesToLoad){
    std::string path = "../res/fonts/";
    std::ifstream file;
    file.open(path + name, std::ios::in);
    if(not file.good()){
        error("Can't open file", path + name);
    }
    log("Font:", name);
    std::string info, common, count, page;
    std::vector<std::string> lines;
    getline(file, info);
    height = rgxIntSearch(info, R"(size=([0-9]+))");
    getline(file, common);
    int pages = rgxIntSearch(common, R"(pages=([0-9]+))");
    int pageOffset = imagesToLoad.size();
    for(auto i=0; i<pages; i++){
        getline(file, page);
        imagesToLoad.push_back(regexStringSearch(page, R"(file=\"(.*)\")"));
    }
    getline(file, count);

    float sizeU = (float)rgxIntSearch(common, R"(scaleW=([\-]?[0-9]+))");
    float sizeV = (float)rgxIntSearch(common, R"(scaleH=([\-]?[0-9]+))");
    int charCount = rgxIntSearch(count, R"(chars count=([\-]?[0-9]+))");

    lines.resize(charCount);
    for(int i = 0; i < charCount; i++){
        getline(file, lines[i]);
    }
    for(auto &it : lines){
        loadCharacter(it, sizeU, sizeV, pageOffset);
    }
    std::string kernings;
    getline(file, kernings);
    int kerningCount = rgxIntSearch(kernings, R"(kernings count=([\-]?[0-9]+))");
    lines.resize(kerningCount);

    for (int i = 0; i < kerningCount; i++)
        getline(file, lines[i]);
    for(auto &it : lines)
        loadKerning(it);

    file.close();
}

}
