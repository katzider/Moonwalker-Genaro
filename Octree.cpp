#include "Octree.h"

int g_CurrentSubdivision = 0;

void CDebug::RenderDebugLines()
{
	glDisable(GL_LIGHTING);

	glBegin(GL_LINES);

		glColor3ub(255, 255, 0);

		for(unsigned int i = 0; i < m_vLines.size(); i++)
		{
			glVertex3f(m_vLines[i].x, m_vLines[i].y, m_vLines[i].z);
		}	

	glEnd();

	glEnable(GL_LIGHTING);
}

void CDebug::AddDebugRectangle(CVector vCenter, float width, float height, float depth)
{
	width /= 2.0f;	height /= 2.0f;	depth /= 2.0f;

	CVector vTopLeftFront( vCenter.x - width, vCenter.y + height, vCenter.z + depth);
	CVector vTopLeftBack(  vCenter.x - width, vCenter.y + height, vCenter.z - depth);
	CVector vTopRightBack( vCenter.x + width, vCenter.y + height, vCenter.z - depth);
	CVector vTopRightFront(vCenter.x + width, vCenter.y + height, vCenter.z + depth);

	CVector vBottom_LeftFront( vCenter.x - width, vCenter.y - height, vCenter.z + depth);
	CVector vBottom_LeftBack(  vCenter.x - width, vCenter.y - height, vCenter.z - depth);
	CVector vBottomRightBack( vCenter.x + width, vCenter.y - height, vCenter.z - depth);
	CVector vBottomRightFront(vCenter.x + width, vCenter.y - height, vCenter.z + depth);

	m_vLines.push_back(vTopLeftFront);		m_vLines.push_back(vTopRightFront);
	m_vLines.push_back(vTopLeftBack);  		m_vLines.push_back(vTopRightBack);
	m_vLines.push_back(vTopLeftFront);		m_vLines.push_back(vTopLeftBack);
	m_vLines.push_back(vTopRightFront);		m_vLines.push_back(vTopRightBack);

	m_vLines.push_back(vBottom_LeftFront);	m_vLines.push_back(vBottomRightFront);
	m_vLines.push_back(vBottom_LeftBack);	m_vLines.push_back(vBottomRightBack);
	m_vLines.push_back(vBottom_LeftFront);	m_vLines.push_back(vBottom_LeftBack);
	m_vLines.push_back(vBottomRightFront);	m_vLines.push_back(vBottomRightBack);

	m_vLines.push_back(vTopLeftFront);		m_vLines.push_back(vBottom_LeftFront);
	m_vLines.push_back(vTopLeftBack);		m_vLines.push_back(vBottom_LeftBack);
	m_vLines.push_back(vTopRightBack);		m_vLines.push_back(vBottomRightBack);
	m_vLines.push_back(vTopRightFront);		m_vLines.push_back(vBottomRightFront);
}

void CDebug::Clear()						
{
	m_vLines.clear();
}

COctree::COctree()
{
	m_bSubDivided = false;

	m_Width = 0; 

	m_TriangleCount = 0;

	m_vCenter = CVector(0, 0, 0);
	m_pWorld = NULL;

	memset(m_pOctreeNodes, 0, sizeof(m_pOctreeNodes));		
}

COctree::~COctree()
{
	if( m_pWorld )
	{
		for(int i = 0; i < m_pWorld->numOfObjects; i++)
		{
			if( m_pWorld->pObject[i].pFaces )
			{
				delete [] m_pWorld->pObject[i].pFaces;	
				m_pWorld->pObject[i].pFaces = NULL;
			}		

			if( m_pWorld->pObject[i].pIndices )
			{
				delete [] m_pWorld->pObject[i].pIndices;	
				m_pWorld->pObject[i].pIndices = NULL;
			}
		}
	
		delete m_pWorld;
		m_pWorld = NULL;
	}

	for(int i = 0; i < 8; i++)
	{
		if( m_pOctreeNodes[i] )
		{
			delete m_pOctreeNodes[i];
			m_pOctreeNodes[i] = NULL;
		}
	}
}

int COctree::GetSceneTriangleCount(t3DModel *pWorld)
{
	int numberOfTriangles = 0;

	for(int i = 0; i < pWorld->numOfObjects; i++)
	{
		numberOfTriangles += pWorld->pObject[i].numOfFaces;
	}

	return numberOfTriangles;
}

void COctree::GetSceneDimensions(t3DModel *pWorld)
{
	int numOfVerts;
	float Width, Height, Depth;
	float maxWidth, maxHeight, maxDepth;

	if(!pWorld) return;

	numOfVerts=0;

	for(int i=0; i < pWorld->numOfObjects; i++)
	{
		numOfVerts += pWorld->pObject[i].numOfVerts;

		for(int j=0; j < pWorld->pObject[i].numOfVerts; j++)
		{
			VertsAux.x = pWorld->pObject[i].pVerts[j].x;
			VertsAux.y = pWorld->pObject[i].pVerts[j].y;
			VertsAux.z = pWorld->pObject[i].pVerts[j].z;
			m_vCenter = m_vCenter + VertsAux;
		}
	}

	m_vCenter.x /= numOfVerts;
	m_vCenter.y /= numOfVerts;	
	m_vCenter.z /= numOfVerts;

	Width=0.0f;
	Height=0.0f;
	Depth=0.0f;
	maxWidth=0.0f;
	maxHeight=0.0f;
	maxDepth=0.0f;
	
	for(int i = 0; i < pWorld->numOfObjects; i++)
	{
		for(int j = 0; j < pWorld->pObject[i].numOfVerts; j++)
		{
			Width  = fabsf(pWorld->pObject[i].pVerts[j].x - m_vCenter.x);	
			Height = fabsf(pWorld->pObject[i].pVerts[j].y - m_vCenter.y);		
			Depth  = fabsf(pWorld->pObject[i].pVerts[j].z - m_vCenter.z);

			if(Width > maxWidth)
				maxWidth=Width;

			if(Height > maxHeight)
				maxHeight=Height;

			if(Depth > maxDepth)
				maxDepth=Depth;
		}
	}

	maxWidth*= 2.0f;
	maxHeight*= 2.0f;
	maxDepth*= 2.0f;

	if(maxWidth > maxHeight && maxWidth > maxDepth)
		m_Width = maxWidth;
	else if(maxHeight > maxWidth && maxHeight > maxDepth)
		m_Width = maxHeight;
	else
		m_Width = maxDepth;
	
}

CVector COctree::GetNewNodeCenter(CVector vCenter, float width, int nodeID)
{
	CVector vNodeCenter(0, 0, 0);

	CVector vCtr = vCenter;

	switch(nodeID)							
	{
		case TOP_LEFT_FRONT:
			vNodeCenter = CVector(vCtr.x - width/4, vCtr.y + width/4, vCtr.z + width/4);
			break;

		case TOP_LEFT_BACK:
			vNodeCenter = CVector(vCtr.x - width/4, vCtr.y + width/4, vCtr.z - width/4);
			break;

		case TOP_RIGHT_BACK:
			vNodeCenter = CVector(vCtr.x + width/4, vCtr.y + width/4, vCtr.z - width/4);
			break;

		case TOP_RIGHT_FRONT:
			vNodeCenter = CVector(vCtr.x + width/4, vCtr.y + width/4, vCtr.z + width/4);
			break;

		case BOTTOM_LEFT_FRONT:
			vNodeCenter = CVector(vCtr.x - width/4, vCtr.y - width/4, vCtr.z + width/4);
			break;

		case BOTTOM_LEFT_BACK:
			vNodeCenter = CVector(vCtr.x - width/4, vCtr.y - width/4, vCtr.z - width/4);
			break;

		case BOTTOM_RIGHT_BACK:
			vNodeCenter = CVector(vCtr.x + width/4, vCtr.y - width/4, vCtr.z - width/4);
			break;

		case BOTTOM_RIGHT_FRONT:
			vNodeCenter = CVector(vCtr.x + width/4, vCtr.y - width/4, vCtr.z + width/4);
			break;
	}

	return vNodeCenter;
}

void COctree::CreateNewNode(t3DModel *pWorld, vector<tFaceList> pList, int triangleCount, CVector vCenter, float width, int nodeID)
{
	if(!triangleCount) return;
	
	t3DModel *pTempWorld = new t3DModel;

	memset(pTempWorld, 0, sizeof(t3DModel));
	pTempWorld->numOfObjects = pWorld->numOfObjects;
	
	for(int i = 0; i < pWorld->numOfObjects; i++)
	{
		t3DObject *pObject = &(pWorld->pObject[i]);

		t3DObject newObject;
		memset(&newObject, 0, sizeof(t3DObject));
		pTempWorld->pObject.push_back(newObject);

		pTempWorld->pObject[i].numOfFaces  = pList[i].totalFaceCount;
		pTempWorld->pObject[i].materialID  = pObject->materialID;
		pTempWorld->pObject[i].bHasTexture = pObject->bHasTexture;
		pTempWorld->pObject[i].pVerts      = pObject->pVerts;

		pTempWorld->pObject[i].pFaces = new tFace [pTempWorld->pObject[i].numOfFaces];

		int index = 0;

		for(int j = 0; j < pObject->numOfFaces; j++)
		{
			if(pList[i].pFaceList[j])	
			{
				pTempWorld->pObject[i].pFaces[index] = pObject->pFaces[j];
				index++;
			}
		}
	}

	m_pOctreeNodes[nodeID] = new COctree;

	CVector vNodeCenter = GetNewNodeCenter(vCenter, width, nodeID);
		
	g_CurrentSubdivision++;

	m_pOctreeNodes[nodeID]->CreateNode(pTempWorld, triangleCount, vNodeCenter, width / 2);

	g_CurrentSubdivision--;

	for(int i = 0; i < pWorld->numOfObjects; i++)
	{
		if(pTempWorld->pObject[i].pFaces)
			delete [] pTempWorld->pObject[i].pFaces;
	}

	delete pTempWorld;

}

void COctree::CreateNode(t3DModel *pWorld, int numberOfTriangles, CVector vCenter, float width)
{
	m_vCenter = vCenter;
	m_Width = width;

	g_Debug.AddDebugRectangle(vCenter, width, width, width);
	
	if( (numberOfTriangles > g_MaxTriangles) && (g_CurrentSubdivision < g_MaxSubdivisiones) )
	{
		m_bSubDivided = true;

		vector<tFaceList> pList1(pWorld->numOfObjects);		// Sup-Izq-Front
		vector<tFaceList> pList2(pWorld->numOfObjects);		// Sup-Izq-Tras
		vector<tFaceList> pList3(pWorld->numOfObjects);		// Sup-Der-Tras
		vector<tFaceList> pList4(pWorld->numOfObjects);		// Sup-Der-Front
		vector<tFaceList> pList5(pWorld->numOfObjects);		// Inf-Izq-Front
		vector<tFaceList> pList6(pWorld->numOfObjects);		// Inf-Izq-Tras
		vector<tFaceList> pList7(pWorld->numOfObjects);		// Inf-Der-Tras
		vector<tFaceList> pList8(pWorld->numOfObjects);		// Inf-Der-Front
	
		CVector vCtr = vCenter;

		for(int i = 0; i < pWorld->numOfObjects; i++)
		{
			t3DObject *pObject = &(pWorld->pObject[i]);

			pList1[i].pFaceList.resize(pObject->numOfFaces);
			pList2[i].pFaceList.resize(pObject->numOfFaces);
			pList3[i].pFaceList.resize(pObject->numOfFaces);
			pList4[i].pFaceList.resize(pObject->numOfFaces);
			pList5[i].pFaceList.resize(pObject->numOfFaces);
			pList6[i].pFaceList.resize(pObject->numOfFaces);
			pList7[i].pFaceList.resize(pObject->numOfFaces);
			pList8[i].pFaceList.resize(pObject->numOfFaces);

			for(int j = 0; j < pObject->numOfFaces; j++)
			{
				for(int whichVertex = 0; whichVertex < 3; whichVertex++)
				{
					CVector vPoint;
					vPoint.x = pObject->pVerts[pObject->pFaces[j].vertIndex[whichVertex]].x;
					vPoint.y = pObject->pVerts[pObject->pFaces[j].vertIndex[whichVertex]].y;
					vPoint.z = pObject->pVerts[pObject->pFaces[j].vertIndex[whichVertex]].z;

					if( (vPoint.x <= vCtr.x) && (vPoint.y >= vCtr.y) && (vPoint.z >= vCtr.z) ) 
						pList1[i].pFaceList[j] = true;

					if( (vPoint.x <= vCtr.x) && (vPoint.y >= vCtr.y) && (vPoint.z <= vCtr.z) ) 
						pList2[i].pFaceList[j] = true;

					if( (vPoint.x >= vCtr.x) && (vPoint.y >= vCtr.y) && (vPoint.z <= vCtr.z) ) 
						pList3[i].pFaceList[j] = true;

					if( (vPoint.x >= vCtr.x) && (vPoint.y >= vCtr.y) && (vPoint.z >= vCtr.z) ) 
						pList4[i].pFaceList[j] = true;

					if( (vPoint.x <= vCtr.x) && (vPoint.y <= vCtr.y) && (vPoint.z >= vCtr.z) ) 
						pList5[i].pFaceList[j] = true;

					if( (vPoint.x <= vCtr.x) && (vPoint.y <= vCtr.y) && (vPoint.z <= vCtr.z) ) 
						pList6[i].pFaceList[j] = true;

					if( (vPoint.x >= vCtr.x) && (vPoint.y <= vCtr.y) && (vPoint.z <= vCtr.z) ) 
						pList7[i].pFaceList[j] = true;

					if( (vPoint.x >= vCtr.x) && (vPoint.y <= vCtr.y) && (vPoint.z >= vCtr.z) ) 
						pList8[i].pFaceList[j] = true;
				}
			}	

			pList1[i].totalFaceCount = 0;		pList2[i].totalFaceCount = 0;
			pList3[i].totalFaceCount = 0;		pList4[i].totalFaceCount = 0;
			pList5[i].totalFaceCount = 0;		pList6[i].totalFaceCount = 0;
			pList7[i].totalFaceCount = 0;		pList8[i].totalFaceCount = 0;
		}

		int triCount1 = 0;	int triCount2 = 0;	int triCount3 = 0;	int triCount4 = 0;
		int triCount5 = 0;	int triCount6 = 0;	int triCount7 = 0;	int triCount8 = 0;
			
		for(int i = 0; i < pWorld->numOfObjects; i++)
		{
			for(int j = 0; j < pWorld->pObject[i].numOfFaces; j++)
			{
				if(pList1[i].pFaceList[j])	{ pList1[i].totalFaceCount++; triCount1++; }
				if(pList2[i].pFaceList[j])	{ pList2[i].totalFaceCount++; triCount2++; }
				if(pList3[i].pFaceList[j])	{ pList3[i].totalFaceCount++; triCount3++; }
				if(pList4[i].pFaceList[j])	{ pList4[i].totalFaceCount++; triCount4++; }
				if(pList5[i].pFaceList[j])	{ pList5[i].totalFaceCount++; triCount5++; }
				if(pList6[i].pFaceList[j])	{ pList6[i].totalFaceCount++; triCount6++; }
				if(pList7[i].pFaceList[j])	{ pList7[i].totalFaceCount++; triCount7++; }
				if(pList8[i].pFaceList[j])	{ pList8[i].totalFaceCount++; triCount8++; }
			}
		}

		CreateNewNode(pWorld, pList1, triCount1, vCenter, width, TOP_LEFT_FRONT);
		CreateNewNode(pWorld, pList2, triCount2, vCenter, width, TOP_LEFT_BACK);
		CreateNewNode(pWorld, pList3, triCount3, vCenter, width, TOP_RIGHT_BACK);
		CreateNewNode(pWorld, pList4, triCount4, vCenter, width, TOP_RIGHT_FRONT);
		CreateNewNode(pWorld, pList5, triCount5, vCenter, width, BOTTOM_LEFT_FRONT);
		CreateNewNode(pWorld, pList6, triCount6, vCenter, width, BOTTOM_LEFT_BACK);
		CreateNewNode(pWorld, pList7, triCount7, vCenter, width, BOTTOM_RIGHT_BACK);
		CreateNewNode(pWorld, pList8, triCount8, vCenter, width, BOTTOM_RIGHT_FRONT);
	}
	else
	{
		AssignTrianglesToNode(pWorld, numberOfTriangles);
	}

}

void COctree::AddObjectIndexToList(int index)
{
	for(unsigned int i = 0; i < m_pObjectList.size(); i++)
	{
		if(m_pObjectList[i] == index)
			return;
	}

	m_pObjectList.push_back(index);
}

void COctree::AssignTrianglesToNode(t3DModel *pWorld, int numberOfTriangles)
{
	m_bSubDivided = false;

	m_TriangleCount = numberOfTriangles;

	m_pWorld = new t3DModel;
	memset(m_pWorld, 0, sizeof(t3DModel));

	m_pWorld->numOfObjects = pWorld->numOfObjects;

	for(int i = 0; i < m_pWorld->numOfObjects; i++)
	{
		t3DObject *pObject = &(pWorld->pObject[i]);

		t3DObject newObject;
		memset(&newObject, 0, sizeof(t3DObject));

		if(pObject->numOfFaces)
			AddObjectIndexToList(i);

		m_pWorld->pObject.push_back(newObject);

		int numOfFaces = pObject->numOfFaces;

		m_pWorld->pObject[i].numOfFaces = numOfFaces;

		m_pWorld->pObject[i].pFaces = new tFace [numOfFaces];
		m_pWorld->pObject[i].pIndices = new UINT [numOfFaces * 3];

		memset(m_pWorld->pObject[i].pIndices, 0, sizeof(UINT) * numOfFaces * 3);

		memcpy(m_pWorld->pObject[i].pFaces, pObject->pFaces, sizeof(tFace) * numOfFaces);

		for(int j = 0; j < numOfFaces * 3; j += 3)
		{
			m_pWorld->pObject[i].pIndices[j]     = m_pWorld->pObject[i].pFaces[j / 3].vertIndex[0];
			m_pWorld->pObject[i].pIndices[j + 1] = m_pWorld->pObject[i].pFaces[j / 3].vertIndex[1];
			m_pWorld->pObject[i].pIndices[j + 2] = m_pWorld->pObject[i].pFaces[j / 3].vertIndex[2];
		}

		delete [] m_pWorld->pObject[i].pFaces;
		m_pWorld->pObject[i].pFaces = NULL;
	}

	m_DisplayListID = g_EndNodeCount;

	g_EndNodeCount++;
}

void COctree::DrawOctree(COctree *pNode, t3DModel *pRootWorld)
{

	if(!pNode) return;

	if(!g_Frustum.CubeInFrustum(pNode->m_vCenter.x, pNode->m_vCenter.y, 
								pNode->m_vCenter.z, pNode->m_Width / 2) )
	{
		return;
	}
	
	if(pNode->IsSubDivided())
	{
		DrawOctree(pNode->m_pOctreeNodes[TOP_LEFT_FRONT],		pRootWorld);
		DrawOctree(pNode->m_pOctreeNodes[TOP_LEFT_BACK],		pRootWorld);
		DrawOctree(pNode->m_pOctreeNodes[TOP_RIGHT_BACK],		pRootWorld);
		DrawOctree(pNode->m_pOctreeNodes[TOP_RIGHT_FRONT],		pRootWorld);
		DrawOctree(pNode->m_pOctreeNodes[BOTTOM_LEFT_FRONT],	pRootWorld);
		DrawOctree(pNode->m_pOctreeNodes[BOTTOM_LEFT_BACK],		pRootWorld);
		DrawOctree(pNode->m_pOctreeNodes[BOTTOM_RIGHT_BACK],	pRootWorld);
		DrawOctree(pNode->m_pOctreeNodes[BOTTOM_RIGHT_FRONT],	pRootWorld);
	}
	else
	{
		g_TotalNodesDrawn++;

		if(!pNode->m_pWorld) return;
			
		glCallList(pNode->m_DisplayListID);

	}
}

void COctree::CreateDisplayList(COctree *pNode, t3DModel *pRootWorld, int displayListOffset, CTga *Textura)
{
	if(!pNode) return;

	if(pNode->IsSubDivided())
	{
		CreateDisplayList(pNode->m_pOctreeNodes[TOP_LEFT_FRONT],	pRootWorld, displayListOffset, Textura);
		CreateDisplayList(pNode->m_pOctreeNodes[TOP_LEFT_BACK],		pRootWorld, displayListOffset, Textura);
		CreateDisplayList(pNode->m_pOctreeNodes[TOP_RIGHT_BACK],	pRootWorld, displayListOffset, Textura);
		CreateDisplayList(pNode->m_pOctreeNodes[TOP_RIGHT_FRONT],	pRootWorld, displayListOffset, Textura);
		CreateDisplayList(pNode->m_pOctreeNodes[BOTTOM_LEFT_FRONT],	pRootWorld, displayListOffset, Textura);
		CreateDisplayList(pNode->m_pOctreeNodes[BOTTOM_LEFT_BACK],	pRootWorld, displayListOffset, Textura);
		CreateDisplayList(pNode->m_pOctreeNodes[BOTTOM_RIGHT_BACK],	pRootWorld, displayListOffset, Textura);
		CreateDisplayList(pNode->m_pOctreeNodes[BOTTOM_RIGHT_FRONT],pRootWorld, displayListOffset, Textura);
	}
	else 
	{
		if(!pNode->m_pWorld) return;

		pNode->m_DisplayListID += displayListOffset;

		glNewList(pNode->m_DisplayListID,GL_COMPILE);

		int counter = 0;
		
		int objectCount = pNode->m_pObjectList.size();
		int materialCount = pRootWorld->pMaterials.size();

		while(counter < objectCount)
		{
			int i = pNode->m_pObjectList[counter];

			t3DObject *pObject     = &pNode->m_pWorld->pObject[i];
			t3DObject *pRootObject = &pRootWorld->pObject[i];

			if(pRootObject->bHasTexture) 
			{
				glEnable(GL_TEXTURE_2D);

				glColor3ub(255, 255, 255);
				glBindTexture(GL_TEXTURE_2D, Textura[pRootObject->materialID].texID);
			} 
			else 
			{
				glDisable(GL_TEXTURE_2D);

				glColor3ub(255, 255, 255);
			}

			if(materialCount && pRootObject->materialID >= 0) 
			{
				BYTE *pColor = pRootWorld->pMaterials[pRootObject->materialID].color;

				glColor3ub(pColor[0], pColor[1], pColor[2]);
			}

			if(pRootObject->pTexVerts) 
			{
				glEnableClientState(GL_TEXTURE_COORD_ARRAY);
				glTexCoordPointer(2, GL_FLOAT, 0, pRootObject->pTexVerts);
			}

			if(pRootObject->pVerts)
			{
				glEnableClientState(GL_VERTEX_ARRAY);
				glVertexPointer(3, GL_FLOAT, 0, pRootObject->pVerts);
			}

			if(pRootObject->pNormals)
			{
				glEnableClientState(GL_NORMAL_ARRAY);
				glNormalPointer(GL_FLOAT, 0, pRootObject->pNormals);
			}

			glDrawElements(GL_TRIANGLES,    pObject->numOfFaces * 3, 
						   GL_UNSIGNED_INT, pObject->pIndices);

			counter++;
		}

		glEndList();
	}
}
