#ifndef BULLET_H
#define BULLET_H

#include "Main.h"

class Bullet
{

public:
	void changePara( float vel, float radius, CVector destino );
	void drawBullet();
	void moveToTarget();
	void destroy();

private:
	boundingsphere bala;
	float speed;
	CVector sTarget;

};

#endif // BULLET_H