#pragma once

#include "GuiEntry.h"
#include "GuiStyle.h"
#include <vector>

class FontManager;
class GUIInterface
{
public:
	GUIInterface(const std::string& name);
	~GUIInterface();

	GuiEntry* getGui(const std::string& message);
	
	const std::string& getName() { return m_name; }
	void setVisibility(const bool visible) { m_isVisible = visible; }
	void constructNewGuiElement(FontManager& fontManager, const GuiStyle& guiStyle, const std::string& message, const sf::Vector2f& position);
	bool isMouseHoveringOver() const {}
	void changeMessage(const std::string& existingMessage, const std::string& newMessage);
	void draw(sf::RenderWindow& window);

private:
	std::vector<GuiEntry*> m_entries;
	std::vector<sf::RectangleShape> m_shapes;
	const std::string m_name;
	bool m_isVisible;
};