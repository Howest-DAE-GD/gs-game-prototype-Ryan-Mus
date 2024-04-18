#pragma once
class Land
{
public:
	Land(const Ellipsef& landcircle,Color4f color,int hp, int troops, int healing);

	void Draw() const;
	void Update(float elapsedSec);
	void DoDamage(Land& target, float elapsedSec);

	void TakeOver(Land& target);
	void BuyTroops();
	void Select(Point2f mousePos);
	
private:
	Color4f Color;
	Ellipsef LandSize;
	int StartHP;
	int HP;
	int Troops;
	int Healing;
	float UAccSec;
	float DAccSec;
	bool IsSelected;

};

