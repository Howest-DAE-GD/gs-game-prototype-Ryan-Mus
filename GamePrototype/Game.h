#pragma once
#include "BaseGame.h"
#include "Land.h"
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
	void Draw( ) const override;

	// Event handling
	void ProcessKeyDownEvent( const SDL_KeyboardEvent& e ) override;
	void ProcessKeyUpEvent( const SDL_KeyboardEvent& e ) override;
	void ProcessMouseMotionEvent( const SDL_MouseMotionEvent& e ) override;
	void ProcessMouseDownEvent( const SDL_MouseButtonEvent& e ) override;
	void ProcessMouseUpEvent( const SDL_MouseButtonEvent& e ) override;

private:
	Land MainLand{ Ellipsef{GetViewPort().width / 2 ,60,100,30},Color4f{0.f,1.f,0.f,1.f},10000,10,-20 };
	Land Colony1{ Ellipsef{100.f,GetViewPort().height - 100.f,40,20},Color4f{1.f,0.f,0.f,1.f},1000,3,5 };
	Land Colony2{ Ellipsef{400.f,GetViewPort().height - 80.f,50,25},Color4f{1.f,0.f,0.f,1.f},1000,3,5 };

	// FUNCTIONS
	void Initialize();
	void Cleanup( );
	void ClearBackground( ) const;
};