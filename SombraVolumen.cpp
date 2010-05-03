#include "SombraVolumen.h"

CShadow3DS::CShadow3DS()
{
}

CShadow3DS::~CShadow3DS()
{
}

void CShadow3DS::EstableceConectividad(t3DModel *pModel)
{
	int p1i, p2i, p1j, p2j;
	int P1i, P2i, P1j, P2j;
	
	for(int m = 0; m < pModel->numOfObjects; m++)
	{
		if(pModel->pObject.size() <= 0) break;
		
		t3DObject *pObject = &pModel->pObject[m];

		for(int i=0; i < pObject->numOfFaces-1; i++)
		{
			for(int j=i+1; j < pObject->numOfFaces; j++)
			{
				for(int ki=0; ki<3; ki++)
				{
					if(pObject->pFaces[i].neighbourIndices[ki] == 0)
					{
						for(int kj=0; kj<3; kj++)
						{						
							p1i=ki;
							p1j=kj;
							p2i=(ki+1)%3;
							p2j=(kj+1)%3;
							
							p1i=pObject->pFaces[i].vertIndex[p1i];
							p2i=pObject->pFaces[i].vertIndex[p2i];
							p1j=pObject->pFaces[j].vertIndex[p1j];
							p2j=pObject->pFaces[j].vertIndex[p2j];

							P1i=((p1i+p2i)-abs(p1i-p2i))/2;
							P2i=((p1i+p2i)+abs(p1i-p2i))/2;
							P1j=((p1j+p2j)-abs(p1j-p2j))/2;
							P2j=((p1j+p2j)+abs(p1j-p2j))/2;

							if(((P1i==P1j) && (P2i==P2j)))
							{  //polígonos adyacentes
								pObject->pFaces[i].neighbourIndices[ki] = j+1;	  
								pObject->pFaces[j].neighbourIndices[kj] = i+1;	  
							}
						}
					}
				}
			}
		}
	}
	
}

void CShadow3DS::calculaPlano(t3DModel *pModel)
{
	int index;

	for(int m = 0; m < pModel->numOfObjects; m++)
	{
		if(pModel->pObject.size() <= 0) break;
		
		t3DObject *pObject = &pModel->pObject[m];

		for(int i=0; i < pObject->numOfFaces; i++)
		{
			index = pObject->pFaces[i].vertIndex[0];
			CVector v1 = CVector(pObject->pVerts[index].x, pObject->pVerts[index].y, pObject->pVerts[index].z);

			index = pObject->pFaces[i].vertIndex[1];
			CVector v2 = CVector(pObject->pVerts[index].x, pObject->pVerts[index].y, pObject->pVerts[index].z);

			index = pObject->pFaces[i].vertIndex[2];
			CVector v3 = CVector(pObject->pVerts[index].x, pObject->pVerts[index].y, pObject->pVerts[index].z);

			pObject->pFaces[i].planeEquation.a = v1.y*(v2.z-v3.z) + v2.y*(v3.z-v1.z) + v3.y*(v1.z-v2.z);
			pObject->pFaces[i].planeEquation.b = v1.z*(v2.x-v3.x) + v2.z*(v3.x-v1.x) + v3.z*(v1.x-v2.x);
			pObject->pFaces[i].planeEquation.c = v1.x*(v2.y-v3.y) + v2.x*(v3.y-v1.y) + v3.x*(v1.y-v2.y);
			pObject->pFaces[i].planeEquation.d = -(v1.x*(v2.y*v3.z - v3.y*v2.z) +
									v2.x*(v3.y*v1.z - v1.y*v3.z) +
									v3.x*(v1.y*v2.z - v2.y*v1.z));
		}
	}
	
}

void CShadow3DS::calculaSombraDepthPass(t3DModel *pModel, CVector lightPosition)
{
	float	 side;

	for(int m = 0; m < pModel->numOfObjects; m++)
	{
		if(pModel->pObject.size() <= 0) break;
		
		t3DObject *pObject = &pModel->pObject[m];

		for(int i=0; i < pObject->numOfFaces; i++)
		{
			Plane *plane = &pObject->pFaces[i].planeEquation;

			side =  plane->a*lightPosition.x+
					plane->b*lightPosition.y+
					plane->c*lightPosition.z+
					plane->d;

			if(side > 0)
				pObject->pFaces[i].visible = true;
			else
				pObject->pFaces[i].visible = false;
		}
	}

	//Aqui comienza el procedimiento de Depth Pass:
	//Se renderizan las caras delanteras del volumen de sombras y se incrementa el valor del buffer stencil
	//cuando se pasa la prueba de profundidad
	glStencilOp(GL_KEEP, GL_KEEP, GL_INCR);
	glCullFace(GL_BACK);
	DrawShadowVolume(pModel, lightPosition);

	//Se renderizan las caras traseras del volumen de sombras y se decrementa el valor del buffer stencil
	//cuando se pasa la prueba de profundidad
	glStencilOp(GL_KEEP, GL_KEEP, GL_DECR);
	glCullFace(GL_FRONT);
	DrawShadowVolume(pModel, lightPosition);
	
}

void CShadow3DS::calculaSombraDepthFail(t3DModel *pModel, CVector lightPosition)
{
	float	 side;

	for(int m = 0; m < pModel->numOfObjects; m++)
	{
		if(pModel->pObject.size() <= 0) break;
		
		t3DObject *pObject = &pModel->pObject[m];

		for(int i=0; i < pObject->numOfFaces; i++)
		{
			Plane *plane = &pObject->pFaces[i].planeEquation;

			side =  plane->a*lightPosition.x+
					plane->b*lightPosition.y+
					plane->c*lightPosition.z+
					plane->d;

			if(side > 0)
				pObject->pFaces[i].visible = true;
			else
				pObject->pFaces[i].visible = false;
		}
	}

	//Aqui comienza el procedimiento de Depth Fail:
	//Se renderizan las caras traseras del volumen de sombras y se incrementa el valor del buffer stencil
	//cuando se falla la prueba de profundidad
	glStencilOp(GL_KEEP, GL_INCR, GL_KEEP);
	glCullFace(GL_FRONT);
	DrawShadowVolume(pModel, lightPosition);

	//Se renderizan las caras delanteras del volumen de sombras y se decrementa el valor del buffer stencil
	//cuando se falla la prueba de profundidad
	glStencilOp(GL_KEEP, GL_DECR, GL_KEEP);
	glCullFace(GL_BACK);
	DrawShadowVolume(pModel, lightPosition);
}

void CShadow3DS::DrawShadowVolume(t3DModel *pModel, CVector lightPos)
{
	CVector Extended;
	CVector currentVertex;
	CVector v1, v2, v3, v4;

	// Esta función usará la silueta del objeto obstructor para crear el volumen de sombra
	glDisable(GL_LIGHTING);

	// Este es el color con el que se dibujarán las líneas de extrusión si queremos mostrarlas.
	glColor3f(1.0f, 1.0f, 0.0f);

	for(int m = 0; m < pModel->numOfObjects; m++)
	{
		if(pModel->pObject.size() <= 0) break;
		
		t3DObject *pObject = &pModel->pObject[m];

		for(int i=0; i < pObject->numOfFaces; i++)
		{
			tFace *pFaces = &pObject->pFaces[i];

			if(pFaces->visible)
			{
				//Se revisa cada lado del triangulo
				for(int j = 0; j < 3; j++)
				{
					int neighbourIndex = pFaces->neighbourIndices[j];
					
					//Si no hay un polígono adyacente, o lo hay pero no es visible, Entonces este borde proyecta sombra
					if((neighbourIndex == 0) || (!pObject->pFaces[neighbourIndex-1].visible))
					{
						//Se guardan los puntos de este borde
						v1.x = pObject->pVerts[pFaces->vertIndex[j]].x;
						v1.y = pObject->pVerts[pFaces->vertIndex[j]].y;
						v1.z = pObject->pVerts[pFaces->vertIndex[j]].z;

						v2.x = pObject->pVerts[pFaces->vertIndex[(j+1)%3]].x;
						v2.y = pObject->pVerts[pFaces->vertIndex[(j+1)%3]].y;
						v2.z = pObject->pVerts[pFaces->vertIndex[(j+1)%3]].z;

						//Se extienden esos dos puntos para formar la cara actual del volumen de sombra
						currentVertex=v1;
						Extended.ExtendVertexPos(currentVertex, lightPos, ExtendAmount);
						v3=Extended;
						
						currentVertex=v2;
						Extended.ExtendVertexPos(currentVertex, lightPos, ExtendAmount);
						v4=Extended;
						
						//Se dibuja la cara del volumen de sombra con los 4 vértices
						glBegin(GL_TRIANGLE_STRIP);
							glVertex3f(v1.x, v1.y, v1.z);
							glVertex3f(v3.x, v3.y, v3.z);
							glVertex3f(v2.x, v2.y, v2.z);
							glVertex3f(v4.x, v4.y, v4.z);
						glEnd();
					}
				}
			}
		}
	}
	
	glEnable(GL_LIGHTING);

}