#include "pch.h"
#include "Land.h"
#include "utils.h"

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

void Land::Update(float elapsedSec) 
{
	UAccSec += elapsedSec;

	if (UAccSec > 1.f)
	{
		HP += Healing;
		UAccSec -= 1.f;
	}
	
}

void Land::DoDamage(Land& target, float elapsedSec)
{
	DAccSec += elapsedSec;
	if(DAccSec > 1.f)
	{
		DAccSec -= 1.f;
		if (target.IsSelected)
		{
				target.HP -= Troops * 10;
		}
	}
}

void Land::TakeOver(Land& target)
{
	Healing += target.Healing;
	Troops += target.Troops;
}

void Land::BuyTroops()
{
	HP -= 10;
	Troops++;
}

void Land::Select(Point2f mousePos)
{
	if (utils::IsPointInRect(mousePos, Rectf{ LandSize.center.x - LandSize.radiusX,LandSize.center.y - LandSize.radiusY, 2 * LandSize.radiusX,2 * LandSize.radiusY }))
	{
		IsSelected = true;
	}
	else IsSelected = false;
}


