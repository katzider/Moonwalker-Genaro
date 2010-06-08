#ifndef _OCTREE_H
#define _OCTREE_H

#include "Main.h"
#include "3ds.h"

//cantidad de nodos en el arbol con vertices almacenados en ellos
extern int g_EndNodeCount;

//cantidad de nodos que se encuentran en el frustrum
extern int g_TotalNodesDrawn;

//cantidad máxima de triángulos por nodo. Cuando un nodo tiene una cantidad menor o igual
//que esta, se detiene la subdivision de nodos y se almacenan los indices de esas caras en el nodo
extern int g_MaxTriangles;

//cantidad máxima de subdivisiones permitida (Niveles de subdivision)
extern int g_MaxSubdivisiones;

//Estos valores (0 - 7) almacenan el indice ID's para el arreglo de nodos del octree (m_pOctreeNodes)
enum eOctreeNodes
{
	TOP_LEFT_FRONT,			// 0
	TOP_LEFT_BACK,			// 1
	TOP_RIGHT_BACK,			// etc...
	TOP_RIGHT_FRONT,
	BOTTOM_LEFT_FRONT,
	BOTTOM_LEFT_BACK,
	BOTTOM_RIGHT_BACK,
	BOTTOM_RIGHT_FRONT
};

struct tFaceList
{
	vector<bool> pFaceList;	

	int totalFaceCount;
};

class CDebug 
{

public:

	void AddDebugRectangle(CVector vCenter, float width, float height, float depth);
	void RenderDebugLines();		
	void Clear();					

private:

	vector<CVector> m_vLines;		
};

class COctree
{

public:

	COctree();
	~COctree();

	CVector GetCenter()							{return m_vCenter;}
	int GetTriangleCount()						{return m_TriangleCount;}
	float GetWidth()							{return m_Width;}
	bool IsSubDivided()							{return m_bSubDivided;}
	void GetSceneDimensions(t3DModel *pWorld);
	int GetSceneTriangleCount(t3DModel *pWorld);
	int GetDisplayListID()						{return m_DisplayListID;}
	void SetDisplayListID(int displayListID)	{m_DisplayListID = displayListID;}
	void AddObjectIndexToList(int index);
	CVector GetNewNodeCenter(CVector vCenter, float width, int nodeID);
	void CreateNode(t3DModel *pWorld, int numberOfTriangles, CVector vCenter, float width);
	void CreateNewNode(t3DModel *pWorld, vector<tFaceList> pList, int triangleCount, CVector vCenter, float width, int nodeID);
	void AssignTrianglesToNode(t3DModel *pWorld, int numberOfTriangles);
	void DrawOctree(COctree *pNode, t3DModel *pRootWorld);
	void CreateDisplayList(COctree *pNode, t3DModel *pRootWorld, int displayListOffset, CTga *Textura);

private:

	bool m_bSubDivided;
	float m_Width;
	int m_TriangleCount;
	CVector m_vCenter;
	CVector VertsAux;
	t3DModel *m_pWorld;
	vector<int>	m_pObjectList;
	int m_DisplayListID;
	COctree *m_pOctreeNodes[8];	
};


#endif
