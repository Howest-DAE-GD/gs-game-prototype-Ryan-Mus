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
	if (target.IsSelected and target.Color != Color)
	{
		if (DAccSec > 1.f)
		{
			DAccSec = 0;
			target.HP -= Troops * 5;
			std::cout << "dealt dmg to " << target.HP << std::endl;
			if (rand() % 5 == 1 and Troops < target.Troops)
			{
				Troops -= 1*(rand()%(target.Troops/10+1));
				if (Troops < 0) Troops = 0;
			}
			if (rand() % 5 == 1 and Troops > target.Troops and target.Troops>0)
			{
				target.Troops -= 1 * (rand() % (Troops / 10 + 1));
				if (target.Troops <= 0)
				{
					target.Troops = 0;
				}
			}
			else
			{
				if (rand() % 10 == 0)
				{
					Troops--;
					if (Troops < 0) Troops = 0;
				}
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

	target.HP = 1000;
	target.Troops = 0;
	target.Color = Color;
}

void Land::BuyTroops()
{
	if (HP > 400)
	{
		HP -= 400;
		Troops++;
	}
}

void Land::TransferTroops(Land& target)
{
	target.Troops += Troops;
	Troops = 0;
}

void Land::Select(Point2f mousePos)
{
	if (utils::IsPointInRect(mousePos, Rectf{ LandSize.center.x - LandSize.radiusX,LandSize.center.y - LandSize.radiusY, 2 * LandSize.radiusX,2 * LandSize.radiusY }))
	{
		IsSelected = !IsSelected;
	}
}

bool Land::GetIsSelected()const
{
	return IsSelected;
}

Color4f Land::GetColor()const
{
	return Color;
}

int Land::GetHP()const
{
	return HP;
}

Circlef Land::GetLandSize() const
{
	return Circlef{ LandSize.center,LandSize.radiusX };
}


