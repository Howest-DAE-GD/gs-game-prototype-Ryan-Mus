#include "pch.h"
#include "Game.h"
#include "utils.h"
#include "structs.h"
#include "texture.h"
#include <iostream>
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
	CreateRandomLand(1);
	CreateRandomLand(1);
	CreateRandomLand(2);
	CreateRandomLand(2);
	CreateRandomLand(3);
	m_DrawPoints.reserve(5);
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
	if (MainLand.GetHP() > 0)
	{
		MainLand.Update(elapsedSec);
		for (Land* coloniesL1 : m_Colonies)
		{
			coloniesL1->Update(elapsedSec);
			if (MainLand.DoDamage(*coloniesL1, elapsedSec))
			{
				m_DrawPoints.push_back(coloniesL1->GetPos());
			}
			if (coloniesL1->GetColor() == MainLand.GetColor())
			{
				coloniesL1->TransferTroops(MainLand);
			}
		}
	}
	
}

void Game::Draw( )
{
	ClearBackground();

	glPushMatrix();
	glTranslatef(GetViewPort().width / 2, GetViewPort().height / 2, 0.f);
	glScalef(scale, scale, scale);
	glTranslatef(-GetViewPort().width / 2, -GetViewPort().height / 2, 0.f);
	glTranslatef(-translateV.x, -translateV.y, 0);

	
	MainLand.Draw();
	MainLand.DrawStats();
	for (Point2f points : m_DrawPoints)
	{
		utils::SetColor(Color4f{ 0.f,0.f,1.f,1.f });
		utils::DrawLine(MainLand.GetPos(), points);
		m_DrawPoints.pop_back();
	}
	for (Land* colonies : m_Colonies)
	{
		colonies->Draw();
	}
	for (Land* colonies : m_Colonies)
	{
		colonies->DrawStats();
	}
	glPopMatrix();

	utils::SetColor(Color4f{ 1.f,1.f,0.f,1.f });
	utils::FillRect(BuyTroopsButton);
	Texture Text{ "Buy Troops (400HP).","JMH Typewriter.ttf",12,Color4f{0.f,0.f,1.f,1.f}};
	Text.Draw(Rectf{ 7,7,Text.GetWidth(),Text.GetHeight() });

	if (MainLand.GetHP() <= 0)
	{
		Texture Text{ "GAME OVER","JMH Typewriter.ttf",50,Color4f{1.f,0.f,0.f,1.f} };
		Text.Draw(Rectf{ 270,300,Text.GetWidth(),Text.GetHeight() });
	}
	bool Victory{ true };
	for (Land* coloniesL1 : m_Colonies)
	{
		if (coloniesL1->GetColor() != MainLand.GetColor())
		{
			Victory = false;
		}
	}
	utils::SetColor(Color4f{ 0.7f,0.7f,0.7f,1.f });
	utils::FillRect(InfoButton);
	Text = Texture{ "INFO","JMH Typewriter.ttf",12,Color4f{0.f,0.f,0.f,1.f} };
	Text.Draw(Rectf{ GetViewPort().width - 80,8,Text.GetWidth(),Text.GetHeight() });
	if (ShowInfo)
	{
		Texture Text{ "LMB: Select","JMH Typewriter.ttf",12,Color4f{1.f,1.f,1.f,1.f} };
		Text.Draw(Rectf{ GetViewPort().width - 150,105,Text.GetWidth(),Text.GetHeight() });
		Text = Texture{ "RMB: Deselect all","JMH Typewriter.ttf",12,Color4f{1.f,1.f,1.f,1.f} };
		Text.Draw(Rectf{ GetViewPort().width - 150,85,Text.GetWidth(),Text.GetHeight() });
		Text = Texture{ "MMB: Move camera/zoom","JMH Typewriter.ttf",12,Color4f{1.f,1.f,1.f,1.f} };
		Text.Draw(Rectf{ GetViewPort().width - 150,65,Text.GetWidth(),Text.GetHeight() });
		Text = Texture{ "Win by colonising all","JMH Typewriter.ttf",12,Color4f{1.f,1.f,1.f,1.f} };
		Text.Draw(Rectf{ GetViewPort().width - 150,45,Text.GetWidth(),Text.GetHeight() });
		Text = Texture{ "healing en troops are transfered with capturing","JMH Typewriter.ttf",12,Color4f{1.f,1.f,1.f,1.f} };
		Text.Draw(Rectf{ GetViewPort().width - 320,25,Text.GetWidth(),Text.GetHeight() });
	}
	if (Victory)
	{
		Texture Text{ "VICTORY","JMH Typewriter.ttf",50,Color4f{0.f,1.f,0.f,1.f} };
		Text.Draw(Rectf{ 300,300,Text.GetWidth(),Text.GetHeight() });
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
	if (isClicking)
	{
		currentPoint = Point2f{ float(e.x),float(e.y) };
		translateV = Vector2f{ clickPoint.x - currentPoint.x,clickPoint.y - currentPoint.y };
		std::cout << translateV << std::endl;
	}
	
}

void Game::ProcessMouseDownEvent( const SDL_MouseButtonEvent& e )
{
	float x{ float((e.x - GetViewPort().width / 2) / scale + GetViewPort().width / 2) };
	float y{ float((e.y - GetViewPort().height / 2) / scale + GetViewPort().height / 2) };
	Point2f mousePosWorld{ x,y };
	mousePosWorld += translateV;
	Point2f mousePosUI{ float(e.x),float(e.y) };

	if (e.button == SDL_BUTTON_LEFT)
	{
		MainLand.Select(mousePosWorld);
		for (Land* colonies : m_Colonies)
		{
			colonies->Select(mousePosWorld);
		}
		if (utils::IsPointInRect(mousePosUI, BuyTroopsButton))
		{
			if (MainLand.GetIsSelected()) MainLand.BuyTroops();
			for (Land* colonies : m_Colonies)
			{
				if (colonies->GetIsSelected() and (colonies->GetColor() == MainLand.GetColor()))colonies->BuyTroops();
			}
		}
		if (utils::IsPointInRect(mousePosUI, InfoButton))
		{
			ShowInfo = !ShowInfo;
		}
	}

	if (e.button == SDL_BUTTON_MIDDLE)
	{
		isClicking = true;
		clickPoint = mousePosUI + translateV;
		std::cout << "clicked right\n";
	}
	if (e.button == SDL_BUTTON_RIGHT)
	{
		for (Land* colonies : m_Colonies)
		{
			colonies->Deselect();
		}
		MainLand.Deselect();
	}
}

void Game::ProcessMouseUpEvent( const SDL_MouseButtonEvent& e )
{
	if (e.button == SDL_BUTTON_MIDDLE)
	{
		isClicking = false;
		std::cout << "stopped clicked right\n";
	}
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

void Game::ProcessMouseWheelEvent(const SDL_MouseWheelEvent& e)
{
	if (e.type == SDL_MOUSEWHEEL)
	{
		if (e.preciseY < 1)
		{
			scale *= 0.9f;
		}
		else
		{
			scale *= 1.1f;
		}
	}


	//scale = 0.5f;
	//std::cout << scale << std::endl;
}

void Game::ClearBackground( ) const
{
	glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
	glClear( GL_COLOR_BUFFER_BIT );
}

void Game::CreateRandomLand(int size)
{
	float Size{};
	switch (size)
	{
		
	case 1:
		Size = float(rand() % 3 * 10 + 20);
		m_Colonies.push_back(new Land{ Ellipsef{(rand() % int(GetViewPort().width - 100.f) + 50.f),(rand() % int(GetViewPort().height - 200.f) + 150.f),Size,Size},Color4f{1.f,0.f,0.f,1.f},int(Size+Size * 100),rand()%10+5,int((Size+Size)/10)});
		break;
	case 2:
		Size = float(rand() % 3 * 10 + 40);
		m_Colonies.push_back(new Land{ Ellipsef{(rand() % int(GetViewPort().width - 100.f) + 50.f),(rand() % int(GetViewPort().height - 200.f) + 150.f),Size,Size} ,Color4f{1.f,0.f,0.f,1.f},int(Size + Size * 100),rand() % 30 + 10,int((Size + Size) / 7 )});
		break;
	case 3:
		Size = float(rand() % 5 * 10 + 60);
		m_Colonies.push_back(new Land{ Ellipsef{(rand() % int(GetViewPort().width - 100.f) + 50.f),(rand() % int(GetViewPort().width - 200.f) + 150.f),Size,Size} ,Color4f{1.f,0.f,0.f,1.f},int(Size + Size * 100),rand() % 90 + 20,int((Size + Size) / 5 )});
		break;
	default:
		break;
	}
	
	for (int i{}; i < (m_Colonies.size()-1); i++)
	{
		if (utils::IsOverlapping(m_Colonies[i]->GetLandSize(), m_Colonies[m_Colonies.size() - 1]->GetLandSize()))
		{
			m_Colonies.pop_back();
			CreateRandomLand(size);
		}
	}
	
}
