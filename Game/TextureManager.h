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
		const auto res = texture->loadFromFile(filename, area);
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
};

std::map<std::string, sf::Texture*> TextureManager::textureMap;