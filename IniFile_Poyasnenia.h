#include <string>
#include <map>
#include <vector>
#include <cassert>
#include <fstream>
#include <iostream>

class IniFile {
	std::map<std::string, std::map<std::string, std::string>> _data;
	std::vector<std::string> _boolValues = { "true", "on", "yes", "y", "1" }; // for realisation function read for bool
public:
	IniFile(std::string path) {
		// открыть файл (path)
		// while(getline(файла))
		// 		если строка начинается
		//		[ и заканчивается символом ]
		//			это секция
		//		иначе если содержит = 
		//			это ключ-значение
		//			найти в строке =
		//			всё до = - key
		//			после = - value
		//		иначе continue
	}

	void save(std::string path) const {
		// for(section : sections)
		// 		<< [ section ]
		//		for(key : section)
		//			<< key = value
	}

	/**
	 * Считывает из секции section
	 * значение ключа key
	 * Если не удалось считать - возвращает default value
	 *
	 * Определить для std::string, int, float, bool
	 */
	template<typename T>
	T read(std::string section, std::string key, T defaultValue = T{}) const;

	template<>
	std::string read(section, key, std::string defaultValue)
	{
		return _data[section][key];
	}

	template<>
	int read(section, key, int defaultValue)
	{
		try {
			return std::stoi(_data[section][key]);
		}
		catch(std::exception e) {
			return defaultValue;
		}
	}

	/**
	 * В ключ key из секции section
	 * записывает значение value
	 *
	 * Определить для std::string, int, float, bool
	 */
	template<typename T>
	void write(std::string section, std::string key, T value);

	/**
	 * Проверяет, существует ли секция section
	 */
	bool sectionExists(std::string section) const;

	/**
	 * Проверяет, существует ли ключ key в секции section
	 */
	bool keyExists(std::string section, std::string key) const;

	/**
	 * Возвращает существующие секции
	 */
	std::vector<std::string> sections() const;

	/**
	 * Возвращает существующие ключи в секции section
	 */
	std::vector<std::string> keys(std::string section) const;
};

// true -  true, True, TRUE, on, On, ON, yes, Yes, YES, y, Y, 1
// false - остальные
