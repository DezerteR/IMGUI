#include "Common.hpp"
#include "Constants.hpp"

const float pi = 3.141592f;
const double dpi = 3.1415926535;
const double dpi2 = 2*3.1415926535;
const float toRad = 3.141592f / 180.f;
const float fromRad = 180.f / 3.141592f;
const float toDeg = 180.f / 3.141592f;
const float hpi = 3.141592f * 0.5f;
const float pi2 = 3.141592f * 2.f;

const float toMS = 5.f/18.f;
const float toKMH = 18.f/5.f;

const glm::vec4 X(1,0,0,0);
const glm::vec4 Y(0,1,0,0);
const glm::vec4 Z(0,0,1,0);
const glm::vec4 W(0,0,0,1);
const glm::vec4 X4(1,0,0,0);
const glm::vec4 Y4(0,1,0,0);
const glm::vec4 Z4(0,0,1,0);
const glm::vec4 W4(0,0,0,1);

const glm::vec3 X3(1,0,0);
const glm::vec3 Y3(0,1,0);
const glm::vec3 Z3(0,0,1);

const glm::mat4 identity(1);
#ifdef USE_BULLET
const btTransform btIdentity = btTransform::getIdentity();
#endif
const glm::quat qIdentity = glm::quat(0,0,0,1);
