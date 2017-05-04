#include "KeyState.hpp"

bool KeyState::lClick = false;
bool KeyState::mClick = false;
bool KeyState::rClick = false;

bool KeyState::lClicked = false;
bool KeyState::mClicked = false;
bool KeyState::rClicked = false;

glm::vec2 KeyState::mousePosition = glm::vec2(0,0);
glm::vec2 KeyState::mouseTranslation = glm::vec2(0,0);
glm::vec2 KeyState::mouseTranslationNormalized = glm::vec2(0,0);

void KeyState::reset(){
    lClick = false;
    mClick = false;
    rClick = false;
}
