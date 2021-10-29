#include <string>
#include <map>
#include <vector>
#include <cassert>
#include <fstream>
#include <utility>
#include <sstream>
#include <iostream>


class IniFile {
private:
    std::string trimmed(const std::string &str){
        size_t i, j;
        i = str.find_first_not_of(' ');
        if (i == std::string::npos) 
            return std::string();
        j = str.find_last_not_of(' ');
        return str.substr(i, j - i + 1);
    }

    std::map<
        std::string,
        std::map<std::string, std::string>
    > _data;
    std::string _filename = "";
public:
	IniFile(std::string filename){
        _filename = filename;
        std::ifstream file(_filename);
        if (file.fail()){
            std::ofstream file(_filename);
            file.close();
        }
        else{
            std::string line;
            std::string section = "";
            std::string key = "";
            std::string value = "";
            std::map<std::string, std::string> s;
            while(getline(file, line)){
                line = trimmed(line);
                if((line.empty()) || (line[0] == ';')){
                    continue;
                }
                if (line[0] == '['){
                    if(key != ""){
                        _data.insert({section, s});
                        key = "";
                        value = "";
                        s.clear();
                    }
                    section = line.substr(1, line.find_last_not_of(']'));
                }
                else {
                    key = line.substr(0, line.find('='));
                    value = line.substr(line.find('=') + 1, line.size() - 1);
                    s.insert({key, value});
                }
            }
            _data.insert({section, s});
            file.close();
        }
    }
	
    /*
    void print(){
        for (auto it = _data.begin(); it != _data.end(); it++) {
            std::map<std::string, std::string> c = it->second;
            std::cout << c.size() << " size" << std::endl;
            for(auto ld = c.begin(); ld != c.end(); ld++)
                std::cout << "Section " << it->first << " Ключ " << ld->first 
                << ", значение " << ld->second << std::endl;
        }
    }
    */

    std::string fileName(){
        return _filename;
    }

	void save() const{
        std::ofstream file(_filename);
        //assert(file.is_open());
		auto sections = getSections();
        for(int i = 0; i < sections.size(); ++i){
            auto keys = getKeys(sections[i]);
            auto values = getValues(sections[i]);
            file << '[' <<sections[i] << ']' << '\n';
            for(int j = 0; j < keys.size(); ++j){
                file << keys[j] << '=' << values[j] << '\n';
             } 
        }
		file.close();
    }
	
    bool isExists(){
        std::fstream file(_filename);
        if(file){
            file.close();
            return 1;
        }
        else{
            return 0;
        }
    }
    /**
     * Считывает из секции section
     * значение ключа key
     * Если не удалось считать - возвращает default value
     * 
     */
    template<typename T>
	T read(std::string section, std::string key, T defaultValue = T{});
    /**
     * В ключ key из секции section 
     * записывает значение value
     *
     * Определить для std::string, int, float, bool, color(if you wish)
     */
	template<typename T>
	void write(std::string section, std::string key, T value);

    /**
     * @brief Get the Section Count object
     * 
     * @return int 
     */
    int getSectionCount(){
        return _data.size();
    }

    /**
     * @brief Get the Keys Count object
     * 
     * @param section 
     * @return int 
     */
    int getKeysCount(std::string section) {
        return _data[section].size();
    }

    /**
     * Проверяет, существует ли секция section
     */
    bool sectionExists(std::string section) const{
        if (_data.find(section) != _data.end()){
            return 1;
        }
        else {
            return 0;
        }
    }

    /**
     * Проверяет, существует ли ключ key в секции section
     */
    bool keyExists(std::string section, std::string key) const{
        if (sectionExists(section) == 1){
            auto c = _data.at(section);
            if (c.find(key) != c.end()){
                return 1;
            }
        }
        return 0;
    }

    /**
     * Возвращает существующие секции
     */
    std::vector<std::string> getSections() const{
        std::vector<std::string> sections;
        for(auto i : _data){
            sections.push_back(i.first);
        }
        return sections;
    }

    /**
     * Возвращает существующие ключи в секции section
     */
    std::vector<std::string> getKeys(std::string section) const{
        std::vector<std::string> keys;
        for (auto j : _data.at(section)){
            keys.push_back(j.first);
        }
        return keys;
    }

    /**
     * возвращает значения в секции
     */
    std::vector<std::string> getValues(std::string section) const{
        std::vector<std::string> values;
        for (auto j : _data.at(section)){
            values.push_back(j.second);
        }
        return values;
    }

    /**
     * удаляет секцию
     */
    void deleteSection (std::string section){
        _data.erase(section);
    }

    /**
     * @brief 
     * удаляет ключ
     */
    void deleteKey(std::string section, std::string key){
        _data[section].erase(key);
    }
};

//не вижу смысла в дополнительной специализации, так как значение
//defaultValue и так специализированно под каждый тип данных как T{0}
	template<typename T>
	T IniFile::read(std::string section, std::string key, T defaultValue) {
        if((sectionExists(section)) && (keyExists(section, key))){
            std::string c = _data.at(section).at(key);
            std::istringstream key(c);
            key >> defaultValue;
            return defaultValue;
        }
        else{
            return defaultValue;
        }
    }
    template<>
    bool IniFile::read<bool>(std::string section, std::string key, 
        bool defaultValue) {
        if((sectionExists(section)) && (keyExists(section, key))){
            std::string c = _data.at(section).at(key);
            if ((c == "True") || (c == "true") || (c == "TRUE") 
            || (c == "On") || (c == "on") || (c == "ON")
            || (c == "Yes") || (c == "yes") || (c == "YES")
            || (c == "y") || (c == "Y") || (c == "1")){
                return true;
            }
            else {
                return false;
            }
        }
        else{
            return defaultValue;
        }
    }
    template<>
	std::string IniFile::read<std::string>(std::string section, 
        std::string key, std::string defaultValue) {
        if((sectionExists(section)) && (keyExists(section, key))){
            return _data.at(section).at(key);
        }
        else{
            return defaultValue;
        }
    }

    template<typename T>
	void IniFile::write(std::string section, std::string key, T value){
        std::ostringstream _value;
        _value << value;
        _data[section][key] = _value.str();
        
    }
    template<>
    void IniFile::write<bool>(std::string section, std::string key, bool value){
        if(value == true){
            _data[section][key] = "true";
        }
        else {
            _data[section][key] = "false";
        }
    }
    template<>
    void IniFile::write<std::string>(std::string section, std::string key, std::string value){
        _data[section][key] = value;
    }
    template<>
    void IniFile::write<char>(std::string section, std::string key, char value){
        _data[section][key] = value;
    }
// true -  true, True, TRUE, on, On, ON, yes, Yes, YES, y, Y, 1
// false - остальные