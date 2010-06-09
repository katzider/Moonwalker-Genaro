#ifndef ANIMATOR_H
#define ANIMATOR_H

#include "Main.h"
#include "Bullet.h"

using namespace std;

class Animator
{

public: 

	// Constructor
	Animator();

	// Agrega un punto a nuestra animacion
	void setChar( parametros* personaje );
	void addPoint( CVector punto );
	void changeSpeed( float speed );
	void startAnim();
	void moveToPoint( CVector punto );
	void setTarget( CVector destino );
	void drawRay();
	int Reload();
	CVector* attack();

private:
	parametros* modelo;
	vector< CVector > points;
	ray rayo;
	int i;
	int delay;

};

#endif // ANIMATOR_H