#include "IMGUI.hpp"
#include "Yaml.hpp"

extern const float pi;

namespace UI {
bool GetInput;
/**
 * Box.xy: position
 * Box.z: circle radius
*/
IMGUI& IMGUI::circleShape(const Box &circle){

    this->m_box = Box(circle.x, circle.y, 20, 20);
    this->m_hover = (glm::distance2(circle.xy(), ks.mousePosition) < circle.z*circle.z) && !captureMouse;
    this->m_lClicked = this->m_hover && this->ks.lClick && !captureMouse;
    this->m_rClicked = this->m_hover && this->ks.rClick && !captureMouse;

    return *this;
}

/**
 * Box.xy: start position
 * Box.zw: end position
 *
*/
IMGUI& IMGUI::rotatedBoxShape(Box &startEnd, float distance, float &proportion){
    bool hovered = false;
    float lengthSquared = glm::distance2(startEnd.xy(), startEnd.zw());

    float t = glm::dot(ks.mousePosition-startEnd.xy(), startEnd.zw() - startEnd.xy())/lengthSquared;
    proportion = t;

    if(t < 0.f)
            hovered = glm::distance2(ks.mousePosition, startEnd.xy()) < distance*distance;
    else if(t > 1.f)
            hovered = glm::distance2(ks.mousePosition, startEnd.xy()) < distance*distance;
    else {
            glm::vec2 projection = startEnd.xy() + t*(startEnd.zw() - startEnd.xy());
            hovered = glm::distance2(ks.mousePosition, projection) < distance*distance;
    }

    this->m_box = Box((startEnd.x + startEnd.z)/2, (startEnd.y + startEnd.w)/2, 20, 20);
    this->m_lClicked = hovered && this->ks.lClick && !captureMouse;
    this->m_rClicked = hovered && this->ks.rClick && !captureMouse;

    return *this;
}
IMGUI& IMGUI::customShape(std::function<bool(glm::vec2)> &fun){
    bool hovered = fun(ks.mousePosition);

    this->m_box = Box(0,0,0,0);
    this->m_lClicked = hovered && this->ks.lClick && !captureMouse;
    this->m_rClicked = hovered && this->ks.rClick && !captureMouse;

    return *this;
}
IMGUI& IMGUI::customShape(bool hovered){

    this->m_box = Box(0,0,0,0);
    this->m_lClicked = hovered && this->ks.lClick && !captureMouse;
    this->m_rClicked = hovered && this->ks.rClick && !captureMouse;

    return *this;
}
IMGUI& IMGUI::image(const std::string &name){
    this->m_image = m_imageSet->set[name];
    this->m_box = currentBox().getSpawnPoint(this->m_image.rect);
    this->m_image.rect = this->m_box;
    this->m_hover = hasHover(this->m_box) && !captureMouse;
    this->m_lClicked = this->m_hover && this->ks.lClick;
    this->m_rClicked = this->m_hover && this->ks.rClick;
    this->m_imageEnbl = true;
    return *this;
}
IMGUI& IMGUI::image(int x, int y, const std::string &name){
    this->m_image = m_imageSet->set[name];
    this->m_image.rect[0] = x;
    this->m_image.rect[1] = y;
    this->m_box = this->m_image.rect;

    this->m_hover = hasHover(this->m_box);
    this->m_lClicked = this->m_hover && this->ks.lClick;
    this->m_rClicked = this->m_hover && this->ks.rClick;
    this->m_imageEnbl = true;
    this->m_buttonFlags |= NoInsertion;
    return *this;
}
IMGUI& IMGUI::rect(){

    this->m_box = currentBox().getSpawnPoint(this->m_box);
    this->m_hover = hasHover(this->m_box);
    this->m_lClicked = this->m_hover && this->ks.lClick;
    this->m_rClicked = this->m_hover && this->ks.rClick;

    return *this;
}
IMGUI& IMGUI::rect(Box r){

    this->m_box = r;
    this->m_hover = hasHover(this->m_box);
    this->m_lClicked = this->m_hover && this->ks.lClick;
    this->m_rClicked = this->m_hover && this->ks.rClick;

    this->m_buttonFlags |= NoInsertion;
    return *this;
}
IMGUI& IMGUI::rect(int x, int y, int w, int h){

    this->m_box = Box(x,y,w,h);
    this->m_hover = hasHover(this->m_box);
    this->m_lClicked = this->m_hover && this->ks.lClick;
    this->m_rClicked = this->m_hover && this->ks.rClick;
    this->m_buttonFlags |= NoInsertion;
    return *this;
}
IMGUI& IMGUI::rect(glm::vec2 xy, int w, int h){

    this->m_box = Box(xy.x,xy.y,w,h);
    this->m_hover = hasHover(this->m_box);
    this->m_lClicked = this->m_hover && this->ks.lClick;
    this->m_rClicked = this->m_hover && this->ks.rClick;
    this->m_buttonFlags |= NoInsertion;
    return *this;
}
IMGUI& IMGUI::rect(glm::vec2 xy, glm::vec2 wh){

    this->m_box = Box(xy.x,xy.y,wh.x,wh.y);
    this->m_hover = hasHover(this->m_box);
    this->m_lClicked = this->m_hover && this->ks.lClick;
    this->m_rClicked = this->m_hover && this->ks.rClick;
    this->m_buttonFlags |= NoInsertion;
    return *this;
}
IMGUI& IMGUI::rect(int w, int h){

    this->m_box = currentBox().getSpawnPoint(Box(0,0,w,h));
    this->m_hover = hasHover(this->m_box);
    this->m_lClicked = this->m_hover && this->ks.lClick;
    this->m_rClicked = this->m_hover && this->ks.rClick;
    return *this;
}
IMGUI& IMGUI::rect(int w, int h, HexColor color){
    this->m_box = currentBox().getSpawnPoint(Box(0,0,w,h));
    this->m_hover = hasHover(this->m_box);
    this->m_lClicked = this->m_hover && this->ks.lClick;
    this->m_rClicked = this->m_hover && this->ks.rClick;

    m_uiGraphic.push(Label, color, this->m_box, currentLayer);

    this->m_forceNoDraw = true;

    return *this;
}
IMGUI& IMGUI::rect(HexColor color){
    this->m_box = currentBox().getSpawnPoint(Box(0,0,20,20));
    this->m_hover = hasHover(this->m_box);
    this->m_lClicked = this->m_hover && this->ks.lClick;
    this->m_rClicked = this->m_hover && this->ks.rClick;

    m_uiGraphic.push(Label, color, this->m_box, currentLayer);

    this->m_forceNoDraw = true;

    return *this;
}
IMGUI& IMGUI::rect(Box r, HexColor color){

    this->m_box = r;
    this->m_hover = hasHover(this->m_box);
    this->m_lClicked = this->m_hover && this->ks.lClick;
    this->m_rClicked = this->m_hover && this->ks.rClick;

    m_uiGraphic.push(Label, color, this->m_box, currentLayer);

    this->m_forceNoDraw = true;
    this->m_buttonFlags |= NoInsertion;

    return *this;
}
IMGUI& IMGUI::rect(int x, int y, int w, int h, HexColor color){

    this->m_box = Box(x,y,w,h);
    this->m_hover = hasHover(this->m_box);
    this->m_lClicked = this->m_hover && this->ks.lClick;
    this->m_rClicked = this->m_hover && this->ks.rClick;

    m_uiGraphic.push(Label, color, this->m_box, currentLayer);

    this->m_buttonFlags |= NoInsertion;
    this->m_forceNoDraw = true;

    return *this;
}

IMGUI& IMGUI::setBox(const Box &box){

    this->m_box = box;
    return *this;
}
IMGUI& IMGUI::size(float x, float y){
    auto &parentBox = currentBox().m_box;
    if(x>-1 && x<1){
        x = floor(abs(parentBox.z * x));
    }
    if(y>-1 && y<1){
        y = floor(abs(parentBox.w * y));
    }
    this->m_box.z = x;
    this->m_box.w = y;

    return *this;
}
IMGUI& IMGUI::offset(float x, float y){
    float u(x),v(y);
    auto &parentBox = currentBox().m_box;
    if(x>-1 && x<1){
        u += parentBox.z*x;
    }
    if(y>-1 && y<1){
        v += parentBox.w*y;
    }
    if(x<0){
        u += parentBox.z;
    }
    if(y<0){
        v += parentBox.w;
    }

    this->m_box.x += floor(u);
    this->m_box.y += floor(v);

    this->m_hover = hasHover(this->m_box);
    this->m_lClicked = this->m_hover && this->ks.lClick;
    this->m_rClicked = this->m_hover && this->ks.rClick;

    return *this;
}

IMGUI& IMGUI::activeElement(const std::string &image){
    if(this->m_active){
        this->m_image.uvs = m_imageSet->set[image].uvs;
    }
    return *this;
}
IMGUI& IMGUI::button(bool &state){
    if(this->m_lClicked)
        state = !state;;
    this->m_active = state;
    return *this;
}

IMGUI& IMGUI::onlClick(std::function<void(void)>fun){
    if(this->m_lClicked)
        fun();
    return *this;
}
IMGUI& IMGUI::onlClick(std::function<void(Box r)>fun){
    if(this->m_lClicked)
        fun(this->m_lastBox);
    return *this;
}
IMGUI& IMGUI::onrClick(std::function<void(Box r)>fun){
    if(this->m_lClicked)
        fun(this->m_lastBox);
    return *this;
}
IMGUI& IMGUI::onrClick(std::function<void(void)>fun){
    if(this->m_rClicked)
        fun();
    return *this;
}

IMGUI& IMGUI::onRepeat(std::function<void(void)>fun){
    if(this->m_hover && this->ks.lClicked)
        fun();
    return *this;
}
IMGUI& IMGUI::onRepeat(std::function<void(void)>fun, uint32_t freq){
    if(this->m_hover && this->ks.lClicked && (timeFromstart%freq == 0))
        fun();
    return *this;
}
IMGUI& IMGUI::onRepeat(std::function<void(Box)>fun){
    if(this->m_hover && this->ks.lClicked)
        fun(this->m_lastBox);
    return *this;
}
IMGUI& IMGUI::onrRepeat(std::function<void(void)>fun){
    if(this->m_hover && this->ks.rClicked)
        fun();
    return *this;
}

IMGUI& IMGUI::onHover(std::function<void(void)>fun){
    if(this->m_hover)
        fun();
    return *this;
}
IMGUI& IMGUI::onHover(std::function<void(Box rect)>fun){
    if(this->m_hover)
        fun(this->m_lastBox);
    return *this;
}

IMGUI& IMGUI::onSpecial(std::function<void(void)>fun){
    if(this->m_special)
        fun();
    return *this;
}
IMGUI& IMGUI::onEverytime(std::function<void(Box rect)>fun){
    fun(this->m_lastBox);
    return *this;
}
IMGUI& IMGUI::onActive(std::function<void(void)>fun){
    if(this->m_active)
        fun();
    return *this;
}
IMGUI& IMGUI::onActive(std::function<void(Box rect)>fun){
    if(this->m_active)
        fun(this->m_lastBox);
    return *this;
}
IMGUI& IMGUI::onEdition(std::function<void(void)>fun){
    if(this->m_underEdition)
        fun();
    return *this;
}

IMGUI& IMGUI::text(const std::string &text, const std::string &font, int flag, int caretPosition){
    this->m_flag |= flag;
    this->m_text = text;
    this->m_caretPosition = caretPosition;
    m_textOffset += fonts[font].render(m_text, this->m_box.xy() + glm::vec2(m_textOffset+3, floor(this->m_box.w/2.f - fonts[font].height/2)), m_fontColor ? m_fontColor : m_style.font.color, this->m_caretPosition);

    if(this->m_flag & CenterText){
        fonts[font].move( (this->m_box.z - m_textOffset)/2-3, 0);
    }
    else if(this->m_flag & TextToRight){
        fonts[font].move( (this->m_box.z - m_textOffset)-3, 0);
    }
    if(LastTextHeight > 0)
        fonts[font].move( 0, LastTextHeight/2);

    return *this;
}
IMGUI& IMGUI::text(const std::string &text, int flag, int caretPosition){
    this->m_flag |= flag;
    this->m_text = text;
    this->m_caretPosition = caretPosition;
    m_textOffset += fonts[m_font].render(text, this->m_box.xy() + glm::vec2(m_textOffset+3, floor(this->m_box.w/2.f - fonts[m_font].height/2.f)), m_fontColor, this->m_caretPosition);

    if(this->m_flag & CenterText){
        fonts[m_font].move( (this->m_box.z - m_textOffset)/2-3, 0);
    }
    else if(this->m_flag & TextToRight){
        fonts[m_font].move( (this->m_box.z - m_textOffset)-5, 0);
    }
    return *this;
}
IMGUI& IMGUI::text(const std::u16string &text, const std::string &font, int flag, int caretPosition){
    this->m_flag |= flag;
    this->m_caretPosition = caretPosition;
    m_textOffset += fonts[font].render(text, this->m_box.xy() + glm::vec2(m_textOffset+3, floor(this->m_box.w/2.f - fonts[font].height/2)), m_fontColor ? m_fontColor : m_style.font.color, this->m_caretPosition);

    if(this->m_flag & CenterText){
        fonts[font].move( (this->m_box.z - m_textOffset)/2-3, 0);
    }
    else if(this->m_flag & TextToRight){
        fonts[font].move( (this->m_box.z - m_textOffset)-3, 0);
    }
    return *this;
}
IMGUI& IMGUI::text(const std::u16string &text, int flag, int caretPosition){
    this->m_flag |= flag;
    this->m_caretPosition = caretPosition;
    m_textOffset += fonts[m_font].render(text, this->m_box.xy() + glm::vec2(m_textOffset+3, floor(this->m_box.w/2.f - fonts[m_font].height/2)), m_fontColor ? m_fontColor : m_style.font.color, this->m_caretPosition);

    if(this->m_flag & CenterText){
        fonts[m_font].move( (this->m_box.z - m_textOffset)/2-3, 0);
    }
    else if(this->m_flag & TextToRight){
        fonts[m_font].move( (this->m_box.z - m_textOffset)-3, 0);
    }
    return *this;
}

IMGUI& IMGUI::color(HexColor hexColor){
    this->m_fontColor = hexColor;
    return *this;
}
IMGUI& IMGUI::boxColor(HexColor hexColor){
    this->m_color = hexColor;
    return *this;
}
IMGUI& IMGUI::font(const std::string &font){
    this->m_font = font;
    return *this;
}
IMGUI& IMGUI::getRect(Box &r){
    r = this->m_box;
    return *this;
}

IMGUI& IMGUI::mouseOffset(Box &out){
    if(this->m_hover){
        out = this->m_box - Box(ks.mousePosition, 0,0);
        out.z = 0;
        out.w = 0;
    }
    return *this;
}
IMGUI& IMGUI::mouseOffset(glm::vec2 &out){
    if(this->m_hover){
        out = this->m_box.xy() - ks.mousePosition;
    }
    return *this;
}
template<typename T>
std::string prettyPrint(T &value, int base){
    if(base == 10)
        return to_string(value);
  std::stringstream stream;

    if(base == 2)
        return "error";
    else if(base == 16)
        stream <<"0x"<< std::setfill ('0')<< std::setw(sizeof(T)*2)<< std::hex << value;
  return stream.str();
}

template<typename T>
IMGUI& IMGUI::edit(T &value, int base){
    // magoc :P
    this->m_editBox = true;
    this->m_underEdition = textEditor.compare(value);

    if(this->m_underEdition)
        printTextEditorValue();
    else
        text(std::to_string(value));

    processTE(value, base);

    return *this;

}
IMGUI& IMGUI::edit(double &value, int flag){
    // magoc :P
    this->m_editBox = true;
    this->m_underEdition = textEditor.compare(value);

    if(this->m_underEdition)
        printTextEditorValue();
    else
        text(std::to_string(value), flag);

    processTE(value);

    return *this;

}
IMGUI& IMGUI::edit(float &value, int flag){
    // magoc :P
    this->m_editBox = true;
    this->m_underEdition = textEditor.compare(value);

    if(this->m_underEdition)
        printTextEditorValue();
    else
        text(std::to_string(value), flag);

    processTE(value);

    return *this;

}
IMGUI& IMGUI::edit(std::string &value, int flag){
    // magic :P
    this->m_editBox = true;
    this->m_underEdition = textEditor.compare(value);

    if(this->m_underEdition)
        printTextEditorValue();
    else
        text(value, flag);

    processTE(value);

    return *this;

}
IMGUI& IMGUI::edit(const std::string &value, std::function<void(const std::string&)> callback, void *identifier){
    // magic :P
    this->m_editBox = true;
    this->m_underEdition = textEditor.valuePointer == identifier;

    if(this->m_underEdition)
        printTextEditorValue();
    else
        text(value, 0);

    /// --------
    if(!(this->m_underEdition && this->m_lClicked)|| this->force()){
        this->textEditor.valuePointer = identifier;
        this->textEditor.currentString = value;
        this->textEditor.m_state = true;
        this->textEditor.m_caretPosition = value.size();
        this->textEditor.m_typeInfo = TypeInfo::EMPTY;
    }
    else if(this->m_underEdition && this->ks.lClick && !this->m_hover){
        this->textEditor.breakEdition();
    }

    onEnter([this, &callback]{
        callback(this->textEditor.value());
        this->textEditor.breakEdition();
        this->m_special = true;
    });

    return *this;

}
IMGUI& IMGUI::edit(std::string *value, std::function<void(void)>callOnenter){
    return *this;
}

IMGUI& IMGUI::setFlag(int flag){
    this->m_flag |= flag;
    return *this;
}
IMGUI& IMGUI::slider(float &value, float min, float max){
    auto r = this->m_box;

    float x = r.x+3.f;
    float y = r.y+r.w/2.f - 3.f;

    float slideLen = r.z - 6.f;
    Box slide = Box(x, y, slideLen, 6.f);

    float sliderW = r.w - 4.f;
    Box slider_ = Box(
            x+slideLen*(value - min)/(max - min)-3.f
            ,r.y+2.f
            ,6.f
            ,sliderW
        );

    if(this->m_hover && ks.lClick){
        currentSlider = (void*)&value;
        this->m_underEdition = true;
    }
    if(currentSlider == (void*)&value and ks.lClicked){
        value = glm::clamp((ks.mousePosition.x - slide.x)/slide.z, 0.f, 1.f)*(max-min)+min;
        slide += Box(-1,-1,2,2);
    }
    // else currentSlider = nullptr;

    m_uiGraphic.push(UI::Hover, m_style.button.color, slide, currentLayer);
    m_uiGraphic.push(UI::Hover, m_style.button.color, slider_, currentLayer);
    return *this;
}
IMGUI& IMGUI::slider(double &value, double min, double max){
    auto r = this->m_box;

    float x = r.x+3.f;
    float y = r.y+r.w/2.f - 3.f;

    float slideLen = r.z - 6.f;
    Box slide = Box(x, y, slideLen, 6.f);

    float sliderW = r.w - 4.f;
    Box slider_ = Box(
            x+slideLen*(value - min)/(max - min)-3.f
            ,r.y+2.f
            ,6.f
            ,sliderW
        );

    if(this->m_hover && ks.lClick){
        currentSlider = (void*)&value;
        this->m_underEdition = true;
    }
    if(currentSlider == (void*)&value and ks.lClicked){
        value = glm::clamp((ks.mousePosition.x - slide.x)/slide.z, 0.f, 1.f)*(max-min)+min;
        slide += Box(-1,-1,2,2);
    }
    // else currentSlider = nullptr;

    m_uiGraphic.push(UI::Hover, m_style.button.color, slide, currentLayer);
    m_uiGraphic.push(UI::Hover, m_style.button.color, slider_, currentLayer);
    return *this;
}
IMGUI& IMGUI::slider(i64 &value, i64 min, i64 max){
    auto r = this->m_box;

    float x = r.x+3.f;
    float y = r.y+r.w/2.f - 3.f;

    float slideLen = r.z - 6.f;
    Box slide = Box(x, y, slideLen, 6.f);

    float sliderW = r.w - 4.f;
    Box slider_ = Box(
            x+slideLen*(value - min)/(max - min)-3.f
            ,r.y+2.f
            ,6.f
            ,sliderW
        );

    if(this->m_hover && ks.lClick){
        currentSlider = (void*)&value;
    }
    if(currentSlider == (void*)&value and ks.lClicked){
        value = glm::clamp((ks.mousePosition.x - slide.x)/slide.z, 0.f, 1.f)*(max-min)+min;
        slide += Box(-1,-1,2,2);
    }
    // else currentSlider = nullptr;

    m_uiGraphic.push(UI::Hover, m_style.button.color, slide, currentLayer);
    m_uiGraphic.push(UI::Hover, m_style.button.color, slider_, currentLayer);
    return *this;
}
IMGUI& IMGUI::slider(u32 &value, u32 min, u32 max){
    auto r = this->m_box;

    float x = r.x+3.f;
    float y = r.y+r.w/2.f - 3.f;

    float slideLen = r.z - 6.f;
    Box slide = Box(x, y, slideLen, 6.f);

    float sliderW = r.w - 4.f;
    Box slider_ = Box(
            x+slideLen*(value - min)/(max - min)-3.f
            ,r.y+2.f
            ,6.f
            ,sliderW
        );

    if(this->m_hover && ks.lClick){
        currentSlider = (void*)&value;
    }
    if(currentSlider == (void*)&value and ks.lClicked){
        value = glm::clamp((ks.mousePosition.x - slide.x)/slide.z, 0.f, 1.f)*(max-min)+min;
        slide += Box(-1,-1,2,2);
    }
    // else currentSlider = nullptr;

    m_uiGraphic.push(UI::Hover, m_style.button.color, slide, currentLayer);
    m_uiGraphic.push(UI::Hover, m_style.button.color, slider_, currentLayer);
    return *this;
}
IMGUI& IMGUI::slider(i32 &value, i32 min, i32 max){
    auto r = this->m_box;

    float x = r.x+3.f;
    float y = r.y+r.w/2.f - 3.f;

    float slideLen = r.z - 6.f;
    Box slide = Box(x, y, slideLen, 6.f);

    float sliderW = r.w - 4.f;
    Box slider_ = Box(
            x+slideLen*(value - min)/(max - min)-3.f
            ,r.y+2.f
            ,6.f
            ,sliderW
        );

    if(this->m_hover && ks.lClick){
        currentSlider = (void*)&value;
    }
    if(currentSlider == (void*)&value and ks.lClicked){
        value = glm::clamp((ks.mousePosition.x - slide.x)/slide.z, 0.f, 1.f)*(max-min)+min;
        slide += Box(-1,-1,2,2);
    }
    // else currentSlider = nullptr;

    m_uiGraphic.push(UI::Hover, m_style.button.color, slide, currentLayer);
    m_uiGraphic.push(UI::Hover, m_style.button.color, slider_, currentLayer);
    return *this;
}
IMGUI& IMGUI::operator () (int flags){
    if(this->m_lClicked || this->m_rClicked)
        this->m_box += Box(-1,-1,2,2);

    // if((flags & UI::Hover) && this->m_hover)
    //      m_uiGraphic.push(UI::Hover, m_style.hover, this->m_box, currentLayer);
    // else if(flags & UI::Editable)
    //      m_uiGraphic.push(UI::Editable, m_style.editBox, this->m_box, currentLayer);
    // else if(flags & UI::Label)
    //      m_uiGraphic.push(UI::Label, m_style.button, this->m_box, currentLayer);
    if (!(m_buttonFlags & NoInsertion))
         currentBox().insertRect(this->m_box);

    // m_uiGraphic.push(flags, m_style, this->m_color, this->m_box, currentLayer);
    m_uiGraphic.push(flags, m_style, this->m_color, this->m_box, currentLayer);

    if(this->m_imageEnbl){
        // this->m_image.color = this->m_color;
        // this->m_image.color = this->m_forceColor ? m_color : m_style.imageColor;
        this->m_image.color = this->m_color ? this->m_color : m_style.image.color;
        m_uiGraphic.push(UI::Image, this->m_image);
    }
    if (flags & UI::CaptureMouse){
        if(this->m_lClicked || this->m_rClicked){
            this->ks.lClick = false;
            this->ks.rClick = false;
            this->ks.mousePosition = glm::vec2(-500, -500);
            this->captureMouse = true;
        }
        if(this->m_lClicked&& this->m_hover){
            this->captureMouse = true;
        }
    }
    restoreDefaults();
    return *this;
}
void IMGUI::restoreDefaults(){
    m_buttonFlags = 0;
    m_imageEnbl = false;
    m_text = "";
    m_caretPosition = -1;
    // m_color = m_style.background;
    // m_style = m_boxStack[m_boxIndex].m_style;
    m_font = m_style.font.name;
    m_fontColor = m_style.font.color;
    m_color = 0;
    // m_fontColor = 0;
    m_lastBox = m_box;
    //m_box = Box(0, 0, 0, 0);
    m_box = Box(0, 0, 120, 24);
    m_flag = 0;
    m_special = false;
    m_editBox = false;
    m_forceColor = false;
    m_forceNoDraw = false;
    m_textOffset = 0;
    m_underEdition = false;
}

// ------------------------------------------------------------
bool IMGUI::overlay(Box &rect, Box &point){
    return     point.x >= rect.x && point.x <= rect.x+rect.z &&
                    point.y >= rect.y && point.y <= rect.y+rect.w;
}
bool IMGUI::hasHover(Box rect){
    if(currentLayer == maxLayer)
        return     (ks.mousePosition.x >= rect.x && ks.mousePosition.x < rect.x+rect.z &&
                    ks.mousePosition.y >= rect.y && ks.mousePosition.y < rect.y+rect.w) && !captureMouse;
    return false;
}
bool IMGUI::outOfTable(){
    auto &&rect = fixRect(m_boxStack[m_boxIndex+1].m_box);
    // if(currentLayer == maxLayer)
        return this->ks.lClick && !(ks.mousePosition.x >= rect.x && ks.mousePosition.x < rect.x+rect.z &&
                    ks.mousePosition.y >= rect.y && ks.mousePosition.y < rect.y+rect.w);


}
Box IMGUI::fixRect(Box rect){
    if(rect.z < 0.f)
        rect = Box(rect.x+rect.z, rect.y, -rect.z, rect.w);
    if(rect.w < 0.f)
        rect = Box(rect.x, rect.y+rect.w, rect.z, -rect.w);

    return rect;
}
// ------------------------------------------------------------

void IMGUI::begin(){
    draw = true;
    m_boxIndex = 0;
    layerToDraw = 0;
    tmpMaxLayer = 0;
    currentLayer = 0;
    m_force = false;
    m_boxStack[0].m_box = Box(0,0,m_maxHorizontal,m_maxVertical);
    m_collRects.clear();
    this->captureMouse = false;
    if(ks.lClicked)
        accu = 0.f;
    if(accu > 1/frequency){
        accu = 0.f;
        counter = true;
    }
    if(accu2 > 0.5f){
        accu2 = 0.f;
        MS500OSCILATOR = !MS500OSCILATOR;
    }
    if(not ks.lClicked) currentSlider = nullptr;
    restoreDefaults();
}
void IMGUI::end(){
    if(m_boxIndex)
        error("box index error");
    m_key = -1;
    m_mouseKey = -1;
    m_force = false;
    drawLayer = layerToDraw;
    maxLayer = tmpMaxLayer;
    counter = false;
    m_collRects.clear();
}

IMGUIBox& IMGUI::table(int flags){
    auto spawnPosition = placeGroup(flags);
    spawnPosition.z = 0;
    spawnPosition.w = 0;
    return table(spawnPosition, flags);
}
IMGUIBox& IMGUI::table(Box spawnPosition, int flags){
    m_group.m_box = spawnPosition;
    m_group.m_border = 0;
    m_group.m_flags = flags;
    m_group.m_currStart = m_group.m_box;
    //m_group.m_currStart.x = 0;
    //m_group.m_currStart.y = 0;
    m_group.m_freeRect = fixRect(spawnPosition); // czy to jest do czego u¿ywane?, nie
    // m_boxStack[0] = m_group;
    spawnPosition.z = 0;
    spawnPosition.w = 0;
    return m_boxStack[++m_boxIndex].box(flags, spawnPosition, this);
}
IMGUIBox& IMGUI::endTable(){
    endBox();
    m_fontSize = m_defaultFontSize;
    m_font = m_defaultFont;
    m_indentation = 0;
    IMGUIBox &group = m_boxStack[m_boxIndex+1];
    float b = group.m_border;

    Box rect = group.m_box;
    if((group.m_flags & FixedSize) != FixedSize){
        if(group.m_flags & LayoutVertical)
            rect.w = (abs(group.m_box.y - group.m_currStart.y)+b)*sign(rect.w);

        else if(group.m_flags & LayoutHorizontal)
            rect.z = (abs(group.m_box.x - group.m_currStart.x)+b)*sign(rect.z);
    }

    rect = fixRect(rect);
    if((group.m_flags & AbsolutePosition) != AbsolutePosition)
        m_collRects.push_back(rect);
    return group;
}

IMGUIBox& IMGUI::box(int flags){
    Box r = Box(0,0,0,0);
    r = m_boxStack[m_boxIndex].getSpawnPoint(r);
    return m_boxStack[++m_boxIndex].box(flags, r, this);
}
IMGUIBox& IMGUIBox::box(int flags, Box spawnPosition, IMGUI *_imgui){
    imgui = _imgui;
    int parentFlags = (imgui->m_group.m_flags | ClearLayout) & ~ClearLayout;

    m_box = spawnPosition;
    m_border = 0;
    m_flags = flags | parentFlags;
    m_currStart = m_box;
    m_currStart.z = 0;
    m_currStart.w = 0;

    if(m_flags & Draw){
        m_rectIdx = imgui->m_uiGraphic.size(UI::BigBox);
        imgui->m_uiGraphic.push(UI::BigBox, imgui->m_style.background.color, spawnPosition, _imgui->currentLayer);
    }

    return *this;
}
IMGUIBox& IMGUIBox::size(float x, float y){
    // if(m_flags & AlignTop)
        // y *= -1;
    // if(m_flags & AlignRight)
        // x *= -1;
    auto &parentBox = imgui->parentBox().m_box;
    if(x>-1 && x<1){
        x = floor(parentBox.z * x);
    }
    if(y>-1 && y<1){
        y = floor(parentBox.w * y);
    }
    m_box.z = x;
    m_box.w = y;
    m_flags |= FixedSize;

    return *this;
}
IMGUIBox& IMGUIBox::overridePosition(float x, float y){
    float u(x),v(y);
    auto &parentBox = imgui->parentBox().m_box;

    m_flags |= AbsolutePosition;

    m_currStart.x = x;
    m_currStart.y = y;
    m_box.x = x;
    m_box.y = y;
    return *this;
}
IMGUIBox& IMGUIBox::position(float x, float y){
    m_flags |= AbsolutePosition;
    float u(x),v(y);
    auto &parentBox = imgui->parentBox().m_box;
    // auto &parent = imgui->parentBox().m_currStart;
    if(x>-1 && x<1){
        u = floor(parentBox.z*x);
    }
    if(y>-1 && y<1){
        v = floor(parentBox.w*y);
    }
    // if(x<0){
        // u += parentBox.z;
    // }
    // if(y<0){

        // v += parentBox.w;
    // }
    m_currStart.x = parentBox.x + u;
    m_currStart.y = parentBox.y + v;
    m_box.x = parentBox.x + u;
    m_box.y = parentBox.y + v;
    return *this;
}
IMGUIBox& IMGUIBox::offset(float x, float y){
    m_flags |= RelativePosition;
    float u(x),v(y);
    auto &parentBox = imgui->parentBox().m_box;
    // auto &parent = imgui->parentBox().m_currStart;
    if(x>-1 && x<1){
        u += floor(parentBox.z*x);
    }
    if(y>-1 && y<1){
        v += floor(parentBox.w*y);
    }
    if(x<0){
        u += parentBox.z;
    }
    if(y<0){
        v += parentBox.w;
    }
    m_currStart.x += u;
    m_currStart.y += v;
    m_box.x += u;
    m_box.y += v;
    return *this;
}
IMGUIBox& IMGUIBox::border(int b){
    m_border = b;
    return *this;
}
IMGUIBox& IMGUIBox::operator()(){
    return *this;
}

IMGUIBox& IMGUI::endBox(){
    IMGUIBox &group = m_boxStack[m_boxIndex];
    float b = group.m_border;

    Box rect = group.m_box;
    // if(!(group.m_flags & FixedSize)){
        if(group.m_flags & LayoutVertical)
            rect.w = std::max((abs(group.m_box.y - group.m_currStart.y)+b), std::abs(group.m_box.w))*sign(rect.w);

        else if(group.m_flags & LayoutHorizontal)
            rect.z = std::max((abs(group.m_box.x - group.m_currStart.x)+b), std::abs(group.m_box.z))*sign(rect.z);
    // }

    rect = fixRect(rect);

    if(group.m_flags & Draw){
        m_uiGraphic.change(UI::BigBox, group.m_rectIdx, m_style.background.color, rect);
    }

    m_boxStack[m_boxIndex].m_box = rect;
    m_boxIndex--;
    if(!(m_boxStack[m_boxIndex+1].m_flags & AbsolutePosition))
        m_boxStack[m_boxIndex].insertRect(rect+Box(-1,-1,0.5f,1));
    return group;
}
IMGUIBox& IMGUIBox::onGroupHover(std::function<void(Box rect)>fun){
    imgui->onGroupHover(fun);
    return *this;
}
IMGUIBox& IMGUIBox::onGrouplClick(std::function<void(Box rect)>fun){
    imgui->onGrouplClick(fun);
    return *this;
}
bool IMGUIBox::onGroupGrab(std::function<void(Box rect)>fun){
    return imgui->onGroupGrab(fun);
}

Box IMGUIBox::getSpawnPoint(const Box &r){
    Box rect(r);

    float border = m_border;
    float h = r.w;
    float w = r.z;

    if(m_flags & LayoutVertical){
        float mod;
        if(m_flags & AlignTop)
            mod = -1.f;
        else if(m_flags & AlignBottom)
            mod = 1.f;

        if(m_flags & AlignLeft){
            rect = m_currStart + Box(border,mod*border,w, h);
        }
        else if(m_flags & AlignRight){
            rect = m_currStart + Box(-border-w,mod*border,w, h);
        }
        if(mod == -1.f){
            rect = Box(rect.x, rect.y - rect.w, rect.z, rect.w);
        }
    }

    else if(m_flags & LayoutHorizontal){
        float mod;
        if(m_flags & AlignLeft)
            mod = 1.f;
        else if(m_flags & AlignRight)
            mod = -1.f;

        if(m_flags & AlignBottom){
            rect = m_currStart + Box(mod*border,1*border,w, h);
        }
        else if(m_flags & AlignTop){
            rect = m_currStart + Box(mod*border,-1*border-h,w, h);
        }
        if(mod == -1.f){
            rect = Box(rect.x-rect.z, rect.y, rect.z, rect.w);
        }
    }

    return glm::ceil(rect);
}
Box IMGUIBox::insertRect(const Box &r){ // r is fixed
    Box rect;

    glm::vec2 r1 =  m_currStart.xy() - r.xy();
    glm::vec2 r2 =  m_currStart.xy() - (r.xy() + r.zw());
    float border = 0;
    // float w = r.z;
    // float h = r.w;
    float w = std::max(abs(r1.x), abs(r2.x))+1;
    float h = std::max(abs(r1.y), abs(r2.y))+1;

    if(m_flags & LayoutVertical){

        float mod = 0;
        if(m_flags & AlignTop)
            mod = -1.f;
        else if(m_flags & AlignBottom)
            mod = 1.f;

        if(m_flags & AlignLeft){
            m_box.z = std::max(m_box.z, w+2*border);
        }
        else if(m_flags & AlignRight){
            m_box.z = std::min(m_box.z, -w-2*border);
        }

        m_currStart += Box(0,(border + h-1)*mod,0,0);

        if(!(m_flags & FixedSize))
            m_box += Box(0,0,0,(border + h-1)*mod);

    }

    else if(m_flags & LayoutHorizontal){
        float mod=1.f;
        if(m_flags & AlignLeft)
            mod = 1.f;
        else if(m_flags & AlignRight)
            mod = -1.f;

        if(m_flags & AlignBottom){
            m_box[3] = std::max(m_box[3], h+2*border);

        }
        else if(m_flags & AlignTop){
            m_box[3] = std::min(m_box[3], -h-2*border);
        }

        m_currStart += Box((border + w-1)*mod,0,0,0);

        if(!(m_flags & FixedSize))
            m_box += Box(0,0,(border + w-1)*mod,0);
    }

    return rect;
}
Box IMGUIBox::placeRect(const Box &r){
    auto rect = getSpawnPoint(r);
    insertRect(rect);
    return rect;
}

bool IMGUI::findCollision(Box &r, Box &out){
    for(auto &it : m_collRects){

        if(overlay(it, r)){
            out = it;
            return true;
        }
    }
    return false;
}
Box IMGUI::placeGroup(int flags){
    Box pos, out;

    Box m_freeRect = Box(0,0,m_maxHorizontal,m_maxVertical);

    if(flags & LayoutVertical){
        if(flags & AlignLeft && flags & AlignTop){
            pos = Box(m_freeRect.x, m_freeRect.y+m_freeRect.w,0,0);
            while(findCollision(pos, out)){
                if(out.y + out.w < 300)
                    pos += Box(out.z+1,0,0,0);
                else
                    pos -= Box(0,out.w+1,0,0);

            }
            pos.z = m_freeRect.z - pos.x + m_freeRect.x;
            pos.w = m_freeRect.y - pos.y;
        }
        else if(flags & AlignRight && flags & AlignTop){
            pos = Box(m_freeRect.x+m_freeRect.z, m_freeRect.y+m_freeRect.w,0,0);
            while(findCollision(pos, out)){
                // if(out.y + out.w < 300)
                if(out.z < out.w)
                    pos -= Box(out.z+1,0,0,0);
                else
                    pos -= Box(0,out.w+1,0,0);

            }
            pos.z = m_freeRect.x - pos.x;
            pos.w = m_freeRect.y - pos.y;
        }
        else if(flags & AlignLeft && flags & AlignBottom){
            pos = Box(m_freeRect.x, m_freeRect.y,0,0);
            while(findCollision(pos, out)){
                if(out.z < out.w)
                // if(out.y + out.w > group.m_freeRect.w/2)
                    pos += Box(out.z+1,0.f,0.f,0.f);
                else
                    pos += Box(0.f,out.w+1,0.f,0.f);
            }
            pos.z = m_freeRect.x+m_freeRect.z - pos.x;
            pos.w = m_freeRect.y+m_freeRect.w - pos.y;
        }
        else if(flags & AlignRight && flags & AlignBottom){
            pos = Box(m_freeRect.x+m_freeRect.z, m_freeRect.y,0,0);
            while(findCollision(pos, out)){
                if(out.y + out.w > 250)
                    pos -= Box(out.z+1,0,0,0);
                else
                    pos += Box(0,out.w+1,0,0);

            }
            pos.z = m_freeRect.x - pos.x;
            pos.w = m_freeRect.y+m_freeRect.w - pos.y;
        }


    }

    else if(flags & LayoutHorizontal){

        if(flags & AlignLeft && flags & AlignTop){
            pos = Box(m_freeRect.x, m_freeRect.y+m_freeRect.w,0,0);
            while(findCollision(pos, out)){
                if(out.x + out.z > 300)
                    pos -= Box(0,out.w+1,0,0);
                else
                    pos += Box(out.z+1,0,0,0);

            }
            pos.z = m_freeRect.z - pos.x + m_freeRect.x;
            pos.w = m_freeRect.y - pos.y;
        }
        else if(flags & AlignRight && flags & AlignTop){
            pos = Box(m_freeRect.x+m_freeRect.z, m_freeRect.y+m_freeRect.w,0,0);
            while(findCollision(pos, out)){
                if(out.w < out.z){
                    pos -= Box(0,out.w+1,0,0);
                    }
                else{
                    pos -= Box(out.z+1,0,0,0);
                    }

            }
            pos.z = m_freeRect.x - pos.x;
            pos.w = m_freeRect.y - pos.y;
        }
        else if(flags & AlignLeft && flags & AlignBottom){
            pos = Box(m_freeRect.x, m_freeRect.y,0,0);
            while(findCollision(pos, out)){
                if(out.x + out.z > 300)
                    pos += Box(0.f,out.w+1,0.f,0.f);
                else
                    pos += Box(out.z+1,0.f,0.f,0.f);

            }
            pos.z = m_freeRect.x+m_freeRect.z - pos.x;
            pos.w = m_freeRect.y+m_freeRect.w - pos.y;
        }
        else if(flags & AlignRight && flags & AlignBottom){
            pos = Box(m_freeRect.x+m_freeRect.z, m_freeRect.y,0,0);
            while(findCollision(pos, out)){
                if(out.x + out.z < 300)
                    pos += Box(0,out.w+1,0,0);
                else
                    pos -= Box(out.z+1,0,0,0);

            }
            pos.z = m_freeRect.x - pos.x;
            pos.w = m_freeRect.y+m_freeRect.w - pos.y;
        }




    }

    return pos;
}

void IMGUI::printTextEditorValue(){
        // this->m_text += this->textEditor.value();
        if(MS500OSCILATOR)
            this->m_caretPosition = this->m_text.size() + textEditor.caretPosition();
        text(this->textEditor.value(),0,this->m_caretPosition);
}
template<typename T>
void IMGUI::processTE(T &value, int base){
    if(!this->m_underEdition && this->m_lClicked || this->force()){
        this->textEditor.setValueToEdit(value, base);
    }
    else if(this->m_underEdition && this->ks.lClick && !this->m_hover){
        this->textEditor.breakEdition();
    }

    onEnter([this]{
        this->textEditor.finishEdition();
        this->m_special = true;
    });
}
template<typename T>
void IMGUI::processTE(T &value){
    if(!(this->m_underEdition && this->m_lClicked)|| this->force()){
        this->textEditor.setValueToEdit(value);
    }
    else if(this->m_underEdition && this->ks.lClick && !this->m_hover){
        this->textEditor.breakEdition();
    }

    onEnter([this]{
        this->textEditor.finishEdition();
        this->m_special = true;
    });
}


TextEditor::TextEditor(){
    m_caretPosition = 0;
    m_typeInfo = TypeInfo::EMPTY;
}


template<typename T>
void TextEditor::setValueToEdit(T &value, int base){
    valuePointer = (void*)&value;
    // currentString = std::to_string(value);
    currentString = prettyPrint(value, base);
    m_state = true;
    m_caretPosition = currentString.size();
    m_typeInfo = recognizeType<T>();
    m_base = base;
}
template<typename T>
void TextEditor::setValueToEdit(T &value){
    valuePointer = (void*)&value;
    currentString = std::to_string(value);
    m_state = true;
    m_caretPosition = currentString.size();
    m_typeInfo = TypeInfo::INT32_t;
    m_base = 10;
}
template<>
void TextEditor::setValueToEdit<double>(double &value){
    valuePointer = (void*)&value;
    currentString = std::to_string(value);
    m_state = true;
    m_caretPosition = currentString.size();
    m_typeInfo = TypeInfo::DOUBLE;
}
template<>
void TextEditor::setValueToEdit<float>(float &value){
    valuePointer = (void*)&value;
    currentString = std::to_string(value);
    m_state = true;
    m_caretPosition = currentString.size();
    m_typeInfo = TypeInfo::FLOAT;
}
template<>
void TextEditor::setValueToEdit<std::string>(std::string &value){
    valuePointer = (void*)&value;
    currentString = value;
    m_state = true;
    m_caretPosition = currentString.size();
    m_typeInfo = TypeInfo::STRING;
}
bool TextEditor::state(){
    return m_state;
}
std::string TextEditor::value(){
    return currentString;
}
void TextEditor::finishEdition(){
    m_state = false;
    if(m_typeInfo == TypeInfo::INT32_t)
        *(int32_t*)valuePointer = std::stoi(currentString,nullptr, m_base);
    if(m_typeInfo == TypeInfo::UINT32_t)
        *(uint32_t*)valuePointer = std::stoi(currentString,nullptr, m_base);
    if(m_typeInfo == TypeInfo::INT64_t)
        *(int64_t*)valuePointer = std::stoi(currentString,nullptr, m_base);
    if(m_typeInfo == TypeInfo::UINT64_t)
        *(uint64_t*)valuePointer = std::stoi(currentString,nullptr, m_base);
    else if(m_typeInfo == TypeInfo::FLOAT)
        *(float*)valuePointer = parseExpr(currentString);
    else if(m_typeInfo == TypeInfo::DOUBLE){
        *(double*)valuePointer = std::stod(currentString);
    }
    else if(m_typeInfo == TypeInfo::STRING){
        // swap(*(std::string*)valuePointer, currentString);
        *(std::string*)valuePointer = currentString;
    }
    m_typeInfo = TypeInfo::EMPTY;
    valuePointer = nullptr;
}
void TextEditor::breakEdition(){
    m_typeInfo = TypeInfo::EMPTY;
    m_state = false;
    valuePointer = nullptr;
}
int TextEditor::caretPosition(){
    return m_caretPosition;
}
void TextEditor::input(u32 key, u32 action, u32 mod){
    char input = 0;

    if(key == GLFW_KEY_ENTER && action == GLFW_PRESS && !GetInput)
        GetInput = true;
    else if(GetInput && (action == GLFW_PRESS || action == GLFW_REPEAT)){

        // if(key == GLFW_KEY_ENTER)
            // finishEdition();
        if(key == GLFW_KEY_ESCAPE)
            breakEdition();

        if(mod == GLFW_MOD_CONTROL ){
            if(key == GLFW_KEY_DELETE  && u32(m_caretPosition)<currentString.size()){
                currentString.erase(m_caretPosition);
                m_caretPosition = glm::clamp(m_caretPosition, 0, (int)currentString.size());
            }
            if(key == GLFW_KEY_BACKSPACE && m_caretPosition>0){
                currentString.erase(0, m_caretPosition);
                m_caretPosition = 0;
                m_caretPosition = glm::clamp(m_caretPosition, 0, (int)currentString.size());
            }
            // unsigned found = str.rfind(key);
            // if (found!=std::string::npos)
                // str.replace (found,key.length(),"seventh");
            if(key == GLFW_KEY_RIGHT)
                m_caretPosition = currentString.size();
            if(key == GLFW_KEY_LEFT)
                m_caretPosition = 0;
        }
        else {
            if(key == GLFW_KEY_DELETE  && u32(m_caretPosition)<currentString.size()){
                currentString.erase(m_caretPosition,1);
                m_caretPosition = glm::clamp(m_caretPosition, 0, (int)currentString.size());
            }
            if(key == GLFW_KEY_BACKSPACE && m_caretPosition>0){
                m_caretPosition -= 1;
                currentString.erase(m_caretPosition,1);
                m_caretPosition = glm::clamp(m_caretPosition, 0, (int)currentString.size());
            }
            if(key == GLFW_KEY_RIGHT)
                m_caretPosition++;
            if(key == GLFW_KEY_LEFT)
                m_caretPosition--;
        }
        if(mod != GLFW_MOD_SHIFT && key<=127){
            if(key>='A' && key<='Z')
                input = key+32;
            else
                input = key;
        }
        //SHIFTEd
        if(mod == GLFW_MOD_SHIFT && key<=127){
            if(key>='A' && key<='Z')
                input = key;//a->A

            else switch(key){
                case '1':    input = '!'; break;
                case '2':    input = '@'; break;
                case '3':    input = '#'; break;
                case '4':    input = '$'; break;
                case '5':    input = '%'; break;
                case '6':    input = '^'; break;
                case '7':    input = '&'; break;
                case '8':    input = '*'; break;
                case '9':    input = '('; break;
                case '0':    input = ')'; break;
                case '[':    input = '{'; break;
                case ']':    input = '}'; break;
                case ';':    input = ':'; break;
                case '\'':input = '"'; break;
                case ',':    input = '<'; break;
                case '.':    input = '>'; break;
                case '/': input = '?'; break;
                case '-':    input = '_'; break;
                case '=':    input = '+'; break;
                case '`':    input = '~'; break;
            }
        }

        // numeric key
        if(key>=GLFW_KEY_KP_0 && key<=GLFW_KEY_KP_ADD ){
            switch(key){
                case GLFW_KEY_KP_0:    input = '0'; break;
                case GLFW_KEY_KP_1:    input = '1'; break;
                case GLFW_KEY_KP_2:    input = '2'; break;
                case GLFW_KEY_KP_3:    input = '3'; break;
                case GLFW_KEY_KP_4:    input = '4'; break;
                case GLFW_KEY_KP_5:    input = '5'; break;
                case GLFW_KEY_KP_6:    input = '6'; break;
                case GLFW_KEY_KP_7:    input = '7'; break;
                case GLFW_KEY_KP_8:    input = '8'; break;
                case GLFW_KEY_KP_9:    input = '9'; break;

                case GLFW_KEY_KP_DECIMAL:    input = ','; break;
                case GLFW_KEY_KP_DIVIDE:    input = '/'; break;
                case GLFW_KEY_KP_MULTIPLY:    input = '*'; break;
                case GLFW_KEY_KP_SUBTRACT:    input = '-'; break;
                case GLFW_KEY_KP_ADD :    input = '+'; break;
            }
        }
        m_caretPosition = glm::clamp(m_caretPosition, 0, (int)currentString.size());
        if(input){
            // if((m_typeInfo == INT32_t && input >='0' && input<='9') || (m_typeInfo == FLOAT && ((input>='0' && input<='9') || input =='.' || input ==',') ) || m_typeInfo == STRING){
            if(true){

                currentString.insert(m_caretPosition, &input, 1);
                m_caretPosition++;
            }
        }
    }

}
template<typename T>
bool TextEditor::compare(T &value){
    return &value == this->valuePointer;
}

float TextEditor::parseExpr(std::string &str){
    if(str == "pi")
        return pi;
    else if(str == "pi/2")
        return pi/2;
    else if(str == "pi/4")
        return pi/4;
    else if(str == "2pi")
        return pi*2;
    else if(str == "-pi")
        return -pi;
    else if(str == "-pi/2")
        return -pi/2;
    else if(str == "-pi/4")
        return -pi/4;
    else if(str == "-2pi")
        return -pi*2;

    return stof(str);

}


template<>
TypeInfo recognizeType<int32_t>(){
    return TypeInfo::INT32_t;
}
template<>
TypeInfo recognizeType<uint32_t>(){
    return TypeInfo::UINT32_t;
}
template<>
TypeInfo recognizeType<int64_t>(){
    return TypeInfo::INT64_t;
}
template<>
TypeInfo recognizeType<uint64_t>(){
    return TypeInfo::UINT64_t;
}
template<>
TypeInfo recognizeType<std::string>(){
    return TypeInfo::STRING;
}
template<>
TypeInfo recognizeType<float>(){
    return TypeInfo::FLOAT;
}
template<>
TypeInfo recognizeType<double>(){
    return TypeInfo::DOUBLE;
}

void loadStyles(IMGUI &ui){
    const Yaml &styles = loadYaml("../styles.yml");
    auto LoadStyle = [&styles, &ui](int idx, std::string styleName){
        auto &cfg = styles["Styles"][styleName];
        ui.m_style = UI::Style {};
        ui.m_style.background.color = cfg["background"].color();
        ui.m_style.button.color     = cfg["button"].color();
        ui.m_style.button.hovered     = cfg["hover"].color();
        ui.m_style.editbox.color    = cfg["editBox"].color();
        ui.m_style.font.color  = cfg["fontColor"].color();
        ui.m_style.image.color = cfg["imageColor"].color();
        ui.m_style.label.color      = cfg["label"].color();
        ui.m_style.font.name = cfg["font"].string();
    };
    LoadStyle(0, "Basic");
}
}
