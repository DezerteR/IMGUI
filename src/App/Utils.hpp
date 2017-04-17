#pragma once
#include "Common.hpp"
#include <fstream>

glm::vec4 colorHex(uint32_t hexVal);
// glm::vec4 colorHex(uint64_t hexVal);
int clamp(int val, int a, int b);
float RNG();
float RNG(float mod);
float sgnRNG();
int iRNG(int val);

void setColumn(glm::mat4 &mat, int num, glm::vec4 vec);
#ifdef USE_BULLET
glm::vec4 toGlm(const btVector3 &v, float z=1.f);
glm::vec3 toGlm3(const btVector3 &v);
btVector3 tobt3(const glm::vec4 &v);
btVector3 tobt3(const glm::vec3 &v);
glm::mat4 toGlm(const btTransform &trans);
std::string to_string(const btVector3 &v);
std::string to_string(const btTransform &t);
#endif
glm::vec4 cross(glm::vec4 const & x, glm::vec4 const & y);
std::string string_formatted(double f);
std::string toString(float f);
std::string to_string(const glm::vec4 &v);
std::string to_string(const glm::vec3 &v);

float period(float val, float a, float b);
double period(double val, double a, double b);
double period(double val);
float sign(float x);
double sign(double x);
float circleDistance(float target, float value);
double circleDistance(double target, double value);

double clamp(double x, double minVal, double maxVal);

template<typename T>
void cyclicIncr(T &value, u32 range){
    if(++value >= range) value = 0;
}
template<typename T>
void cyclicDecr(T &value, u32 range){
    if(value <= 0) value = range-1;
    else value--;
}

template <typename T>
bool saveCache(std::string fileName, const std::vector<T> &data){
    std::ofstream file;
    file.open("cache/"+fileName+".cache", std::ios::binary | std::ios::out | std::ios::trunc);

    // if(file.is_open()){
        file.write((char*)data.data(), data.size()*sizeof(T));
    // }
    // else log(__FUNCTION__, "Unable to cache file:", fileName);
    file.close();
    return true;
}
template <typename T>
bool loadCache(std::string fileName, std::vector<T> &data){
    std::ifstream file;
    int size=0;
    // try {
        file.open("../bin/cache/"+fileName+".cache", std::ios::binary | std::ios::in);
        // if(file.is_open()){
        if(file.good()){
            file.seekg(0, std::ios::end);
            size = file.tellg()/sizeof(T);
            file.seekg(0, std::ios::beg);

            data.resize(size);
            file.read((char*)data.data(), size*sizeof(T));
            file.close();
        }
        else {
            file.close();
            return false;
        }
    return true;
}

bool writeBinary(const std::string &path, i8 *data, u32 size);
bool readBinary(const std::string &path, i8 *data, u32 size);

bool writeBinary(const std::string &path, std::function<void(std::ofstream&)> &fun);
bool readBinary(const std::string &path, std::function<void(i8*, u32)> &fun);

// void listFiles(std::set<std::string>&files, std::string pathName, std::string fileType);
const std::string getName(const std::string &name);
const std::string getExt(const std::string &name);
const std::string cutExt(const std::string &name);
const std::string getPath(const std::string &name);
bool isFile(const std::string &file);
bool findFile(const std::string &from, const std::string &name, const std::string &params, std::string &ref);
std::vector<std::string> listDirectory(const std::string &dir);

class PDreg {
public:
    float kr;
    float td;
    float speed;
    float prev;

    float operator () (float toReach, float current, float dt);
};

struct Statement {
    std::string statement;
    float lifeTime;
};

void statement(std::string text, float lifeTime = 5.f);
