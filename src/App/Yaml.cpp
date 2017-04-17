#include "Yaml.hpp"
#include <regex>
#include <fstream>
#include <boost/algorithm/string/trim.hpp>
using namespace std::string_literals;
struct Stringify : public boost::static_visitor<std::string>
{

    std::string operator()(const std::string &s) const {
        return s;
    }

    std::string operator()(const u32 &u) const {
        std::stringstream stream;
        stream<<std::hex<<u;
        return stream.str();
    }

    std::string operator()(const double &d) const {
        std::stringstream stream;
        stream<<d;
        return stream.str();
    }

    std::string operator()(const glm::vec4 &v) const {
        std::stringstream stream;
        stream<< "[ "<<v.x<<" "<<v.y<<" "<<v.z<<" "<<v.w<<" ]"s;
        return stream.str();
    }

    std::string operator()(const floatVec &v) const {
        std::stringstream stream;
        stream<<"{ ";
        for(const auto &it : v){
            stream<<it<<" "s;
        }
        stream<<" }";
        return stream.str();
    }

    std::string operator()(const bool &b) const {
        return b ? "yes"s : "no"s;
    }

    std::string operator()(const std::function<void(void)> &f) const {
        return "std::function<void(void)>";
    }
};
struct StringifyWithInfo : public boost::static_visitor<std::string>
{

    std::string operator()(const std::string &s) const {
        return "string: "s + s;
    }

    std::string operator()(const u32 &u) const {
        std::stringstream stream;
        stream<<std::hex<<u;
        return "color: "s + stream.str();
    }

    std::string operator()(const double &d) const {
        std::stringstream stream;
        stream<<d;
        return "double: "s + stream.str();
    }

    std::string operator()(const glm::vec4 &v) const {
        std::stringstream stream;
        stream<< "[ "<<v.x<<" "<<v.y<<" "<<v.z<<" "<<v.w<<" ]"s;
        return "glm: "s + stream.str();
    }

    std::string operator()(const floatVec &v) const {
        std::stringstream stream;
        stream<<"{ ";
        for(const auto &it : v){
            stream<<it<<" "s;
        }
        stream<<" }";
        return "vector "s + stream.str();
    }

    std::string operator()(const bool &b) const {
        return "bool: " + (b ? "yes"s : "no"s);
    }

    std::string operator()(const std::function<void(void)> &f) const {
        return "std::function<void(void)>";
    }
};

std::string Yaml::string() const {
    return boost::apply_visitor(Stringify(), value);
}

std::string Yaml::debugString() const {
    return boost::apply_visitor(StringifyWithInfo(), value);
}

Variants decode(std::string s){
    std::regex rFloat(R"(-?[0-9]+\.?[0-9]*)");
    std::regex rColor3("([0-9a-fA-F]{6})");
    std::regex rColor4("([0-9a-fA-F]{8})");
    std::smatch result;


    if(s.front() == '[' && s.back() == ']'){ /// then match vector
        std::regex_iterator<std::string::iterator> rit ( s.begin(), s.end(), rFloat );
        std::regex_iterator<std::string::iterator> rend;
        glm::vec4 out(0);
        u32 i=0;
        while (rit!=rend && i<4){
            out[i++] = stof(rit->str());
            ++rit;
        }
        return out;
    }
    if(s.front() == '{' && s.back() == '}'){ /// then match vector
        std::regex_iterator<std::string::iterator> rit ( s.begin(), s.end(), rFloat );
        std::regex_iterator<std::string::iterator> rend;
        floatVec out;
        while (rit!=rend){
            out.push_back(stof(rit->str()));
            ++rit;
        }
        return out;
    }

    if(std::regex_match(s, rFloat)){
        return std::stod(s);
    }
    if(std::regex_search(s, result, rColor4)){
        return (u32)std::stoul(result[1], nullptr, 16);
    }
    if(std::regex_search(s, result, rColor3)){
        return (u32)std::stoul(std::string(result[1])+"ff"s, nullptr, 16);
    }

    if(s == "yes" || s == "True" || s == "true") return true;
    if(s == "no" || s == "False" || s == "false") return false;

    return s;
}

enum RecordType
{
    Key_Value, /// key: value
    Array_Value, /// - value
    Array, /// -
    Array_Key_Value, /// - key: value

};

struct Record
{
    RecordType type;
    std::pair<std::string, Variants> value;
};
class NodeLoader
{
public:
    NodeLoader(const std::string &filename){
        file.open(filename);
    }
    ~NodeLoader(){
        file.close();
    }
    Yaml load();
private:
    void loadToBuffer();
    bool isCommentOrEmpty(const std::string&) const;

    u32 countSpaces(const std::string&) const;
    u32 countSpacesArrayIgnore(const std::string&) const;
    Record readValue(const std::string &line);
    u32 insertToParent(Yaml &parent, u32 expectedDepth, u32 start, bool omitArray = false);

    std::fstream file;
    std::vector <std::string> fileBuff;
    u32 lineCount;
    Yaml node {};
};

Yaml NodeLoader::load(){
    loadToBuffer();

    insertToParent(node, 0, 0);

    return node;
}
void NodeLoader::loadToBuffer(){
    if(not file.is_open()){
        error("No such file");
        return;
     }

    std::string buff;
    while(!file.eof()){
        getline(file, buff);
        if(not isCommentOrEmpty(buff)){
            fileBuff.push_back(buff);
        }
    }
    lineCount = fileBuff.size();
}

bool NodeLoader::isCommentOrEmpty(const std::string &s) const {
    for(auto &it : s)
        if(it == '#') return true;
        else if(it != ' ') return false;
    return true;
}

u32 NodeLoader::countSpaces(const std::string &s) const {
    u32 out = 0;
    for(auto &it : s){
        if(it == ' ') ++out;
        else return out;
    }
    return out;
}

u32 NodeLoader::countSpacesArrayIgnore(const std::string &s) const {
    u32 out = 0;
    for(auto &it : s){
        if(it == ' ' || it == '-') ++out;
        else return out;
    }
    return out;
}

Record NodeLoader::readValue(const std::string &line){
    /// \s* any no of whiteSpaces
    /// \S* any non-whiteSpaces
    /// -? optional '-'
    /// () match

    Record out;
    std::regex arrayKeyValue("\\s*-\\s*(\\S*):\\s*([\\S+\\s]*)"); /// - key: value
    std::regex keyValue("\\s*(\\S*):\\s*([\\S+\\s]*)"); /// key: value
    std::regex arrayValue("\\s*-\\s*([\\S+\\s]*)"); /// - value
    std::smatch match;

    if(std::regex_search(line, match, arrayKeyValue)){
        out.value.first = match[1];
        out.value.second = decode(match[2]);
        out.type = Array_Key_Value;
    }
    else if(std::regex_search(line, match, keyValue)){
        out.value.first = match[1];
        out.value.second = decode(match[2]);
        out.type = Key_Value;
    }
    else if(std::regex_search(line, match, arrayValue)){
        out.value.second = decode(match[1]);
        out.type = Array_Value;
    }
    else {
        error("match fail");
    }
    return out;
}

u32 NodeLoader::insertToParent(Yaml &parent, u32 expectedDepth, u32 start, bool omitArray){
    Yaml *currentNode = nullptr;

    for(u32 i=start; i<lineCount; i++){
        auto &line = fileBuff[i];
        auto depth = omitArray ? countSpacesArrayIgnore(line) : countSpaces(line);

        if(depth < expectedDepth) return i-1;
        else if(depth > expectedDepth){
            i = insertToParent(*currentNode, depth, i);
        }
        else if(depth == expectedDepth){
            auto keyVal = readValue(line);
            // log(keyVal.value.first, keyVal.value.second);
            if(keyVal.type == Key_Value || (keyVal.type == Array_Key_Value && omitArray)){
                currentNode = &(parent.push(keyVal.value.first, keyVal.value.second));
                omitArray = false;
            }
            else if(keyVal.type == Array_Value){
                parent.isArray = true;
                currentNode = &(parent.push(keyVal.value.second));
            }
            else if(keyVal.type == Array_Key_Value){
                parent.isArray = true;
                currentNode = &(parent.push(""s));
                i = insertToParent(*currentNode, countSpacesArrayIgnore(line), i, true);
            }
        }
    }
    return 10000;
}

void print(const Yaml &node, std::string offset, bool printOffset){

    if(node.isArray){
        for(auto &it : node.container){
            auto val = boost::apply_visitor(Stringify(), it.value);
            if(printOffset) std::cout<<offset;
            std::cout<<"- ";
            if(not val.empty()){
                std::cout<<" "<<val<<std::endl;
                print(it, offset+"  ", false);
            }
            else {
                print(it, offset+"  ", false);
            }
        }
    }
    else if(not node.container.empty()){
        for(auto &it : node.container){
            auto val = boost::apply_visitor(Stringify(), it.value);
            if(printOffset) std::cout<<offset;
            std::cout<<it.key<<":";
            if(not val.empty())
                std::cout<<" "<<val<<std::endl;
            else
                std::cout<<std::endl;
            print(it, offset+"  ");
            printOffset = true;
        }
    }
}
void print(std::ofstream &file, const Yaml &node, std::string offset = "", bool printOffset = true){

    if(node.isArray){
        for(auto &it : node.container){
            auto val = boost::apply_visitor(Stringify(), it.value);
            if(printOffset) file<<offset;
            file<<"- ";
            if(not val.empty()){
                file<<" "<<val<<"\n";
                print(file, it, offset+"  ", false);
            }
            else {
                print(file, it, offset+"  ", false);
            }
        }
    }
    else if(not node.container.empty()){
        for(auto &it : node.container){
            auto val = boost::apply_visitor(Stringify(), it.value);
            if(printOffset) file<<offset;
            file<<it.key<<":";
            if(not val.empty())
                file<<" "<<val<<"\n";
            else
                file<<"\n";
            print(file, it, offset+"  ");
            printOffset = true;
        }
    }
}

Yaml loadYaml(const std::string &filename){
    info("YAML", filename);
    NodeLoader loader(filename);
    return loader.load();
}

void saveYaml(const std::string &filename, Yaml &node){
    std::ofstream file;
    file.open(filename, std::ios::out | std::ios::trunc);

    if(file.is_open()){
        print(file, node);
    }
    else {
        error(__FUNCTION__, "Unable to open file:", filename);
    }
}
