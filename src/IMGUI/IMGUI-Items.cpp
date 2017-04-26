#include "IMGUI.hpp"
#include "Yaml.hpp"
#include "Assets.hpp"

namespace UI {
IMGUI& IMGUI::circleShape(const Box &circle){

    item.box = Box(circle.x, circle.y, 20, 20);
    item.hover = (glm::distance2(circle.xy(), updater.mb.mousePosition) < circle.z*circle.z) && !captureMouse;
    item.lClicked = item.hover && updater.mb.lmbPress && !captureMouse;
    item.rClicked = item.hover && updater.mb.rmbPress && !captureMouse;

    return *this;
}
IMGUI& IMGUI::rotatedBoxShape(Box &startEnd, float distance, float &proportion){
    bool hovered = false;
    float lengthSquared = glm::distance2(startEnd.xy(), startEnd.zw());

    float t = glm::dot(updater.mb.mousePosition-startEnd.xy(), startEnd.zw() - startEnd.xy())/lengthSquared;
    proportion = t;

    if(t < 0.f)
            hovered = glm::distance2(updater.mb.mousePosition, startEnd.xy()) < distance*distance;
    else if(t > 1.f)
            hovered = glm::distance2(updater.mb.mousePosition, startEnd.xy()) < distance*distance;
    else {
            glm::vec2 projection = startEnd.xy() + t*(startEnd.zw() - startEnd.xy());
            hovered = glm::distance2(updater.mb.mousePosition, projection) < distance*distance;
    }

    item.box = Box((startEnd.x + startEnd.z)/2, (startEnd.y + startEnd.w)/2, 20, 20);
    item.lClicked = hovered && updater.mb.lmbPress && !captureMouse;
    item.rClicked = hovered && updater.mb.rmbPress && !captureMouse;

    return *this;
}
IMGUI& IMGUI::customShape(std::function<bool(glm::vec2)> &fun){
    bool hovered = fun(updater.mb.mousePosition);

    item.box = Box(0,0,0,0);
    item.lClicked = hovered && updater.mb.lmbPress && !captureMouse;
    item.rClicked = hovered && updater.mb.rmbPress && !captureMouse;

    return *this;
}
IMGUI& IMGUI::customShape(bool hovered){

    item.box = Box(0,0,0,0);
    item.lClicked = hovered && updater.mb.lmbPress && !captureMouse;
    item.rClicked = hovered && updater.mb.rmbPress && !captureMouse;

    return *this;
}
IMGUI& IMGUI::image(const std::string &name){
    item.icon = m_imageSet->set[name];
    item.box = currentBox().getSpawnPoint(item.icon.rect);
    item.icon.rect = item.box;
    item.hover = hasHover(item.box) && !captureMouse;
    item.lClicked = item.hover && updater.mb.lmbPress;
    item.rClicked = item.hover && updater.mb.rmbPress;
    item.imageEnabled = true;
    return *this;
}
IMGUI& IMGUI::image(int x, int y, const std::string &name){
    item.icon = m_imageSet->set[name];
    item.icon.rect[0] = x;
    item.icon.rect[1] = y;
    item.box = item.icon.rect;

    item.hover = hasHover(item.box);
    item.lClicked = item.hover && updater.mb.lmbPress;
    item.rClicked = item.hover && updater.mb.rmbPress;
    item.imageEnabled = true;
    item.buttonFlags |= NoInsertion;
    return *this;
}
IMGUI& IMGUI::rect(){

    item.box = currentBox().getSpawnPoint(item.box);
    item.hover = hasHover(item.box);
    item.lClicked = item.hover && updater.mb.lmbPress;
    item.rClicked = item.hover && updater.mb.rmbPress;

    return *this;
}
IMGUI& IMGUI::rect(Box r){

    item.box = r;
    item.hover = hasHover(item.box);
    item.lClicked = item.hover && updater.mb.lmbPress;
    item.rClicked = item.hover && updater.mb.rmbPress;

    item.buttonFlags |= NoInsertion;
    return *this;
}
IMGUI& IMGUI::rect(int x, int y, int w, int h){

    item.box = Box(x,y,w,h);
    item.hover = hasHover(item.box);
    item.lClicked = item.hover && updater.mb.lmbPress;
    item.rClicked = item.hover && updater.mb.rmbPress;
    item.buttonFlags |= NoInsertion;
    return *this;
}
IMGUI& IMGUI::rect(glm::vec2 xy, int w, int h){

    item.box = Box(xy.x,xy.y,w,h);
    item.hover = hasHover(item.box);
    item.lClicked = item.hover && updater.mb.lmbPress;
    item.rClicked = item.hover && updater.mb.rmbPress;
    item.buttonFlags |= NoInsertion;
    return *this;
}
IMGUI& IMGUI::rect(glm::vec2 xy, glm::vec2 wh){

    item.box = Box(xy.x,xy.y,wh.x,wh.y);
    item.hover = hasHover(item.box);
    item.lClicked = item.hover && updater.mb.lmbPress;
    item.rClicked = item.hover && updater.mb.rmbPress;
    item.buttonFlags |= NoInsertion;
    return *this;
}
IMGUI& IMGUI::rect(int w, int h){

    item.box = currentBox().getSpawnPoint(Box(0,0,w,h));
    item.hover = hasHover(item.box);
    item.lClicked = item.hover && updater.mb.lmbPress;
    item.rClicked = item.hover && updater.mb.rmbPress;
    return *this;
}
IMGUI& IMGUI::rect(int w, int h, HexColor color){
    item.box = currentBox().getSpawnPoint(Box(0,0,w,h));
    item.hover = hasHover(item.box);
    item.lClicked = item.hover && updater.mb.lmbPress;
    item.rClicked = item.hover && updater.mb.rmbPress;

    m_uiGraphic.push(Label, color, item.box, currentLayer);

    item.noDraw = true;

    return *this;
}
IMGUI& IMGUI::rect(HexColor color){
    item.box = currentBox().getSpawnPoint(Box(0,0,20,20));
    item.hover = hasHover(item.box);
    item.lClicked = item.hover && updater.mb.lmbPress;
    item.rClicked = item.hover && updater.mb.rmbPress;

    m_uiGraphic.push(Label, color, item.box, currentLayer);

    item.noDraw = true;

    return *this;
}
IMGUI& IMGUI::rect(Box r, HexColor color){

    item.box = r;
    item.hover = hasHover(item.box);
    item.lClicked = item.hover && updater.mb.lmbPress;
    item.rClicked = item.hover && updater.mb.rmbPress;

    m_uiGraphic.push(Label, color, item.box, currentLayer);

    item.noDraw = true;
    item.buttonFlags |= NoInsertion;

    return *this;
}
IMGUI& IMGUI::rect(int x, int y, int w, int h, HexColor color){

    item.box = Box(x,y,w,h);
    item.hover = hasHover(item.box);
    item.lClicked = item.hover && updater.mb.lmbPress;
    item.rClicked = item.hover && updater.mb.rmbPress;

    m_uiGraphic.push(Label, color, item.box, currentLayer);

    item.buttonFlags |= NoInsertion;
    item.noDraw = true;

    return *this;
}

IMGUI& IMGUI::setBox(const Box &box){

    item.box = box;
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
    item.box.z = x;
    item.box.w = y;

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

    item.box.x += floor(u);
    item.box.y += floor(v);

    item.hover = hasHover(item.box);
    item.lClicked = item.hover && updater.mb.lmbPress;
    item.rClicked = item.hover && updater.mb.rmbPress;

    return *this;
}

IMGUI& IMGUI::activeElement(const std::string &image){
    if(item.active){
        item.icon.uvs = m_imageSet->set[image].uvs;
    }
    return *this;
}
IMGUI& IMGUI::button(bool &state){
    if(item.lClicked)
        state = !state;;
    item.active = state;
    return *this;
}

IMGUI& IMGUI::onLMB(std::function<void(void)>fun){
    if(item.lClicked)
        fun();
    return *this;
}
IMGUI& IMGUI::onLMB(std::function<void(Box r)>fun){
    if(item.lClicked)
        fun(item.lastBox);
    return *this;
}
IMGUI& IMGUI::onRMB(std::function<void(Box r)>fun){
    if(item.lClicked)
        fun(item.lastBox);
    return *this;
}
IMGUI& IMGUI::onRMB(std::function<void(void)>fun){
    if(item.rClicked)
        fun();
    return *this;
}

IMGUI& IMGUI::onRepeat(std::function<void(void)>fun){
    if(item.hover && updater.mb.lmbPress)
        fun();
    return *this;
}
IMGUI& IMGUI::onRepeat(std::function<void(void)>fun, uint32_t freq){
    if(item.hover && updater.mb.lmbPress && (timeFromstart%freq == 0))
        fun();
    return *this;
}
IMGUI& IMGUI::onRepeat(std::function<void(Box)>fun){
    if(item.hover && updater.mb.lmbPress)
        fun(item.lastBox);
    return *this;
}
IMGUI& IMGUI::onrRepeat(std::function<void(void)>fun){
    if(item.hover && updater.mb.rmbPress)
        fun();
    return *this;
}

IMGUI& IMGUI::onHover(std::function<void(void)>fun){
    if(item.hover)
        fun();
    return *this;
}
IMGUI& IMGUI::onHover(std::function<void(Box rect)>fun){
    if(item.hover)
        fun(item.lastBox);
    return *this;
}

IMGUI& IMGUI::onSpecial(std::function<void(void)>fun){
    if(item.special)
        fun();
    return *this;
}
IMGUI& IMGUI::onEverytime(std::function<void(Box rect)>fun){
    fun(item.lastBox);
    return *this;
}
IMGUI& IMGUI::onActive(std::function<void(void)>fun){
    if(item.active)
        fun();
    return *this;
}
IMGUI& IMGUI::onActive(std::function<void(Box rect)>fun){
    if(item.active)
        fun(item.lastBox);
    return *this;
}
IMGUI& IMGUI::onEdition(std::function<void(void)>fun){
    if(item.activeEdition)
        fun();
    return *this;
}

IMGUI& IMGUI::text(const std::string &text, int font, int flag, int caretPosition){
    item.flag |= flag;
    item.text = text;
    item.caret = caretPosition;
    item.textOffset += fontRenderer.render(item.text, font, item.box.xy() + glm::vec2(item.textOffset+3, floor(item.box.w/2.f - assets::getFont(font).lineHeight/2)), item.textColor ? item.textColor : m_style.colors[Style::Font], item.caret);

    if(item.flag & CenterText){
        fontRenderer.move( (item.box.z - item.textOffset)/2-3, 0);
    }
    else if(item.flag & TextToRight){
        fontRenderer.move( (item.box.z - item.textOffset)-3, 0);
    }
    if(LastTextHeight > 0)
        fontRenderer.move( 0, LastTextHeight/2);

    return *this;
}
IMGUI& IMGUI::text(const std::u16string &text, int font, int flag, int caretPosition){
    item.flag |= flag;
    item.caret = caretPosition;
    item.textOffset += fontRenderer.render(text, font, item.box.xy() + glm::vec2(item.textOffset+3, floor(item.box.w/2.f - assets::getFont(font).lineHeight/2)), item.textColor ? item.textColor : m_style.colors[Style::Font], item.caret);

    if(item.flag & CenterText){
        fontRenderer.move( (item.box.z - item.textOffset)/2-3, 0);
    }
    else if(item.flag & TextToRight){
        fontRenderer.move( (item.box.z - item.textOffset)-3, 0);
    }
    return *this;
}

IMGUI& IMGUI::color(HexColor hexColor){
    item.textColor = hexColor;
    return *this;
}
IMGUI& IMGUI::boxColor(HexColor hexColor){
    item.color = hexColor;
    return *this;
}
IMGUI& IMGUI::font(const std::string &font){
    item.font = font;
    return *this;
}
IMGUI& IMGUI::getRect(Box &r){
    r = item.box;
    return *this;
}

IMGUI& IMGUI::mouseOffset(Box &out){
    if(item.hover){
        out = item.box - Box(updater.mb.mousePosition, 0,0);
        out.z = 0;
        out.w = 0;
    }
    return *this;
}
IMGUI& IMGUI::mouseOffset(glm::vec2 &out){
    if(item.hover){
        out = item.box.xy() - updater.mb.mousePosition;
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
    item.editBox = true;
    item.activeEdition = textEditor.compare(value);

    if(item.activeEdition)
        printTextEditorValue();
    else
        text(std::to_string(value));

    processTE(value, base);

    return *this;

}
IMGUI& IMGUI::edit(double &value, int flag){
    // magoc :P
    item.editBox = true;
    item.activeEdition = textEditor.compare(value);

    if(item.activeEdition)
        printTextEditorValue();
    else
        text(std::to_string(value), flag);

    processTE(value);

    return *this;

}
IMGUI& IMGUI::edit(float &value, int flag){
    // magoc :P
    item.editBox = true;
    item.activeEdition = textEditor.compare(value);

    if(item.activeEdition)
        printTextEditorValue();
    else
        text(std::to_string(value), flag);

    processTE(value);

    return *this;

}
IMGUI& IMGUI::edit(std::string &value, int flag){
    // magic :P
    item.editBox = true;
    item.activeEdition = textEditor.compare(value);

    if(item.activeEdition)
        printTextEditorValue();
    else
        text(value, flag);

    processTE(value);

    return *this;

}
IMGUI& IMGUI::edit(const std::string &value, std::function<void(const std::string&)> callback, void *identifier){
    // magic :P
    item.editBox = true;
    item.activeEdition = textEditor.valuePointer == identifier;

    if(item.activeEdition)
        printTextEditorValue();
    else
        text(value, 0);

    /// --------
    if(!(item.activeEdition && item.lClicked)|| this->force()){
        this->textEditor.valuePointer = identifier;
        this->textEditor.currentString = value;
        this->textEditor.m_state = true;
        this->textEditor.m_caretPosition = value.size();
        this->textEditor.m_typeInfo = TypeInfo::EMPTY;
    }
    else if(item.activeEdition && updater.mb.lmbPress && !item.hover){
        this->textEditor.breakEdition();
    }

    onEnter([this, &callback]{
        callback(this->textEditor.value());
        this->textEditor.breakEdition();
        item.special = true;
    });

    return *this;

}
IMGUI& IMGUI::edit(std::string *value, std::function<void(void)>callOnenter){
    return *this;
}

IMGUI& IMGUI::setFlag(int flag){
    item.flag |= flag;
    return *this;
}
IMGUI& IMGUI::slider(float &value, float min, float max){
    auto r = item.box;

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

    if(item.hover && updater.mb.lmbPress){
        item.currentSlider = (void*)&value;
        item.activeEdition = true;
    }
    if(item.currentSlider == (void*)&value and updater.mb.lmbRepeat){
        value = glm::clamp((updater.mb.mousePosition.x - slide.x)/slide.z, 0.f, 1.f)*(max-min)+min;
        slide += Box(-1,-1,2,2);
    }
    // else item.currentSlider = nullptr;

    m_uiGraphic.push(Style::Slide & (item.activeEdition ? Style::None : Style::Press), slide, currentLayer);
    m_uiGraphic.push(Style::Slider & (item.activeEdition ? Style::None : Style::Press), slider_, currentLayer);
    return *this;
}
IMGUI& IMGUI::slider(double &value, double min, double max){
    auto r = item.box;

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

    if(item.hover && updater.mb.lmbPress){
        item.currentSlider = (void*)&value;
        item.activeEdition = true;
    }
    if(item.currentSlider == (void*)&value and updater.mb.lmbRepeat){
        value = glm::clamp((updater.mb.mousePosition.x - slide.x)/slide.z, 0.f, 1.f)*(max-min)+min;
        slide += Box(-1,-1,2,2);
    }
    // else item.currentSlider = nullptr;

    m_uiGraphic.push(Style::Slide & (item.activeEdition ? Style::None : Style::Press), slide, currentLayer);
    m_uiGraphic.push(Style::Slider & (item.activeEdition ? Style::None : Style::Press), slider_, currentLayer);
    return *this;
}
IMGUI& IMGUI::slider(i64 &value, i64 min, i64 max){
    auto r = item.box;

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

    if(item.hover && updater.mb.lmbPress){
        item.currentSlider = (void*)&value;
    }
    if(item.currentSlider == (void*)&value and updater.mb.lmbRepeat){
        value = glm::clamp((updater.mb.mousePosition.x - slide.x)/slide.z, 0.f, 1.f)*(max-min)+min;
        slide += Box(-1,-1,2,2);
    }
    // else item.currentSlider = nullptr;

    m_uiGraphic.push(Style::Slide & (item.activeEdition ? Style::None : Style::Press), slide, currentLayer);
    m_uiGraphic.push(Style::Slider & (item.activeEdition ? Style::None : Style::Press), slider_, currentLayer);
    return *this;
}
IMGUI& IMGUI::slider(u32 &value, u32 min, u32 max){
    auto r = item.box;

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

    if(item.hover && updater.mb.lmbPress){
        item.currentSlider = (void*)&value;
    }
    if(item.currentSlider == (void*)&value and updater.mb.lmbRepeat){
        value = glm::clamp((updater.mb.mousePosition.x - slide.x)/slide.z, 0.f, 1.f)*(max-min)+min;
        slide += Box(-1,-1,2,2);
    }
    // else item.currentSlider = nullptr;

    m_uiGraphic.push(Style::Slide & (item.activeEdition ? Style::None : Style::Press), slide, currentLayer);
    m_uiGraphic.push(Style::Slider & (item.activeEdition ? Style::None : Style::Press), slider_, currentLayer);
    return *this;
}
IMGUI& IMGUI::slider(i32 &value, i32 min, i32 max){
    auto r = item.box;

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

    if(item.hover && updater.mb.lmbPress){
        item.currentSlider = (void*)&value;
    }
    if(item.currentSlider == (void*)&value and updater.mb.lmbRepeat){
        value = glm::clamp((updater.mb.mousePosition.x - slide.x)/slide.z, 0.f, 1.f)*(max-min)+min;
        slide += Box(-1,-1,2,2);
    }
    // else item.currentSlider = nullptr;

    m_uiGraphic.push(Style::Slide & (item.activeEdition ? Style::None : Style::Press), slide, currentLayer);
    m_uiGraphic.push(Style::Slider & (item.activeEdition ? Style::None : Style::Press), slider_, currentLayer);
    return *this;
}
IMGUI& IMGUI::operator () (int flags){
    if(item.lClicked || item.rClicked)
        item.box += Box(-1,-1,2,2);
    if (!(item.buttonFlags & NoInsertion))
         currentBox().insertRect(item.box);

    if(item.style){
        if(item.hover and (item.lClicked or item.rClicked or updater.mb.lmbRepeat or updater.mb.rmbRepeat)) item.style &= Style::Press;
        else if(item.hover) item.style &= Style::Hover;
        else item.style &= Style::None;
        m_uiGraphic.push(item.style, m_style, item.color, item.box, currentLayer);
    }
    if(item.imageEnabled){
        m_uiGraphic.push(Style::Image, item.icon);
    }
    if (flags & UI::CaptureMouse){
        if(item.lClicked || item.rClicked){
            updater.mb.lmbPress = false;
            updater.mb.rmbPress = false;
            this->updater.mb.mousePosition = glm::vec2(-500, -500);
            this->captureMouse = true;
        }
        if(item.lClicked&& item.hover){
            this->captureMouse = true;
        }
    }
    restoreDefaults();
    return *this;
}
void IMGUI::restoreDefaults(){
    item.textColor = 0;
    item.borderColor = 0;
    item.color = 0;
    item.buttonState = 0;
    item.lastBox = item.box;
    item.box = {};
    item.hover = 0;
    item.lClicked = 0;
    item.rClicked = 0;
    item.style = 0;
    item.buttonFlags = 0;
    item.imageEnabled = 0;
    // item.icon = 0;
    item.noDraw = 0;
    item.active = 0;
    item.flag = 0;
    item.text = "";
    item.caret = -1;
    // item.font = ;
    item.textOffset = 0;
    item.fontSize = 0;
    item.activeEdition = false;
    item.editBox = false;
    item.special = false;
}

template<typename T>
void IMGUI::processTE(T &value, int base){
    if(!item.activeEdition && item.lClicked || this->force()){
        this->textEditor.setValueToEdit(value, base);
    }
    else if(item.activeEdition && updater.mb.lmbPress && !item.hover){
        this->textEditor.breakEdition();
    }

    onEnter([this]{
        this->textEditor.finishEdition();
        item.special = true;
    });
}
template<typename T>
void IMGUI::processTE(T &value){
    if(!(item.activeEdition && item.lClicked)|| this->force()){
        this->textEditor.setValueToEdit(value);
    }
    else if(item.activeEdition && updater.mb.lmbPress && !item.hover){
        this->textEditor.breakEdition();
    }

    onEnter([this]{
        this->textEditor.finishEdition();
        item.special = true;
    });
}

}
