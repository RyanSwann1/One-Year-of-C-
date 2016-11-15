#include "StatePause.h"
#include "GUIManager.h"
#include "SharedContext.h"
#include "StateManager.h"
#include "FontManager.h"
#include "EventManager.h"
#include "Window.h"

StatePause::StatePause(StateManager& stateManager, const StateType stateType)
	: StateBase(stateManager, stateType)
{
	GUIInterface pausedInterface("Paused");
	FontManager& fontManager = *m_stateManager->getSharedContext().m_fontManager;
	pausedInterface.constructNewGuiElement(fontManager, GuiStyle(sf::Color::White, 18, sf::Vector2f(150, 50), "Magnificent"), "Paused", sf::Vector2f(400, 200));
	pausedInterface.constructNewGuiElement(fontManager, GuiStyle(sf::Color::White, 18, sf::Vector2f(150, 50), "Magnificent"), "Resume", sf::Vector2f(400, 550));
	pausedInterface.constructNewGuiElement(fontManager, GuiStyle(sf::Color::White, 18, sf::Vector2f(150, 50), "Magnificent"), "Quit", sf::Vector2f(400, 650));
	
	GUIManager& guiManager = *m_stateManager->getSharedContext().m_guiManager;
	guiManager.addInterface("Paused", pausedInterface);
}

StatePause::~StatePause()
{
	GUIManager& guiManager = *m_stateManager->getSharedContext().m_guiManager;
	guiManager.removeInterface("Paused");
}

void StatePause::onEnter()
{
	EventManager& eventManager = *m_stateManager->getSharedContext().m_eventManager;
	eventManager.addCallback<StatePause>("Left_Click", StateType::Paused, &StatePause::reactToInput, this);

	GUIManager& guiManager = *m_stateManager->getSharedContext().m_guiManager;
	guiManager.setCurrentInterface("Paused");
	guiManager.setInterfaceVisibility("Paused", true);
}

void StatePause::onExit()
{
	EventManager& eventManger = *m_stateManager->getSharedContext().m_eventManager;
	eventManger.removeCallback("Left_Click");

	GUIManager& guiManager = *m_stateManager->getSharedContext().m_guiManager;
	guiManager.setInterfaceVisibility("Paused", false);
}

void StatePause::draw(sf::RenderWindow & window)
{
	GUIManager& guiManager = *m_stateManager->getSharedContext().m_guiManager;
	guiManager.draw(window);
}

void StatePause::reactToInput(const EventDetails& eventDetails)
{
	if (eventDetails.m_name == "Left_Click")
	{
		onMouseClick();
	}
}

void StatePause::onMouseClick()
{
	GUIManager& guiManager = *m_stateManager->getSharedContext().m_guiManager;

	sf::RenderWindow& window = *&m_stateManager->getSharedContext().m_window->getWindow();
	sf::Vector2i mousePos = sf::Mouse::getPosition(*&window);
	if (guiManager.interactingWithGuiEntry(mousePos, "Resume"))
	{
		m_stateManager->switchTo(StateType::Game);
	}
	else if (guiManager.interactingWithGuiEntry(mousePos, "Quit"))
	{
		m_stateManager->switchTo(StateType::MainMenu);
		m_stateManager->removeState(StateType::Paused);
		m_stateManager->removeState(StateType::Game);
	}
}