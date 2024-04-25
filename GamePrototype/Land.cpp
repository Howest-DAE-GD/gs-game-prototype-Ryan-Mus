#include "pch.h"
#include "Land.h"
#include "utils.h"
#include "texture.h"
#include <iostream>

Land::Land(const Ellipsef& landcircle, Color4f color, int hp, int troops, int healing): LandSize{landcircle}, HP{hp}, Troops{troops}, Healing{healing},UAccSec{}, Color{color}
, IsSelected{ false }, StartHP{hp}, DAccSec{}
{
}

void Land::Draw() const
{	
	if (IsSelected)
	{
		utils::SetColor(Color4f{1.f,1.f,1.f,1.f});
		utils::DrawEllipse(LandSize,3);
	}
	utils::SetColor(Color);
	utils::FillEllipse(LandSize);
	utils::SetColor(Color4f{ 1.f,0.f,1.f,1.f });
	utils::FillRect(Rectf{ LandSize.center.x - LandSize.radiusX,LandSize.center.y - 5.f,HP / float(StartHP) * 2 * LandSize.radiusX,10.f });
}

void Land::DrawStats() const
{
	Texture Text{ "HP:" + std::to_string(HP) + "\t\tTroops: " + std::to_string(Troops),"JMH Typewriter.ttf",14,Color4f{1.f,1.f,1.f,1.f} };
	Text.Draw(Rectf{LandSize.center.x - LandSize.radiusX,LandSize.center.y + LandSize.radiusY,Text.GetWidth(),Text.GetHeight()});
}

void Land::Update(float elapsedSec) 
{
	UAccSec += elapsedSec;

	if (UAccSec > 1.f)
	{
		HP += Healing;
		UAccSec -= 1.f;
	}
	if (HP > StartHP)
	{
		HP = StartHP;
	}
	
}

void Land::DoDamage(Land& target, float elapsedSec)
{
	DAccSec += elapsedSec;
	if(DAccSec > 1.f)
	{
		DAccSec -= 1.f;
		if (target.IsSelected and target.Color != Color)
		{
				target.HP -= Troops * 10;
				std::cout << "dealt dmg to " << target.HP << std::endl;
				if (rand() % 11 == 1)
				{
					Troops -= target.Troops/10;
					if (Troops < 0) Troops = 0;
				}
		}
	}
	if (target.HP < 0)
	{
		TakeOver(target);
	}
	
}

void Land::TakeOver(Land& target)
{
	Healing += target.Healing;
	Troops += target.Troops;

	target.HP = 500;
	target.Troops = 0;
	target.Color = Color;
}

void Land::BuyTroops()
{
	HP -= 100;
	Troops++;
}

void Land::Select(Point2f mousePos)
{
	if (utils::IsPointInRect(mousePos, Rectf{ LandSize.center.x - LandSize.radiusX,LandSize.center.y - LandSize.radiusY, 2 * LandSize.radiusX,2 * LandSize.radiusY }))
	{
		IsSelected = !IsSelected;
	}
}

bool Land::GetIsSelected()
{
	return IsSelected;
}

Color4f Land::GetColor()
{
	return Color;
}


