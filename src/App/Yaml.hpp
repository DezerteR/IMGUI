#pragma once
#include "Common.hpp"
#include "Logging.hpp"
#include <boost/variant.hpp>
#include <LinearMath/btVector3.h>

/**
 *  http://stackoverflow.com/questions/24628099/group-class-template-specializations grupowanie typów, specjalizacje
 *  dla grup
 *  SFINE & type_triats  :http://eli.thegreenplace.net/2014/sfinae-and-enable_if/
 *
 *  https://rmf.io/cxx11/almost-static-if/ :best?
 */

struct Yaml;

typedef std::vector<float> floatVec;
typedef std::vector<glm::vec4> vec4Vec;
typedef boost::variant<
        std::string,             /// 0
        u32,                     /// bitfields? colors etc
        double,                  /// 1
        glm::vec4,               /// 2
        floatVec,                /// 3, czy jest jakiś sens na te typy?
        bool,                    ///
        // vec4Vec,                 /// 4, czy jest jakiś sens na te typy?
        std::function<void(void)>/// 5
    > Variants;
namespace {
    // template <typename... Condition>
    // using EnableIf = typename std::enable_if<all<Condition...>::value, detail::enabler>::type;

    template<class T>
    struct is_from_true_types: std::false_type {};

    template<>
    struct is_from_true_types<std::string>: std::true_type {};
    template<>
    struct is_from_true_types<double>: std::true_type {};
    template<>
    struct is_from_true_types<glm::vec4>: std::true_type {};
    template<>
    struct is_from_true_types<floatVec>: std::true_type {};
    // template<>
    // struct is_from_true_types<bool>: std::true_type {};
    template<>
    struct is_from_true_types<std::function<void(void)>>: std::true_type {};

}

Variants decode(std::string s);
void print(const Yaml &node, std::string offset = "", bool printOffset = true);
Yaml loadYaml(const std::string &filename);
void saveYaml(const std::string &filename, Yaml &node);

class Yaml {
public:
    std::string key;
    Variants value;
    std::vector<Yaml> container;
    u64 userInfo;
    bool isArray {false};
    Yaml() = default;
    Yaml(const Yaml&) = default;
    Yaml(Yaml&&) = default;
    ~Yaml() { }

    Yaml& push(const Yaml &node){
        container.push_back(node);
        return container.back();
    }
    Yaml& push(const std::string &key, const Variants &val){
        container.push_back({key, val});
        return container.back();
    }
    Yaml& push(const Variants &val){
        container.push_back({"", val});
        return container.back();
    }

    Yaml& operator [] (const std::string &s){
        for(auto &it : container)
            if(it.key == s) return it;

        return push(s, "");
    }
    Yaml& operator [] (u32 i){
        if(i >= container.size())
            push("");
        return container[i];
    }
    const Yaml& operator [] (const std::string &s) const {
        for(const auto &it : container)
            if(it.key == s) return it;
        error(s, "in", key, "doesn't exist");
        print(*this);
        return *this;
    }
    const Yaml& operator [] (u32 i) const {
        return container.at(i);
    }

    bool has(const std::string &s) const {
        for(const auto &it : container)
            if(it.key == s) return true;

        return false;
    }

    auto begin(){
        return container.begin();
    }
    auto begin() const {
        return container.begin();
    }
    auto end(){
        return container.end();
    }
    auto end() const {
        return container.end();
    }
    auto size() const {
        return container.size();
    }

    template<typename T, typename = std::enable_if_t<is_from_true_types<T>::value>>
    void operator = (const T &val){
        value = val;
    }
    template<typename T, typename = std::enable_if_t<std::is_arithmetic<T>::value && !std::is_same<T, double>::value>>
    void operator = (T val){
        value = (double)val;
    }
    void operator = (const std::string &val){
        // value = string(val);
        value = val;
    }
    void operator = (const Yaml &&node){
        // value = string(val);
        value = node.value;
        container = node.container;
        userInfo = node.userInfo;
        isArray = node.isArray;
    }
    void operator = (const Yaml &node){
        // value = string(val);
        value = node.value;
        container = node.container;
        userInfo = node.userInfo;
        isArray = node.isArray;
    }
    void operator = (glm::vec2 v){
        value = glm::vec4(v, 0, 0);
    }

    template<
        typename T,
        typename = std::enable_if_t<is_from_true_types<T>::value>>
    T& as(){
        return boost::get<T>(value);
    }
    template<
        typename T,
        typename = std::enable_if_t<std::is_arithmetic<T>::value && !std::is_same<T, double>::value>>
    T as() const {
        return boost::get<double>(value);
    }

    glm::vec4& vec4(){
        return boost::get<glm::vec4>(value);
    }
    glm::vec4 vec4() const {
        return boost::get<glm::vec4>(value);
    }
    glm::vec4 vec31() const {
        return glm::vec4(boost::get<glm::vec4>(value).xyz(), 1);
    }
    glm::vec4 vec30() const {
        return glm::vec4(boost::get<glm::vec4>(value).xyz(), 0);
    }
    glm::vec3 vec3() const {
        return boost::get<glm::vec4>(value).xyz();
    }
    glm::vec2 vec2() const {
        return boost::get<glm::vec4>(value).xy();
    }
    glm::quat quat() const {
        auto val = boost::get<glm::vec4>(value);
        return glm::quat(val.x, val.y, val.z, val.w);
    }
    btVector3  btVec() const {
        auto val = boost::get<glm::vec4>(value);
        return btVector3(val.x, val.y, val.z);
    }
    u32& color(){
        return boost::get<u32>(value);
    }
    u32 color() const {
        return boost::get<u32>(value);
    }

    void toArray(){
        auto &vec = boost::get<glm::vec4>(value);
        floatVec out(4);

    }
    /// to glm, how name it?
    void toVector(){
        auto &vec = boost::get<floatVec>(value);
        glm::vec4 out{};

        for(u32 i=0; i<vec.size() && i<4; i++)
            out[i] = vec[i];
        value = out;
    }

    double& number(){
        return boost::get<double>(value);
    }
    double number() const {
        return boost::get<double>(value);
    }
    void execute(){
        boost::get<std::function<void(void)>>(value)();
    }
    bool& boolean(){
        return boost::get<bool>(value);
    }
    bool boolean() const {
        return boost::get<bool>(value);
    }
    std::string string() const ;
    std::string debugString() const ;

    bool operator ~ () const {
        return ~userInfo;
    }
    bool operator | (u64 val) const {
        return userInfo | val;
    }
    void operator |= (u64 val){ // set bit
         userInfo |= val;
    }
    void operator ^= (u64 val){ // toggle bit
         userInfo ^= val;
    }
    bool operator & (u64 val) const {
        return userInfo & val;
    }
    void operator &= (u64 val){ // reset bit userinfo &= ~bit
         userInfo &= val;
    }


};
