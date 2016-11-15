#pragma once

#include "Window.h"
#include "TextureManager.h"
#include "StateManager.h"
#include "SharedContext.h"
#include "EntityManager.h"
#include "Camera.h"
#include "AudioManager.h"
#include "GUIManager.h"
#include "FontManager.h"

class Game
{
public:
	Game();
	~Game();

	bool isRunning() const { return m_window.isRunning(); }

	void update();
	void draw();
	void lateUpdate();

private:
	//Remember that these classes destructors are called from bottom to top
	Window m_window;
	FontManager m_fontManager;
	GUIManager m_guiManager;
	AudioManager m_audioManager;
	TextureManager m_textureManager;
	SharedContext m_sharedContext;
	StateManager m_stateManager;
	EntityManager m_entityManager;
	
	
	sf::Time m_timeElasped;
	sf::Clock m_clock;
};

