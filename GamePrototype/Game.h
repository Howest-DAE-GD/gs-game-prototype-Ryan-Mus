#pragma once
#include "BaseGame.h"
#include "Land.h"
#include <vector>
#include "Vector2f.h"
class Game : public BaseGame
{
public:
	explicit Game( const Window& window );
	Game(const Game& other) = delete;
	Game& operator=(const Game& other) = delete;
	Game( Game&& other) = delete;
	Game& operator=(Game&& other) = delete;
	// http://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#Rh-override
	~Game();

	void Update( float elapsedSec ) override;
	void Draw( ) override;

	// Event handling
	void ProcessKeyDownEvent( const SDL_KeyboardEvent& e ) override;
	void ProcessKeyUpEvent( const SDL_KeyboardEvent& e ) override;
	void ProcessMouseMotionEvent( const SDL_MouseMotionEvent& e ) override;
	void ProcessMouseDownEvent( const SDL_MouseButtonEvent& e ) override;
	void ProcessMouseUpEvent( const SDL_MouseButtonEvent& e ) override;
	void ProcessMouseWheelEvent(const SDL_MouseWheelEvent& e)override;
	

private:
	Land MainLand{ Ellipsef{GetViewPort().width / 2 ,60,100,30},Color4f{0.f,1.f,0.f,1.f},12000,5,-30 };

	std::vector<Land*> m_Colonies;
	std::vector<Point2f> m_DrawPoints;
	
	Rectf BuyTroopsButton{ 5,5,120,20 };
	Rectf InfoButton{ GetViewPort().width - 100,5,80,20 };
	bool ShowInfo{ false };
	float scale{ 1.f };

	Point2f clickPoint{};
	Point2f currentPoint{};
	Vector2f translateV{};

	bool isClicking{};
	// FUNCTIONS
	
	void Initialize();
	void Cleanup( );
	void ClearBackground( ) const;
	void CreateRandomLand(int size);
};