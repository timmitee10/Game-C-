#pragma once
#include <string>
#include <SFML/Graphics/Texture.hpp>
#include <map>
/*
 * Loads in texture from files and stores them in a map
 */
class TextureManager
{
public:
	inline static bool Load(const std::string& filename, const sf::IntRect& area = sf::IntRect())
	{
		auto* texture = new sf::Texture();
		const auto path = folder + filename;
		const auto res = texture->loadFromFile(path, area);
		if (!res)
			return false;
		textureMap.insert({ filename,texture });
		return true;
	}

	inline 	static const sf::Texture* Get(const std::string& name)
	{
		return textureMap.at(name);
	}

	static std::map<std::string, sf::Texture*> textureMap;
private:
	static std::string folder;
};

inline std::map<std::string, sf::Texture*> TextureManager::textureMap;
inline std::string TextureManager::folder = "Content/";