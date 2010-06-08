#ifndef ANIMATOR_H
#define ANIMATOR_H

#include "Main.h"

using namespace std;

class Animator
{

public: 
	// Constructores y deconstructores
	Animator();
	~Animator();

	// Agrega un punto a nuestra animacion
	void setChar( parametros& personaje );
	void addPoint( CVector punto );
	void changeSpeed( float speed );
	void startAnim();
	void moveToPoint( CVector punto );

private:
	parametros& modelo;
	vector< CVector > points;

};

#endif // ANIMATOR_H