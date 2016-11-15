#include "StateMainMenu.h"
#include "SharedContext.h"
#include "StateManager.h"
#include "GUIInterface.h"
#include "GUIManager.h"
#include "Window.h"
#include "EventManager.h"

StateMainMenu::StateMainMenu(StateManager& stateManager, const StateType stateType)
	: StateBase(stateManager, stateType)
{
	//Create the main menu interface
	GUIManager& guiManager = *m_stateManager->getSharedContext().m_guiManager;
	FontManager& fontManager = *m_stateManager->getSharedContext().m_fontManager;
	GUIInterface mainMenuInterface("MainMenu");
	mainMenuInterface.constructNewGuiElement(fontManager, GuiStyle(sf::Color::White, 18, sf::Vector2f(150, 50), "Magnificent"), "Play", sf::Vector2f(350, 200));
	mainMenuInterface.constructNewGuiElement(fontManager, GuiStyle(sf::Color::White, 18, sf::Vector2f(150, 50), "Magnificent"), "Quit", sf::Vector2f(350, 400));
	guiManager.addInterface("MainMenu", mainMenuInterface);
}

StateMainMenu::~StateMainMenu()
{
	GUIManager& guiManager = *m_stateManager->getSharedContext().m_guiManager;
	guiManager.setInterfaceVisibility("MainMenu", false);
	guiManager.removeInterface("MainMenu");
}

void StateMainMenu::onEnter()
{
	EventManager& eventManager = *m_stateManager->getSharedContext().m_eventManager;
	eventManager.addCallback<StateMainMenu>("Left_Click", StateType::MainMenu, &StateMainMenu::reactToInput, this);

	GUIManager& guiManager = *m_stateManager->getSharedContext().m_guiManager;
	guiManager.setCurrentInterface("MainMenu");
	guiManager.setInterfaceVisibility("MainMenu", true);
}

void StateMainMenu::onExit()
{
	EventManager& eventManager = *m_stateManager->getSharedContext().m_eventManager;
	eventManager.removeCallback("Left_Click");
}

void StateMainMenu::draw(sf::RenderWindow & window)
{
	m_stateManager->getSharedContext().m_guiManager->draw(window);
}

void StateMainMenu::reactToInput(const EventDetails& eventDetails)
{
	if (eventDetails.m_name == "Left_Click")
	{
		onMouseClick();
	}
}

void StateMainMenu::onMouseClick()
{
	Window& window = *m_stateManager->getSharedContext().m_window;
	sf::Vector2i mousePos = sf::Mouse::getPosition(window.getWindow());
	GUIManager& guiManager = *m_stateManager->getSharedContext().m_guiManager;
	
	if (guiManager.interactingWithGuiEntry(mousePos, "Play"))
	{
		m_stateManager->switchTo(StateType::Game);
		m_stateManager->removeState(StateType::MainMenu);
	}
	else if (guiManager.interactingWithGuiEntry(mousePos, "Quit"))
	{
		m_stateManager->switchTo(StateType::EndGame);
		m_stateManager->removeState(StateType::MainMenu);
	}	
}