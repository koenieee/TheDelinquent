#include "ScreenController.hpp"

sf::Thread* ScreenController::m_thread = nullptr; // instance of (running) Thread
sf::RenderWindow* ScreenController::m_window = nullptr; // instance of RenderWindow for rendering
bool ScreenController::m_running = false; // indicator for running thread

sf::Font* ScreenController::m_font = new sf::Font();
sf::Clock ScreenController::m_fpsClock;
sf::Clock ScreenController::m_gameClock;
long ScreenController::m_nrDrawUpdates = 0;
long ScreenController::m_avgFps = 0;

unsigned int ScreenController::m_fpsLimit = 60;

unsigned int ScreenController::m_windowWidth = GlobalValues::WINDOW_SIZE_X;
unsigned int ScreenController::m_windowHeight = GlobalValues::WINDOW_SIZE_Y;

bool ScreenController::m_focus = true;

sf::View ScreenController::m_view;
sf::View ScreenController::m_fixedView;

sf::Vector2f ScreenController::movementOffset;

State * ScreenController::activeState = nullptr;

void ScreenController::initialize()
{
	m_window = new sf::RenderWindow{
		sf::VideoMode{ m_windowWidth, m_windowHeight },
		"De Delinquent", sf::Style::Close, sf::ContextSettings(24, 8, 4) };// , ;
	//m_window->setVerticalSyncEnabled(true);
	//m_window->setFramerateLimit(60);

	activeState = StateController::getActiveState();

	m_font->loadFromFile(GlobalValues::MAIN_PATH() + "fonts/PRISTINA.TTF");
	m_gameClock.restart();

	m_view = m_window->getDefaultView();
	m_fixedView = m_window->getDefaultView();
	m_window->setView(m_view);

	m_thread = new sf::Thread(ScreenController::run);
	m_running = true;
	m_thread->launch();
}

void ScreenController::shutDown()
{
	printf("Stopping thread...");
	m_running = false;
	m_thread->~Thread();
	m_thread = nullptr;
	printf("Stopped thread!");

	printf("Closing window...");
	m_window->close();
	delete m_window;
	m_window = nullptr;
	printf("Closed window!");
}

void ScreenController::run()
{
	while (m_running)
	{
		//if (m_focus)
		//{
			m_window->setActive(true); // necessary?

			// clear window
			m_window->clear();

			m_nrDrawUpdates++;

			m_window->setView(m_view);

			// render drawables
			// OLD StateController::drawScrollable(*m_window);
			StateController::lockActiveState();
			if (activeState != nullptr){
				ScreenController::activeState->drawScrollable(*m_window);
			}

			m_window->setView(m_fixedView);

			// OLD StateController::drawFixed(*m_window);
			if (activeState != nullptr){
				ScreenController::activeState->drawFixed(*m_window);
			}
			StateController::unlockActiveState();

			//Update visible FPS counter every second
			if (m_gameClock.getElapsedTime().asMilliseconds() >= 1000)
			{
				m_gameClock.restart();
				m_avgFps = m_nrDrawUpdates;
				m_nrDrawUpdates = 0;
			}

			sf::Text text;
			text.setString("FPS: " + std::to_string(m_avgFps));
			text.setFont(*m_font);
			text.setPosition(10, 10);
			text.setColor(sf::Color::Red);
			m_window->draw(text);

			// present window
			m_window->display();

			m_window->setActive(false); // necessary?
		//}

		if (m_fpsLimit != 0)
		{
			sf::sleep(sf::microseconds((long)(1.0f / m_fpsLimit * 1000000) - m_fpsClock.getElapsedTime().asMicroseconds()));
			m_fpsClock.restart();
		}
	}
	printf("Thread ended!");
}

sf::RenderWindow* ScreenController::getWindow()
{
	return m_window;
}


void ScreenController::changeResolution(unsigned int width, unsigned int height)
{
	m_windowWidth = width;
	m_windowHeight = height;
	m_window->setSize(sf::Vector2u(m_windowWidth, m_windowHeight));

	// TODO:
	// If changeresolution gets implemented again, it should change views appropiately
	// m_view.set
	// m_fixedView = m_window->getDefaultWindow();
}

void ScreenController::updateFocus(bool focus)
{
	m_focus = focus;
}


void ScreenController::moveView(sf::Vector2f move)
{
	m_view.move(move);
	movementOffset -= move;
}

void ScreenController::centerView(sf::Vector2f center) {
	m_view.setCenter(center);
}

sf::Vector2f ScreenController::getMovementOffset(){
	return movementOffset;
}


void ScreenController::resetView()
{
	movementOffset = sf::Vector2f{ 0, 0 };
	m_view = m_window->getDefaultView();
}

void ScreenController::setActiveState(State * activeState){
	ScreenController::activeState = activeState;
}