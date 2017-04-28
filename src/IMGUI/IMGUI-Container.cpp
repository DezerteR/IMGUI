#include "IMGUI.hpp"
#include "Yaml.hpp"
#include "Assets.hpp"

namespace UI {


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
    m_group.m_freeRect = fixRect(spawnPosition); // czy to jest do czego u¿ywane?, nie
    spawnPosition.z = 0;
    spawnPosition.w = 0;
    return m_boxStack[++m_boxIndex].box(flags, spawnPosition, this);
}
IMGUIBox& IMGUI::endTable(){
    endBox();
    // m_fontSize = m_defaultFontSize;
    m_font = m_defaultFont;
    // m_indentation = 0;
    IMGUIBox &group = m_boxStack[m_boxIndex+1];
    float b = group.m_border;

    Box rect = group.m_box;
    if((group.m_flags & FixedSize) != FixedSize){
        if(group.m_flags & Vertical)
            rect.w = (abs(group.m_box.y - group.m_currStart.y)+b)*sign(rect.w);

        else if(group.m_flags & Horizontal)
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
        imgui->m_uiGraphic.push(Style::Box & Style::None, spawnPosition, _imgui->currentLayer);
    }

    return *this;
}
IMGUIBox& IMGUIBox::size(int x, int y){
    const auto &parentBox = imgui->parentBox().m_box;

    m_box.z = x;
    m_box.w = y;
    m_flags |= FixedSize;

    return *this;
}
IMGUIBox& IMGUIBox::size(int x, float y){
    const auto &parentBox = imgui->parentBox().m_box;

    y = floor(parentBox.w * y);

    m_box.z = x;
    m_box.w = y;
    m_flags |= FixedSize;

    return *this;
}
IMGUIBox& IMGUIBox::size(float x, int y){
    const auto &parentBox = imgui->parentBox().m_box;

    x = floor(parentBox.z * x);

    m_box.z = x;
    m_box.w = y;
    m_flags |= FixedSize;

    return *this;
}
IMGUIBox& IMGUIBox::size(float x, float y){
    const auto &parentBox = imgui->parentBox().m_box;

    x = floor(parentBox.z * x);
    y = floor(parentBox.w * y);

    m_box.z = x;
    m_box.w = y;
    m_flags |= FixedSize;

    return *this;
}
IMGUIBox& IMGUIBox::pos(int x, int y){
    m_flags |= AbsolutePosition;
    float u(x),v(y);
    const auto &parentBox = imgui->parentBox().m_box;

    if(x < 0) u += parentBox.z;
    if(y < 0) v += parentBox.w;

    m_currStart.x = parentBox.x + u;
    m_currStart.y = parentBox.y + v;
    m_box.x = parentBox.x + u;
    m_box.y = parentBox.y + v;
    return *this;
}
IMGUIBox& IMGUIBox::pos(int x, float y){
    m_flags |= AbsolutePosition;
    float u(x),v(y);
    const auto &parentBox = imgui->parentBox().m_box;

    if(x < 0) u += parentBox.z;
    v = floor(parentBox.w*y);

    m_currStart.x = parentBox.x + u;
    m_currStart.y = parentBox.y + v;
    m_box.x = parentBox.x + u;
    m_box.y = parentBox.y + v;
    return *this;
}
IMGUIBox& IMGUIBox::pos(float x, int y){
    m_flags |= AbsolutePosition;
    float u(x),v(y);
    const auto &parentBox = imgui->parentBox().m_box;


    if(x<1) u = floor(parentBox.z*x);
    if(y < 0) v += parentBox.w;

    m_currStart.x = parentBox.x + u;
    m_currStart.y = parentBox.y + v;
    m_box.x = parentBox.x + u;
    m_box.y = parentBox.y + v;
    return *this;
}
IMGUIBox& IMGUIBox::pos(float x, float y){
    m_flags |= AbsolutePosition;
    float u(x),v(y);
    const auto &parentBox = imgui->parentBox().m_box;

    u += floor(parentBox.z*x);
    v = floor(parentBox.w*y);

    m_currStart.x = parentBox.x + u;
    m_currStart.y = parentBox.y + v;
    m_box.x = parentBox.x + u;
    m_box.y = parentBox.y + v;
    return *this;
}
IMGUIBox& IMGUIBox::screenPos(int x, int y){
    m_flags |= AbsolutePosition;
    float u(x),v(y);
    auto &parentBox = imgui->parentBox().m_box;
    // auto &parent = imgui->parentBox().m_currStart;
    if(x>-1 && x<1) u = floor(parentBox.z*x);
    else if(x < 0) u += parentBox.z;

    if(y>-1 && y<1) v = floor(parentBox.w*y);
    else if(y < 0) u += parentBox.w;

    m_currStart.x = u;
    m_currStart.y = v;
    m_box.x = u;
    m_box.y = v;
    return *this;
}
// IMGUIBox& IMGUIBox::offset(float x, float y){
    //     m_flags |= RelativePosition;
    //     float u(x),v(y);
    //     auto &parentBox = imgui->parentBox().m_box;
    //     // auto &parent = imgui->parentBox().m_currStart;
    //     if(x>-1 && x<1){
    //         u += floor(parentBox.z*x);
    //     }
    //     if(y>-1 && y<1){
    //         v += floor(parentBox.w*y);
    //     }
    //     if(x<0){
    //         u += parentBox.z;
    //     }
    //     if(y<0){
    //         v += parentBox.w;
    //     }
    //     m_currStart.x += u;
    //     m_currStart.y += v;
    //     m_box.x += u;
    //     m_box.y += v;
    //     return *this;
    // }
IMGUIBox& IMGUIBox::border(int x, int y){
    m_border = x;
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
        if(group.m_flags & Vertical)
            rect.w = std::max((abs(group.m_box.y - group.m_currStart.y)+b), std::abs(group.m_box.w))*sign(rect.w);

        else if(group.m_flags & Horizontal)
            rect.z = std::max((abs(group.m_box.x - group.m_currStart.x)+b), std::abs(group.m_box.z))*sign(rect.z);
    // }

    rect = fixRect(rect);

    if(group.m_flags & Draw){
        m_uiGraphic.change(Style::Box & Style::None, group.m_rectIdx, rect, currentLayer);
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

    if(m_flags & Vertical){
        float mod;
        if(m_flags & ToTop)
            mod = -1.f;
        else if(m_flags & ToBottom)
            mod = 1.f;

        if(m_flags & ToLeft){
            rect = m_currStart + Box(border,mod*border,w, h);
        }
        else if(m_flags & ToRight){
            rect = m_currStart + Box(-border-w,mod*border,w, h);
        }
        if(mod == -1.f){
            rect = Box(rect.x, rect.y - rect.w, rect.z, rect.w);
        }
    }

    else if(m_flags & Horizontal){
        float mod;
        if(m_flags & ToLeft)
            mod = 1.f;
        else if(m_flags & ToRight)
            mod = -1.f;

        if(m_flags & ToBottom){
            rect = m_currStart + Box(mod*border,1*border,w, h);
        }
        else if(m_flags & ToTop){
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

    if(m_flags & Vertical){

        float mod = 0;
        if(m_flags & ToTop)
            mod = -1.f;
        else if(m_flags & ToBottom)
            mod = 1.f;

        if(m_flags & ToLeft){
            m_box.z = std::max(m_box.z, w+2*border);
        }
        else if(m_flags & ToRight){
            m_box.z = std::min(m_box.z, -w-2*border);
        }

        m_currStart += Box(0,(border + h-1)*mod,0,0);

        if(!(m_flags & FixedSize))
            m_box += Box(0,0,0,(border + h-1)*mod);

    }

    else if(m_flags & Horizontal){
        float mod=1.f;
        if(m_flags & ToLeft)
            mod = 1.f;
        else if(m_flags & ToRight)
            mod = -1.f;

        if(m_flags & ToBottom){
            m_box[3] = std::max(m_box[3], h+2*border);

        }
        else if(m_flags & ToTop){
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

    Box m_freeRect = bounds;

    if(flags & Vertical){
        if(flags & ToLeft && flags & ToTop){
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
        else if(flags & ToRight && flags & ToTop){
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
        else if(flags & ToLeft && flags & ToBottom){
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
        else if(flags & ToRight && flags & ToBottom){
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

    else if(flags & Horizontal){

        if(flags & ToLeft && flags & ToTop){
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
        else if(flags & ToRight && flags & ToTop){
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
        else if(flags & ToLeft && flags & ToBottom){
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
        else if(flags & ToRight && flags & ToBottom){
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

}
