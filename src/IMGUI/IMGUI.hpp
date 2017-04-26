#pragma once
#include <GLFW/glfw3.h>
#include <functional>
#include "Utils.hpp"
#include "Common.hpp"
#include "BaseStructs.hpp"
#include "KeyState.hpp"
#include "FontRenderer.hpp"
#include "UIGraphicComponent.hpp"
#include "Style.hpp"
#include "UIUpdater.hpp"

class Window;

namespace UI {
typedef glm::vec4 Box;
extern bool GetInput;

class IMGUI;
struct UISettings
{};

struct IMGUIBox
{
    int m_flags{ 0 };
    Box m_box;
    Box m_currStart;
    Box m_freeRect;
    int m_border;
    int m_style;
    int m_rectIdx;
    IMGUI *imgui;

    IMGUIBox& box(int flags, Box spawnPosition, IMGUI *_imgui);
    IMGUIBox& size(float x = 0, float y = 0);
    IMGUIBox& size(const Box &box){ return size(box.z, box.w); }
    IMGUIBox& size(glm::vec2 box){ return position(box.x, box.y); }

    IMGUIBox& overridePosition(float x = 0, float y = 0); // box isn't inserted
    IMGUIBox& position(float x = 0, float y = 0); // box isn't inserted
    IMGUIBox& position(const Box &box){ return position(box.x, box.y); } // box isn't inserted
    IMGUIBox& position(glm::vec2 box){ return position(box.x, box.y); } // box isn't inserted

    IMGUIBox& offset(float x = 0, float y = 0); // makes box inserted
    IMGUIBox& offset(const Box &box){ return offset(box.x, box.y); }
    IMGUIBox& offset(glm::vec2 box){ return offset(box.x, box.y); }

    IMGUIBox& border(int b = 0);
    IMGUIBox& operator()();


    // "derived" from imgui
    Box getSpawnPoint(const Box &r);
    Box insertRect(const Box &r);
    Box placeRect(const Box &r);

    IMGUIBox& onGroupHover(std::function<void(Box rect)>fun);
    IMGUIBox& onGrouplClick(std::function<void(Box rect)>fun);
    bool onGroupGrab(std::function<void(Box rect)>fun);
};

struct FigureInfo
{
    Box r;
    GLuint textureID;
    GLuint shader;
    std::vector <std::pair<std::string, glm::mat4>> m_matrices;
    std::vector <std::pair<std::string, Box>> m_vec4;
    std::vector <std::pair<std::string, glm::vec3>> m_vec3;
    std::vector <std::pair<std::string, glm::vec2>> m_vec2;
    std::vector <std::pair<std::string, float>> m_float;
};

enum GroupFlags
{
    LayoutVertical = 0x01,
    LayoutHorizontal = 0x02,

    AlignLeft = 0x04,
    AlignRight = 0x08,

    AlignTop = 0x10,
    AlignBottom = 0x20,
    Centered = 0x40,

    Draw = 0x100,
    Background = 0x200,

    HotFix1 = 0x400,
    CenterText = 0x1000,
    TextToLeft = 0x2000,
    TextToRight = 0x4000,
    FixedSize = 0x10000,
    FixedPos = 0x20000,
    FixedPos2 = 0x40000,
    NewLayer = 0x80000,
    RelativePosition = 0x100000,
    AbsolutePosition = 0x200000,
    NoInsertion = 0x400000,

    HorizonBottom = LayoutHorizontal | AlignLeft | AlignBottom,
    HorizonTop = LayoutHorizontal | AlignLeft | AlignTop,
    VertBottom = LayoutVertical | AlignLeft | AlignBottom,
    VertTop = LayoutVertical | AlignLeft | AlignTop,

    ClearLayout = LayoutHorizontal | LayoutVertical | Draw | FixedSize | FixedPos,
};

enum class TypeInfo : int32_t
{
    EMPTY, INT32_t, UINT32_t, INT64_t, UINT64_t, STRING, FLOAT, DOUBLE
};
class TextEditor
{
    enum TextEditorFlags {
        EMPTY = 0, STRING = 1, FLOAT = 6, INT = 10, HEX = 16, BINARY = 2, SYMBOLIC = 6,
    };
public:
    TypeInfo m_typeInfo;
    TextEditor();
    template<typename T>
    void setValueToEdit(T &value, int base);
    template<typename T>
    void setValueToEdit(T &value);

    float parseExpr(std::string &str);

    template<typename T>
    bool compare(T &value){
        return &value == this->valuePointer;
    }

    std::string value();
    bool state();
    void finishEdition();
    void breakEdition();
    int caretPosition();
    void input(u32 key, u32 action, u32 mod);

    void *valuePointer;
    std::string currentString;
    bool m_state;
    int m_caretPosition;
    int m_base;
private:
};

struct CurrentItem
{
    HexColor textColor;
    HexColor borderColor;
    HexColor color;
    int buttonState;
    glm::vec4 box;
    bool hover;
    bool lClicked;
    bool rClicked;
    int style;
    int buttonFlags;
    bool imageEnabled;
    Icon icon;
    bool noDraw;
    bool active; /// dość niefortunna
    Box lastBox; /// przenieść to do IMGUI/kontenera

    int flag; /// coś z tekstem
    std::string text;
    int caret;
    std::string font;
    float textOffset;
    int fontSize;

    bool activeEdition;
    bool editBox; /// nie widzę różnicy pomiedzy
    bool special;

    void *currentSlider;
};

class IMGUI
{
    CurrentItem item {};
    glm::vec4 bounds;
    Updater &updater;
public:
    UIGraphicComponent m_uiGraphic;
    TextEditor textEditor;
    FontRenderer fontRenderer;

    IMGUI(glm::vec4 bounds, Updater &updater) : m_uiGraphic(m_style), bounds(bounds), updater(updater){}

    void printTextEditorValue();
    template<typename T>
    void processTE(T &value, int base);
    template<typename T>
    void processTE(T &value);

    template<typename ...Args>
    IMGUI& button(Args &&...args){
        item.style = Style::Button;
        return rect(args...);
    }
    template<typename ...Args>
    IMGUI& label(Args &&...args){
        item.style = Style::Label;
        return rect(args...);
    }
    template<typename ...Args>
    IMGUI& editbox(Args &&...args){
        item.style = Style::Editbox;
        return rect(args...);
    }

    IMGUIBox& box(int flags);
    IMGUIBox& endBox();

    IMGUIBox& table(int flags);
    IMGUIBox& table(Box position, int flags);
    IMGUIBox& endTable();

    IMGUIBox& parentBox(){
        return m_boxStack[m_boxIndex - 1];
    }
    IMGUIBox& currentBox(){
        return m_boxStack[m_boxIndex];
    }

    IMGUI& circleShape(const Box &circle);
    IMGUI& rotatedBoxShape(Box &startEnd, float distance, float &proportion);
    IMGUI& customShape(std::function<bool(glm::vec2)> &fun);
    IMGUI& customShape(bool hovered);

    IMGUI& image(const std::string &name);
    IMGUI& image(int x, int y, const std::string &name);
    IMGUI& rect();
    IMGUI& rect(Box r);
    IMGUI& rect(HexColor r);
    IMGUI& rect(int x, int y, int w, int h);
    IMGUI& rect(glm::vec2 xy, int w, int h);
    IMGUI& rect(glm::vec2 xy, glm::vec2 wh);
    IMGUI& rect(int w, int h);
    IMGUI& rect(int w, int h, HexColor color);
    IMGUI& rect(Box r, HexColor color);
    IMGUI& rect(int x, int y, int w, int h, HexColor color);

    IMGUI& setBox(const Box &box);
    IMGUI& size(float x = 0, float y = 0);
    IMGUI& offset(float x = 0, float y = 0);

    template<typename T>
    IMGUI& switcher(T &state, const T &val){
        if(item.lClicked)
            state = val;
        item.active = (state == val);
        return *this;
    }
    IMGUI& activeElement(const std::string &image);
    IMGUI& button(bool &state);
    IMGUI& onLMB(std::function<void(void)>fun);
    IMGUI& onLMB(std::function<void(Box rect)>fun);
    IMGUI& onRMB(std::function<void(Box rect)>fun);
    IMGUI& onRMB(std::function<void(void)>fun);

    IMGUI& onRepeat(std::function<void(void)>fun);
    IMGUI& onRepeat(std::function<void(void)>fun, uint32_t freq);
    IMGUI& onRepeat(std::function<void(Box)>fun);
    IMGUI& onrRepeat(std::function<void(void)>fun);

    IMGUI& onHover(std::function<void(void)>fun);
    IMGUI& onSpecial(std::function<void(void)>fun);
    IMGUI& onHover(std::function<void(Box rect)>fun);
    IMGUI& onEverytime(std::function<void(Box rect)>fun);
    IMGUI& onActive(std::function<void(void)>fun);
    IMGUI& onActive(std::function<void(Box rect)>fun);
    IMGUI& onEdition(std::function<void(void)>fun);

    IMGUI& text(const std::string &text, int font = font::Default, int flag = 0x2000, int caretPosition = -1);
    IMGUI& text(const std::u16string &text, int font = font::Default, int flag = 0x2000, int caretPosition = -1);

    // IMGUI& format(const std::string &font, int flag = 0x2000, int caretPosition = -1){
    //     this->m_flag |= flag;
    //     this->m_caretPosition = caretPosition;
    //     this->m_font = font;
    //     return *this;
    // }

    IMGUI& color(HexColor hexColor);
    IMGUI& boxColor(HexColor hexColor);
    IMGUI& font(const std::string &font);
    IMGUI& getRect(Box &r);
    IMGUI& mouseOffset(Box &out);
    IMGUI& mouseOffset(glm::vec2 &out);

    template<typename T>
    IMGUI& edit(T &value, int base = 10);
    IMGUI& edit(float &value, int flag = 0);
    IMGUI& edit(double &value, int flag = 0);
    IMGUI& edit(std::string &value, int flag = 0);
    IMGUI& edit(std::string *value, std::function<void(void)>fun);
    IMGUI& edit(const std::string &value, std::function<void(const std::string&)> callback, void *identifier);

    IMGUI& setFlag(int flag);
    IMGUI& slider(float &value, float min = 0.f, float max = 1.f);
    IMGUI& slider(double &value, double min = 0.0, double max = 1.0);
    IMGUI& slider(i64 &value, i64 min, i64 max);
    IMGUI& slider(u32 &value, u32 min, u32 max);
    IMGUI& slider(i32 &value, i32 min, i32 max);
    IMGUI& operator () (int flags = 0);

    void restoreDefaults();
    void begin();
    void end();

    Box fixRect(Box rect);
    Box placeRect(const Box &r);
    Box placeGroup(int flags);
    bool hasHover(Box rect);
    bool findCollision(Box &r, Box &out);
    bool overlay(Box &rect, Box &point);

    void mouseKeyInput(int key, int action);
    void keyInput(int key, int action, int mod);
    bool keyAction(int key, bool *state);
    bool keyAction(int key, std::function<void(void)>fun);
    bool mouseKeyAction(int key, bool *state);

    bool onRightClick(bool *state);
    bool onEnter(bool *state);
    bool onESC(bool *state);
    bool onESC(std::function<void(void)>fun);
    bool onEnter(std::function<void(void)>fun);
    bool tableHover();
    bool outOfTable();

    void onGroupHover(std::function<void(Box rect)>fun);
    void onGrouplClick(std::function<void(Box rect)>fun);
    bool onGroupGrab(std::function<void(Box rect)>fun);
    void setFont(const std::string &s);
    void setfont(const std::string &s, int size);

    void forceClick();

    void switchDrawing();

    Box getBox();

    void drawOnlyTopLayer();
    void beginLayer();
    void endLayer();

    void border(int borderSize);
    void indentation(int indentationLen);

    bool captureMouse;
    void updateCounter(float deltaTime);

    void updateCounter(uint32_t deltaTime);

    std::vector <Box> m_rects;
    std::vector <std::pair<Box, std::string>> m_shapePoint;
    std::vector <Box> m_collRects;
    float accu;
    float accu2;
    float frequency;
    ImageSet *m_imageSet;
    void setDefaultFont(std::string font, int size);
    bool hover();
    HexColor m_defaultColor{ 0xb0b0ffff };
    HexColor m_defaultFontColor{ 0xffffffff };
    Box m_defaultRect{ 0, 0, 150, 22 };
    std::string m_defaultFont{ "ui_12" };
    int m_defaultFontSize{ 12 };
    Style m_style;
    int m_buttonFlags;
    std::string m_font { "ui_12" };
    IMGUIBox m_group;
    IMGUIBox m_boxStack[20];
    int m_groupIndex;
    int m_boxIndex;

    bool m_force;
    bool counter;
    bool MS500OSCILATOR;
    bool MS50IMPULSE;
    bool MS100IMPULSE;
    uint32_t timeFromstart {0};


    int currentLayer;
    int tmpMaxLayer;
    int maxLayer;
    int drawLayer;
    int layerToDraw;
    int m_key, m_action, m_mod;
    int m_mouseKey, m_mouseAction;
    std::unordered_map<std::string, Font> fonts;
    Box m_lastBox;
    std::string name { "ui" };
private:
    inline bool force(){
        if (m_force){
            m_force = false;
            return true;
        }
        return false;
    }
    void *currentSlider { nullptr };
};

template<typename T>
TypeInfo recognizeType(){
    return TypeInfo::INT32_t;
}

template<typename T>
std::string prettyPrint(T &value, int base);

void loadStyles(IMGUI &ui);



}
extern UI::IMGUI ui;
#define vertical(x) ui.box(UI::LayoutVertical); \
                                                x\
                                        ui.endBox();
#define horizontal(x) ui.box(UI::LayoutHorizontal); \
                                                x\
                                        ui.endBox();
