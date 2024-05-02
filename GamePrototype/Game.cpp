#include "pch.h"
#include "Game.h"
#include "utils.h"
#include "structs.h"
Game::Game( const Window& window ) 
	:BaseGame{ window }
{
	Initialize();
}

Game::~Game( )
{
	Cleanup( );
}

void Game::Initialize( )
{
	m_Colonies.push_back( new Land{Ellipsef{100.f,GetViewPort().height - 100.f,40,20},Color4f{1.f,0.f,0.f,1.f},3000,30,10});
	m_Colonies.push_back( new Land{ Ellipsef{400.f,GetViewPort().height - 80.f,50,25},Color4f{1.f,0.f,0.f,1.f},5000,5,7 });
	m_Colonies.push_back(new Land{ Ellipsef{700.f,GetViewPort().height - 150.f,80,45},Color4f{1.f,0.f,0.f,1.f},8000,20,15 });
	m_Colonies.push_back(new Land{ Ellipsef{500.f,GetViewPort().height - 200.f,20,15},Color4f{1.f,0.f,0.f,1.f},1000,15,5 });
	m_Colonies.push_back(new Land{ Ellipsef{300.f,GetViewPort().height - 250.f,100,60},Color4f{1.f,0.f,0.f,1.f},20000,50,20 });
}

void Game::Cleanup( )
{
	for (Land* colonies : m_Colonies)
	{
		delete colonies;
	}
}

void Game::Update( float elapsedSec )
{
	
	MainLand.Update(elapsedSec);
	for (Land* coloniesL1 : m_Colonies)
	{
		coloniesL1->Update(elapsedSec);
		MainLand.DoDamage(*coloniesL1,elapsedSec);
		if (coloniesL1->GetColor() == MainLand.GetColor())
		{
			coloniesL1->TransferTroops(MainLand);
		}
	}
}

void Game::Draw( ) const
{
	ClearBackground();
	utils::SetColor(Color4f{ 1.f,1.f,0.f,1.f });
	utils::FillRect(BuyTroopsButton);
	MainLand.Draw();
	MainLand.DrawStats();
	
	for (Land* colonies : m_Colonies)
	{
		colonies->Draw();
		colonies->DrawStats();
	}
	
}

void Game::ProcessKeyDownEvent( const SDL_KeyboardEvent & e )
{
	//std::cout << "KEYDOWN event: " << e.keysym.sym << std::endl;
}

void Game::ProcessKeyUpEvent( const SDL_KeyboardEvent& e )
{
	//std::cout << "KEYUP event: " << e.keysym.sym << std::endl;
	//switch ( e.keysym.sym )
	//{
	//case SDLK_LEFT:
	//	//std::cout << "Left arrow key released\n";
	//	break;
	//case SDLK_RIGHT:
	//	//std::cout << "`Right arrow key released\n";
	//	break;
	//case SDLK_1:
	//case SDLK_KP_1:
	//	//std::cout << "Key 1 released\n";
	//	break;
	//}
}

void Game::ProcessMouseMotionEvent( const SDL_MouseMotionEvent& e )
{
	//std::cout << "MOUSEMOTION event: " << e.x << ", " << e.y << std::endl;
}

void Game::ProcessMouseDownEvent( const SDL_MouseButtonEvent& e )
{
	Point2f mousePos{ float(e.x),float(e.y) };

	MainLand.Select(mousePos);
	for (Land* colonies : m_Colonies)
	{
		colonies->Select(mousePos);
	}
	if (utils::IsPointInRect(mousePos, BuyTroopsButton))
	{
		if (MainLand.GetIsSelected()) MainLand.BuyTroops();
			for (Land* colonies : m_Colonies)
			{
				if (colonies->GetIsSelected() and (colonies->GetColor() == MainLand.GetColor()))colonies->BuyTroops();
			}
	}

	//std::cout << "MOUSEBUTTONDOWN event: ";
	//switch ( e.button )
	//{
	//case SDL_BUTTON_LEFT:
	//	std::cout << " left button " << std::endl;
	//	break;
	//case SDL_BUTTON_RIGHT:
	//	std::cout << " right button " << std::endl;
	//	break;
	//case SDL_BUTTON_MIDDLE:
	//	std::cout << " middle button " << std::endl;
	//	break;
	//}
	
}

void Game::ProcessMouseUpEvent( const SDL_MouseButtonEvent& e )
{
	//std::cout << "MOUSEBUTTONUP event: ";
	//switch ( e.button )
	//{
	//case SDL_BUTTON_LEFT:
	//	std::cout << " left button " << std::endl;
	//	break;
	//case SDL_BUTTON_RIGHT:
	//	std::cout << " right button " << std::endl;
	//	break;
	//case SDL_BUTTON_MIDDLE:
	//	std::cout << " middle button " << std::endl;
	//	break;
	//}
}

void Game::ClearBackground( ) const
{
	glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
	glClear( GL_COLOR_BUFFER_BIT );
}
