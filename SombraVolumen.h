#ifndef _SOMBRAVOLUMEN_H_
#define _SOMBRAVOLUMEN_H_

#include "Main.h"
#include "3ds.h"

//Cantidad de extrusion
#define ExtendAmount	5.0f

class CShadow3DS
{
public:
	CShadow3DS();
	~CShadow3DS();

	void EstableceConectividad(t3DModel *pModel);
	void calculaPlano(t3DModel *pModel);
	void calculaSombraDepthPass(t3DModel *pModel, CVector lightPosition);
	void calculaSombraDepthFail(t3DModel *pModel, CVector lightPosition);
	void DrawShadowVolume(t3DModel *pModel, CVector lightPos);
		
private:
	
};

#endif