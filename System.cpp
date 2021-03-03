#include "System.h"
#include <DxLib.h>


bool IsHit(Object obj1, Object obj2)
{
	bool result = IsHit(obj1.posX, obj1.posY, obj1.sizeX, obj1.sizeY,
		                obj2.posX, obj2.posY, obj2.sizeX, obj2.sizeY);

	return result;
}

bool IsHit( float _posX1, float _posY1,
			float _sizeX1, float _sizeY1,
			float _posX2, float _posY2,
			float _sizeX2, float _sizeY2 )
{
	// �@の右側が�Aの左側より大きい
	// �@の左側が�Aの右側より小さい
	// �@の下側が�Aの上側より大きい
	// �@の上側が�Aの下側より小さい
	if ((_posX1 + (_sizeX1 / 2.0f) > _posX2 - (_sizeX2 / 2.0f)) &&
		(_posX1 - (_sizeX1 / 2.0f) < _posX2 + (_sizeX2 / 2.0f)) &&
		(_posY1 + (_sizeY1 / 2.0f) > _posY2 - (_sizeY2 / 2.0f)) &&
		(_posY1 - (_sizeY1 / 2.0f) < _posY2 + (_sizeY2 / 2.0f)))
	{
		return true;
	}

	return false;
}

float Start(int obj)
{
	if (obj == 0)
	{
		int result = DrawString(250, 240, "aaaaaaaaa！", GetColor(0, 300, 200));
		return result;
	}
}
