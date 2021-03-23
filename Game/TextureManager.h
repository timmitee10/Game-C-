#pragma once
#include <map>
#include <memory>
#include <string>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Texture.hpp>
/*
 * Loads in texture from files and stores them in a map
 */
class TextureManager
{
public:
	static bool Load(const std::string& filename, const sf::IntRect& area = sf::IntRect())
	{
		auto* texture = new sf::Texture();
		auto res = texture->loadFromFile(filename, area);
		if (!res)
			return false;
		textureMap.insert({filename,std::make_unique<sf::Texture>(*texture)});
	}

	static const sf::Texture& Get(const std::string& name)
	{
		return *textureMap.at(name).get();
	}
	
	static std::map<std::string, std::unique_ptr<sf::Texture>> textureMap;
};