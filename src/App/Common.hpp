#include <cstdio>
#include <cstdlib>
#include <algorithm>
#include <random>
#include <unordered_map>
#include <functional>
#include <sstream>
#include <chrono>
#include <string>
#include <unordered_map>
#include <iomanip>
#include <type_traits>
#include <memory>

#include <map>
#include <set>
#include <vector>
#include <list>
#include <cmath>

#define GLM_FORCE_SWIZZLE
#define GLM_FORCE_RADIANS
#define GLM_FORCE_SSE2
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/projection.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_access.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <glm/gtx/normal.hpp>
#include <glm/gtx/matrix_major_storage.hpp>
#include <glm/gtx/perpendicular.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/random.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/norm.hpp>
#include <glm/gtc/noise.hpp>
#include <glm/gtx/matrix_interpolation.hpp>
#include <glm/gtx/optimum_pow.hpp>

using HexColor = uint32_t;
using i16 = int16_t;
using i32 = int32_t;
using i64 = int64_t;
using i8 = char;
using Scalar = double;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;
using u8 = uint8_t;
