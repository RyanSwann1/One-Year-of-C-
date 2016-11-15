#include "GUIManager.h"
#include <iostream>


GUIManager::GUIManager()
	: m_currentInterface(nullptr)
{}

GUIManager::~GUIManager()
{
	//m_currentInterface = nullptr;
	//m_guiInterfaces.clear();
}

void GUIManager::addInterface(const std::string & interfaceName, const GUIInterface& guiInterface)
{
	auto iter = findInterface(interfaceName);
	if (!iter)
	{
		m_guiInterfaces.emplace(std::make_pair(interfaceName, guiInterface));
	}
	else {
		std::cerr << "Interface already exists.\n";
	}
}

void GUIManager::removeInterface(const std::string & interfaceName)
{
	auto iter = m_guiInterfaces.find(interfaceName);
	if (iter != m_guiInterfaces.cend())
	{
		if (m_currentInterface->getName() == interfaceName)
		{
			m_currentInterface = nullptr;
		}
		m_guiInterfaces.erase(iter);
		int i = 0;
	
	}
}

void GUIManager::setCurrentInterface(const std::string & interfaceName)
{
	auto iter = findInterface(interfaceName);
	if (iter)
	{
		m_currentInterface = iter;
	}
	else
	{
		std::cerr << "Interface not found.\n";
	}
}

void GUIManager::setInterfaceVisibility(const std::string & interfaceName, const bool visible)
{
	auto iter = findInterface(interfaceName);
	if (iter)
	{
		iter->setVisibility(visible);
	}
}

void GUIManager::draw(sf::RenderWindow & window)
{
	if (m_currentInterface)
	{
		m_currentInterface->draw(window);
	}
}

bool GUIManager::interactingWithGuiEntry(const sf::Vector2i& mousePos, const std::string& message)
{
	if (m_currentInterface)
	{
		GuiEntry* guiEntry = m_currentInterface->getGui(message);
		if (guiEntry)
		{
			sf::FloatRect& guiEntryRect = guiEntry->getPosition();

			if (mousePos.x > guiEntryRect.left && 
				mousePos.y > guiEntryRect.top && 
				mousePos.x < (guiEntryRect.left + guiEntryRect.width) && 
				mousePos.y < (guiEntryRect.top + guiEntryRect.height))
			{
				std::cout << "Mouse Position x: " << mousePos.x << "\n";
				std::cout << "Mouse Position y: " << mousePos.y << "\n";
				return true;
			}
			else
			{
				std::cout << "Mouse Position x: " << mousePos.x << "\n";
				std::cout << "Mouse Position y: " << mousePos.y << "\n";
			}
		}
	}
	//Debug
	//else
	//{
	//	std::cout << "Mouse position x: " << mousePos.x << "\n";
	//	std::cout << guiEntryRect.left << "\n";
	//	std::cout << "Mouse Position y: " << mousePos.y << "\n";
	//	std::cout << guiEntryRect.top << "\n";
	//}
	
	return false;
}

GUIInterface * GUIManager::findInterface(const std::string & interfaceName)
{
	auto cIter = m_guiInterfaces.find(interfaceName);
	return (cIter != m_guiInterfaces.cend() ? &cIter->second : nullptr);
}