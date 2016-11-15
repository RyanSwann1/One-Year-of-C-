#pragma once

#include "GUIInterface.h"
#include "SharedContext.h"
#include <unordered_map>

class GUIManager
{
public:
	GUIManager();
	~GUIManager();

	void addInterface(const std::string& interfaceName, const GUIInterface& guiInterface);
	void removeInterface(const std::string& interfaceName);
	void setCurrentInterface(const std::string& interfaceName);
	void setInterfaceVisibility(const std::string& interfaceName, const bool visible);

	bool interactingWithGuiEntry(const sf::Vector2i& mousePos, const std::string& message);
	void draw(sf::RenderWindow& window);

private:
	std::unordered_map<std::string, GUIInterface> m_guiInterfaces;
	GUIInterface* m_currentInterface;
	GUIInterface* findInterface(const std::string& interfaceName);
};