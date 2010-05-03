#include "Main.h"
#include "3ds.h"

// Probando esta mamada XD
//lol hcker
// Agregando comentario de prueba
//Y viendo si este funciona... FAHL


//Libreria que usamos para el timer
#pragma comment( lib, "winmm.lib" )	

HDC			hDC=NULL;		// Dispositivo de contexto GDI
HGLRC		hRC=NULL;		// Contexto de renderizado
HWND		hWnd=NULL;		// Manejador de ventana
HINSTANCE	hInstance;		// Instancia de la aplicacion

bool	keys[256];			// Arreglo para el manejo de teclado
bool	active=TRUE;		// Bandera de ventana activa

int glWidth;
int glHeight;

GLUquadricObj	*e;

//Variables para fmod (cambiar el tamaño de los arreglos según el número de archivos de audio a usar)
FMOD_SOUND       *sound[2] = { 0, 0 };
FMOD_CHANNEL     *channel[2] = { 0, 0 };

parametros player1;  //Variable con la que tenemos acceso a la estructura de parámetros
CMateriales Material;

//Nombre y ubicación del modelo
#define FILE_NAME1  "Modelos/bob.3ds"

//Contenedor de texturas del modelo
CTga textureModel1[20];

CLoad3DS g_Load3ds;

//Acceso a la estructura que almacena los datos del modelo
t3DModel g_3DModel1;

//Contenedor de texturas para el escenario
CTga textura[30];

jerarquiaModelo player1modelo;	//Acceso a la estructura con las variables de cada pieza del modelo
const int maxKF1=3;				//Num. total de KeyFrames para la secuencia 1 (caminar)
FRAME KeyFrame1[maxKF1];		//Contenedor para almacenar cada keyframe de la secuencia 1

bool play=false;//Bandera para iniciar la animación
int playIndex=0;//Auxiliar para leer la información del contenedor de keyframes
int tipoAnim=1; //Indicador del tipo de animación

CMultitexturas Multitext;

GLuint modelo1;

//Constantes de iluminación y materiales
GLfloat LightPos[] = { 0.0f, 20.0f, 25.0f, 1.0f};		// Posición de la luz
GLfloat LightAmb[] = { 0.8f,  0.8f, 0.8f, 1.0f};			// Valores de la componente ambiente
GLfloat LightDif[] = { 0.9f,  0.9f, 0.9f, 1.0f};			// Valores de la componente difusa
GLfloat LightSpc[] = { 0.5f,  0.5f, 0.5f, 1.0f};			// Valores de la componente especular

CVector lightPosition;

//Variables auxiliares para animación de texturas
float aText1;
float aText2;

//Acceso a la clase CFont
CFont Font;

//Variables para el control de fps
GLfloat Time1;
GLfloat Time2;
GLfloat DiffTime;
static int FPS = 0;

bool  framerate_limit = true;

int	  adjust=2;											// Speed Adjustment For Really Slow Video Cards

struct			 										// Create A Structure For The Timer Information
{
  __int64       frequency;								// Timer Frequency
  float         resolution;								// Timer Resolution
  unsigned long mm_timer_start;							// Multimedia Timer Start Value
  unsigned long mm_timer_elapsed;						// Multimedia Timer Elapsed Time
  bool			performance_timer;						// Using The Performance Timer?
  __int64       performance_timer_start;				// Performance Timer Start Value
  __int64       performance_timer_elapsed;				// Performance Timer Elapsed Time
} timer;												// Structure Is Named timer

int		steps[6]={ 1, 2, 4, 5, 10, 20 };				// Stepping Values For Slow Video Adjustment

LRESULT	CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);	// Declaracion de WndProc (Procedimiento de ventana)

void TimerInit(void)
{
	memset(&timer, 0, sizeof(timer));

	if (!QueryPerformanceFrequency((LARGE_INTEGER *) &timer.frequency))
	{
		timer.performance_timer	= FALSE;
		timer.mm_timer_start	= timeGetTime();
		timer.resolution		= 1.0f/1000.0f;
		timer.frequency			= 1000;
		timer.mm_timer_elapsed	= timer.mm_timer_start;
	}
	else
	{
		QueryPerformanceCounter((LARGE_INTEGER *) &timer.performance_timer_start);
		timer.performance_timer			= TRUE;
		timer.resolution				= (float) (((double)1.0f)/((double)timer.frequency));
		timer.performance_timer_elapsed	= timer.performance_timer_start;
	}
}

float TimerGetTime()
{
	__int64 time;

	if (timer.performance_timer)
	{
		QueryPerformanceCounter((LARGE_INTEGER *) &time);
		return ( (float) ( time - timer.performance_timer_start) * timer.resolution)*1000.0f;
	}
	else
	{
		return( (float) ( timeGetTime() - timer.mm_timer_start) * timer.resolution)*1000.0f;
	}
}

void CalculateFrameRate()
{
	static float framesPerSecond    = 0.0f;
    static float lastTime			= 0.0f;
	static char strFrameRate[50] = {0};

	float currentTime = GetTickCount() * 0.001f;				

	++framesPerSecond;

	if( currentTime - lastTime > 1.0f )
    {
	    lastTime = currentTime;
		
		FPS=int(framesPerSecond);
		
	    framesPerSecond = 0;
    }
}

GLvoid ReSizeGLScene(GLsizei width, GLsizei height)		// Redimensiona e inicializa la ventana
{
	if (height==0)							// Para que no se presente una division por cero
	{
		height=1;							// la altura se iguala a 1
	}

	glViewport(0,0,width,height);					// Resetea el puerto de vista

	glMatrixMode(GL_PROJECTION);					// Selecciona la Matriz de Proyeccion
	glLoadIdentity();								// Resetea la Matriz de Proyeccion

	// Calcula el radio de aspecto o proporcion de medidas de la ventana
	gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,2000.0f);
	
	glMatrixMode(GL_MODELVIEW);							// Selecciona la Matriz de Vista de Modelo
	glLoadIdentity();									// Resetea la Matriz de Vista de Modelo

	glWidth=width;
	glHeight=height;
}

void CargaTexturas()
{
	textura[0].LoadTGA("Texturas/t1.tga");
}

void DescargaTexturas()
{
	textura[0].Elimina();
}

int CargaModelos()
{
	
	if(!g_Load3ds.Load3DSFile(FILE_NAME1, &g_3DModel1, textureModel1))
		return 0;
			
	return TRUE;
}

void DescargaModelos()
{
	g_Load3ds.UnLoad3DSFile(&g_3DModel1, textureModel1);
	
}

void CreaListas()
{
	modelo1=glGenLists(1);

	glNewList(modelo1,GL_COMPILE);
		g_Load3ds.Render3DSFile(&g_3DModel1, textureModel1, 1);
	glEndList();
	
}

void DestruyeListas()
{
	glDeleteLists(modelo1,1);
}

void InicializaParametrosdeControl()
{
	//Esta función establece los parámetros como velocidad del objeto y distancia de la cámara así como la posición y dirección iniciales
	player1.visible=true;
	player1.VelocidadObj=0.2f;
	player1.DistanciaCam=10.0f;

	player1.CamaraPosAlt=5.0f;	//Posición en y de la cámara (altura a la que se situa la cámara)
	player1.CamaraObjAlt=4.0f;	//Posición en y del objetivo de la cámara (altura a la que ve la cámara)
	player1.AngDir=90.0f;		//Este ángulo inicial hace que la dirección inicial sea paralela al eje Z y con sentido negativo
	player1.AngObj=0.0f;		//Este valor se elige dependiendo de la orientación con la que aparece el modelo en la escena al dibujarlo
								//sin aplicarle ninguna transformación (hacia adonde está volteando). Se elige un ángulo tal que al aplicarle
								//una rotación inicial con respecto al eje Y esté viendo hacia la misma dirección que la definida por AngDir
	
	player1.PosicionObj=CVector(0.0f, 0.0f, 0.0f); //Esta es la posición inicial del objeto en la escena
	player1.Direccion.x=(float)cos(player1.AngDir*PI/180.0f); //Dirección inicial definida por el ángulo inicial AngDir (x=cos(AngDir), y=0.0, z=sen(AngDir))
	player1.Direccion.y=0.0f;
	player1.Direccion.z=(float)sin(player1.AngDir*PI/180.0f);   
	player1.PosicionCam=CVector(0.0f, player1.CamaraPosAlt, 10.0f); //Posición inicial de la cámara a [DistanciaCam] unidades detrás del objeto
	player1.ObjetivoCam=player1.PosicionObj;		//La cámara ve siempre al objeto
	player1.ObjetivoCam.y=player1.CamaraObjAlt;		//Para que no vea a los "pies" del objeto (personaje)

	player1.Dir=0;
	player1.DirAnt=0;

	player1.escalaX=0.4f;
	player1.escalaY=0.4f;
	player1.escalaZ=0.4f;

	player1.CamaraObjAltE=0.0f;

}

void InicializaAnim()
{
	//Se inicializan las variables de la secuencia 1
	for(int i=0; i<maxKF1; i++)
	{
		KeyFrame1[i].Angt1=0.0f;
		KeyFrame1[i].Angt2=0.0f;
		KeyFrame1[i].Angc1=0.0f;
		KeyFrame1[i].Angc2=0.0f;
		KeyFrame1[i].Angbi1=0.0f;
		KeyFrame1[i].Angbi2=0.0f;
		KeyFrame1[i].Angbib=0.0f;
		KeyFrame1[i].Angbd1=0.0f;
		KeyFrame1[i].Angbd2=0.0f;
		KeyFrame1[i].Angbdb=0.0f;
		KeyFrame1[i].Angpizq=0.0f;
		KeyFrame1[i].Angpizqb=0.0f;
		KeyFrame1[i].Angpder=0.0f;
		KeyFrame1[i].Angpderb=0.0f;
		KeyFrame1[i].Angpi=0.0f;
		KeyFrame1[i].Angpd=0.0f;
		KeyFrame1[i].Xtor=0.0f;
		KeyFrame1[i].Ytor=0.0f;
		KeyFrame1[i].Ztor=0.0f;

		KeyFrame1[i].incAngt1=false;
		KeyFrame1[i].incAngt1=false;
		KeyFrame1[i].incAngc1=false;
		KeyFrame1[i].incAngc2=false;
		KeyFrame1[i].incAngbi1=false;
		KeyFrame1[i].incAngbi2=false;
		KeyFrame1[i].incAngbib=false;
		KeyFrame1[i].incAngbd1=false;
		KeyFrame1[i].incAngbd2=false;
		KeyFrame1[i].incAngbdb=false;
		KeyFrame1[i].incAngpizq=false;
		KeyFrame1[i].incAngpizqb=false;
		KeyFrame1[i].incAngpder=false;
		KeyFrame1[i].incAngpderb=false;
		KeyFrame1[i].incAngpi=false;
		KeyFrame1[i].incAngpd=false;
		KeyFrame1[i].incXtor=false;
		KeyFrame1[i].incYtor=false;
		KeyFrame1[i].incZtor=false;
	}

	player1modelo.Angt1=0.0f;
	player1modelo.Angt2=0.0f;
	player1modelo.Angc1=0.0f;
	player1modelo.Angc2=0.0f;
	player1modelo.Angbi1=0.0f;
	player1modelo.Angbi2=0.0f;
	player1modelo.Angbib=0.0f;
	player1modelo.Angbd1=0.0f;
	player1modelo.Angbd2=0.0f;
	player1modelo.Angbdb=0.0f;
	player1modelo.Angpizq=0.0f;
	player1modelo.Angpizqb=0.0f;
	player1modelo.Angpder=0.0f;
	player1modelo.Angpderb=0.0f;
	player1modelo.Angpi=0.0f;
	player1modelo.Angpd=0.0f;
	player1modelo.Xtor=0.0f;
	player1modelo.Ytor=0.0f;
	player1modelo.Ztor=0.0f;
	
}

void DatosAnimacion()
{
	//Secuencia 1
	//KeyFrame1[0].Angt1=0.0f;	
	//..
}

int InitGL(GLvoid)										// Aqui se configuran los parametros iniciales de OpenGL
{
	Multitext.InitMultitext(hWnd);

	glShadeModel(GL_SMOOTH);							// Activa Smooth Shading
	glClearColor(0.4f, 0.6f, 0.8f, 0.5f);				// Fondo azul
	glClearDepth(1.0f);									// Valor para el Depth Buffer
	glClearStencil(0);
	glEnable(GL_DEPTH_TEST);							// Activa Depth Testing
	glDepthFunc(GL_LEQUAL);								// Tipo de Depth Testing a usar
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Correccion de cálculos de perspectiva

	glCullFace(GL_BACK);								// Set Culling Face To Back Face
	glEnable(GL_CULL_FACE);								// Enable Culling

	//Iluminación
	//Inicializa la luz
	glLightfv(GL_LIGHT0, GL_POSITION, LightPos);		// Posicion de la luz1
	glLightfv(GL_LIGHT0, GL_AMBIENT,  LightAmb);		// Componente ambiente
	glLightfv(GL_LIGHT0, GL_DIFFUSE,  LightDif);		// Componente difusa
	glLightfv(GL_LIGHT0, GL_SPECULAR, LightSpc);		// Componente especular

	/*
	glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 0.05f);
    glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.05f);
    glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.0f);
	*/
	
	glEnable(GL_LIGHT0);								// Activa luz0
	glEnable(GL_LIGHTING);								// Habilita la iluminación

	//CargaTexturas();

	CargaModelos();
	CreaListas();

	Font.BuildFont();

	InitGLSL();
	
	e=gluNewQuadric();

	InicializaParametrosdeControl();
	InicializaAnim();
	DatosAnimacion();

	return TRUE;										
}

void ERRCHECK(FMOD_RESULT result)
{
    if (result != FMOD_OK)
    {
        printf("FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));
        exit(-1);
    }
}

void LiberaSonido(FMOD_SYSTEM *system, FMOD_RESULT result)
{
	result = FMOD_Sound_Release(sound[0]);
    ERRCHECK(result);
    result = FMOD_Sound_Release(sound[1]);
    ERRCHECK(result);
    result = FMOD_System_Close(system);
    ERRCHECK(result);
    result = FMOD_System_Release(system);
    ERRCHECK(result);
}

void ControlPersonaje(int funcion)
{
	if(funcion == 1) //Giro a la derecha
	{
		player1.AngDir+=1.0f;
		if(player1.AngDir > 360.0f)
			player1.AngDir-=360.0f;

		player1.AngObj-=1.0f;
		if(player1.AngObj < 0.0f)
			player1.AngObj+=360.0f;

		player1.Direccion.x=(float)cos(player1.AngDir*PI/180.0f);
		player1.Direccion.y=0.0f;
		player1.Direccion.z=(float)sin(player1.AngDir*PI/180.0f);

		player1.PosicionCam=player1.PosicionObj-player1.Direccion*player1.DistanciaCam;
		player1.PosicionCam.y=player1.CamaraPosAlt;
		player1.ObjetivoCam=player1.PosicionObj;
		player1.ObjetivoCam.y=player1.CamaraObjAlt;
	}
	else if(funcion == 2) //Giro a la izquierda
	{
		player1.AngDir-=1.0f;
		if(player1.AngDir < 0.0f)
			player1.AngDir+=360.0f;

		player1.AngObj+=1.0f;
		if(player1.AngObj > 360.0f)
			player1.AngObj-=360.0f;

		player1.Direccion.x=(float)cos(player1.AngDir*PI/180.0f);
		player1.Direccion.y=0.0f;
		player1.Direccion.z=(float)sin(player1.AngDir*PI/180.0f);

		player1.PosicionCam=player1.PosicionObj-player1.Direccion*player1.DistanciaCam;
		player1.PosicionCam.y=player1.CamaraPosAlt;
		player1.ObjetivoCam=player1.PosicionObj;
		player1.ObjetivoCam.y=player1.CamaraObjAlt;
	}
	else if(funcion == 3) //Avanza hacia adelante
	{
		player1.PosicionObj=player1.PosicionObj+player1.Direccion*player1.VelocidadObj;
		player1.PosicionCam=player1.PosicionObj-player1.Direccion*player1.DistanciaCam;
		player1.PosicionCam.y=player1.CamaraPosAlt;
		player1.ObjetivoCam=player1.PosicionObj;
		player1.ObjetivoCam.y=player1.CamaraObjAlt;

		player1.PosAntObj=player1.PosicionObj;

	}
	else if(funcion == 4) //Avanza hacia atrás
	{
		player1.PosicionObj=player1.PosicionObj-player1.Direccion*player1.VelocidadObj;
		player1.PosicionCam=player1.PosicionObj-player1.Direccion*player1.DistanciaCam;
		player1.PosicionCam.y=player1.CamaraPosAlt;
		player1.ObjetivoCam=player1.PosicionObj;
		player1.ObjetivoCam.y=player1.CamaraObjAlt;

		player1.PosAntObj=player1.PosicionObj;

	}
	else if(funcion == 5) //Sube objetivo de la cámara
	{
		player1.ObjetivoCam.y+=1.0f;
		player1.CamaraObjAlt+=1.0f;
		player1.CamaraObjAltE+=1.0f;
		
	}
	else if(funcion == 6) //Baja objetivo de la cámara
	{
		player1.ObjetivoCam.y-=1.0f;
		player1.CamaraObjAlt-=1.0f;
		player1.CamaraObjAltE-=1.0f;
		
	}
}

void animacion(FRAME *KeyFrame, int maxKF , int frames)
{
	if(play)
	{		
		if((abs(KeyFrame[playIndex+1].Angt1-player1modelo.Angt1))<0.1 &&
		   (abs(KeyFrame[playIndex+1].Angt2-player1modelo.Angt2))<0.1 &&
		   (abs(KeyFrame[playIndex+1].Angc1-player1modelo.Angc1))<0.1 &&
		   (abs(KeyFrame[playIndex+1].Angc2-player1modelo.Angc2))<0.1 &&
		   (abs(KeyFrame[playIndex+1].Angbi1-player1modelo.Angbi1))<0.1 &&
		   (abs(KeyFrame[playIndex+1].Angbi2-player1modelo.Angbi2))<0.1 &&
		   (abs(KeyFrame[playIndex+1].Angbib-player1modelo.Angbib))<0.1 &&
		   (abs(KeyFrame[playIndex+1].Angbd1-player1modelo.Angbd1))<0.1 &&
		   (abs(KeyFrame[playIndex+1].Angbd2-player1modelo.Angbd2))<0.1 &&
		   (abs(KeyFrame[playIndex+1].Angbdb-player1modelo.Angbdb))<0.1 &&
		   (abs(KeyFrame[playIndex+1].Angpizq-player1modelo.Angpizq))<0.1 &&
		   (abs(KeyFrame[playIndex+1].Angpizqb-player1modelo.Angpizqb))<0.1 &&
		   (abs(KeyFrame[playIndex+1].Angpder-player1modelo.Angpder))<0.1 &&
		   (abs(KeyFrame[playIndex+1].Angpderb-player1modelo.Angpderb))<0.1 &&
		   (abs(KeyFrame[playIndex+1].Angpi-player1modelo.Angpi))<0.1 &&
		   (abs(KeyFrame[playIndex+1].Angpd-player1modelo.Angpd))<0.1 &&
		   (abs(KeyFrame[playIndex+1].Xtor-player1modelo.Xtor))<0.1 &&
		   (abs(KeyFrame[playIndex+1].Ytor-player1modelo.Ytor))<0.1 &&
		   (abs(KeyFrame[playIndex+1].Ztor-player1modelo.Ztor))<0.1)
		{			
			playIndex++;			
			if(playIndex>maxKF-2)
			{
				playIndex=0;
				play=false;
								
			}
		}
		else
		{
			KeyFrame[playIndex].incAngt1    = (KeyFrame[playIndex+1].Angt1-KeyFrame[playIndex].Angt1)/frames;
			KeyFrame[playIndex].incAngt2    = (KeyFrame[playIndex+1].Angt2-KeyFrame[playIndex].Angt2)/frames;
			KeyFrame[playIndex].incAngc1    = (KeyFrame[playIndex+1].Angc1-KeyFrame[playIndex].Angc1)/frames;
			KeyFrame[playIndex].incAngc2    = (KeyFrame[playIndex+1].Angc2-KeyFrame[playIndex].Angc2)/frames;
			KeyFrame[playIndex].incAngbi1   = (KeyFrame[playIndex+1].Angbi1-KeyFrame[playIndex].Angbi1)/frames;
			KeyFrame[playIndex].incAngbi2   = (KeyFrame[playIndex+1].Angbi2-KeyFrame[playIndex].Angbi2)/frames;
			KeyFrame[playIndex].incAngbib   = (KeyFrame[playIndex+1].Angbib-KeyFrame[playIndex].Angbib)/frames;
			KeyFrame[playIndex].incAngbd1   = (KeyFrame[playIndex+1].Angbd1-KeyFrame[playIndex].Angbd1)/frames;
			KeyFrame[playIndex].incAngbd2   = (KeyFrame[playIndex+1].Angbd2-KeyFrame[playIndex].Angbd2)/frames;
			KeyFrame[playIndex].incAngbdb   = (KeyFrame[playIndex+1].Angbdb-KeyFrame[playIndex].Angbdb)/frames;
			KeyFrame[playIndex].incAngpizq  = (KeyFrame[playIndex+1].Angpizq-KeyFrame[playIndex].Angpizq)/frames;
			KeyFrame[playIndex].incAngpizqb = (KeyFrame[playIndex+1].Angpizqb-KeyFrame[playIndex].Angpizqb)/frames;
			KeyFrame[playIndex].incAngpder  = (KeyFrame[playIndex+1].Angpder-KeyFrame[playIndex].Angpder)/frames;
			KeyFrame[playIndex].incAngpderb = (KeyFrame[playIndex+1].Angpderb-KeyFrame[playIndex].Angpderb)/frames;
			KeyFrame[playIndex].incAngpi    = (KeyFrame[playIndex+1].Angpi-KeyFrame[playIndex].Angpi)/frames;
			KeyFrame[playIndex].incAngpd    = (KeyFrame[playIndex+1].Angpd-KeyFrame[playIndex].Angpd)/frames;
			KeyFrame[playIndex].incXtor     = (KeyFrame[playIndex+1].Xtor-KeyFrame[playIndex].Xtor)/frames;
			KeyFrame[playIndex].incYtor     = (KeyFrame[playIndex+1].Ytor-KeyFrame[playIndex].Ytor)/frames;
			KeyFrame[playIndex].incZtor     = (KeyFrame[playIndex+1].Ztor-KeyFrame[playIndex].Ztor)/frames;
			
			player1modelo.Angt1    += KeyFrame[playIndex].incAngt1;
			player1modelo.Angt2    += KeyFrame[playIndex].incAngt2;
			player1modelo.Angc1    += KeyFrame[playIndex].incAngc1;
			player1modelo.Angc2    += KeyFrame[playIndex].incAngc2;
			player1modelo.Angbi1   += KeyFrame[playIndex].incAngbi1;
			player1modelo.Angbi2   += KeyFrame[playIndex].incAngbi2;
			player1modelo.Angbib   += KeyFrame[playIndex].incAngbib;
			player1modelo.Angbd1   += KeyFrame[playIndex].incAngbd1;
			player1modelo.Angbd2   += KeyFrame[playIndex].incAngbd2;
			player1modelo.Angbdb   += KeyFrame[playIndex].incAngbdb;
			player1modelo.Angpizq  += KeyFrame[playIndex].incAngpizq;
			player1modelo.Angpizqb += KeyFrame[playIndex].incAngpizqb;
			player1modelo.Angpder  += KeyFrame[playIndex].incAngpder;
			player1modelo.Angpderb += KeyFrame[playIndex].incAngpderb;
			player1modelo.Angpi    += KeyFrame[playIndex].incAngpi;
			player1modelo.Angpd    += KeyFrame[playIndex].incAngpd;
			player1modelo.Xtor     += KeyFrame[playIndex].incXtor;
			player1modelo.Ytor     += KeyFrame[playIndex].incYtor;
			player1modelo.Ztor     += KeyFrame[playIndex].incZtor;
			
		}
	}
}

void DibujaPersonaje()
{
	glTranslatef(player1modelo.Xtor, player1modelo.Ytor, player1modelo.Ztor);
	glRotatef(player1modelo.Angt2, 0.0f, 1.0f, 0.0f);
	glRotatef(player1modelo.Angt1, 1.0f, 0.0f, 0.0f);
			
	glCallList(modelo1);
}

void DibujaEjes()
{
	glColor3f(1.0f,0.0f,0.0f);

	glBegin(GL_LINES);
		//Eje X
		glVertex3f(-100.0f, 0.0f, 0.0f);
		glVertex3f( 100.0f, 0.0f, 0.0f);

		//Eje Y
		glVertex3f(0.0f, -100.0f, 0.0f);
		glVertex3f(0.0f,  100.0f, 0.0f);

		//Eje Z
		glVertex3f(0.0f, 0.0f, -100.0f);
		glVertex3f(0.0f, 0.0f,  100.0f);
	glEnd();

	glColor3f(1.0f,1.0f,1.0f);
}

void DibujaLuz(CVector l)
{
	//Dibuja una esfera que representa la fuente luminosa
	glDisable(GL_LIGHTING);									// Deshabilita iluminación
	glPushMatrix();
		glTranslatef(l.x, l.y, l.z);// Traslada a la posicion de la luz
		glColor4f(1.0f, 0.9f, 0.0f, 1.0f);					// Color amarillo
		gluSphere(e, 1.0f, 16, 8);							// Dibuja la esfera
	glPopMatrix();
	glEnable(GL_LIGHTING);									// Habilita Iluminación

	glColor3f(1.0f, 1.0f, 1.0f);
}

void ActualizaAnimText()
{
	if(aText1 < 1.0f)
		aText1+=0.01f;
	else
		aText1=0.0f;

	if(aText2 < 1.0f)
		aText2+=0.001f;
	else
		aText2=0.0f;
}

void DibujaTextos()
{
	glDisable(GL_DEPTH_TEST);							// Desactiva la prueba de profundidad
	glMatrixMode(GL_PROJECTION);						// Selecciona la matriz de proyección
	glPushMatrix();										// Guarda la matriz de proyección
	glLoadIdentity();									// Limpia la matriz de proyección
	glOrtho(0,glWidth,0,glHeight,-1,1);					// Crea una proyección ortogonal
	glMatrixMode(GL_MODELVIEW);							// Selecciona la matriz de modelo de vista
	glPushMatrix();										// Guarda la matriz de modelo de vista
	
	glDisable(GL_LIGHTING);
	glLoadIdentity();

	glColor3f(1.0f,1.0f,1.0f);
	
	glEnable(GL_TEXTURE_2D);

	glAlphaFunc(GL_GREATER, 0.8f);
	glEnable(GL_ALPHA_TEST);

	Font.glPrint((2.0f/640.0f)*glWidth, glWidth*0.05f,glHeight*0.9f,"Ejemplo texto");
	Font.glPrint((1.2f/640.0f)*glWidth, glWidth*0.05f,glHeight*0.85f,"FPS %d",FPS);
								
	glDisable(GL_ALPHA_TEST);
	glDisable(GL_TEXTURE_2D);

	glColor3f(1.0f,1.0f,1.0f);

	glEnable(GL_LIGHTING);
	glMatrixMode(GL_PROJECTION);						// Selecciona la matriz de proyección
	glPopMatrix();										// Recupera la anterior matriz de proyección
	glMatrixMode(GL_MODELVIEW);							// Selecciona la matriz de modelo de vista
	glPopMatrix();										// Recupera la anterior matriz de modelo de vista
	glEnable(GL_DEPTH_TEST);							// Activa la prueba de profundidad
		
}

int RenderizaEscena(GLvoid)								// Aqui se dibuja todo lo que aparecera en la ventana
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glLoadIdentity();
	
	gluLookAt(0.0f, 0.0f, 20.0f, 0.0f, 0.0f, 0.0f, 0, 1, 0);
	/*
	gluLookAt(player1.PosicionCam.x, player1.PosicionCam.y, player1.PosicionCam.z, 
			  player1.ObjetivoCam.x, player1.ObjetivoCam.y, player1.ObjetivoCam.z, 
			  0.0f, 1.0f, 0.0f);
	*/

	//Se actualizan los parámetros de iluminación
	glLightfv(GL_LIGHT0, GL_POSITION, LightPos);		// Posicion de la luz1
	glLightfv(GL_LIGHT0, GL_AMBIENT,  LightAmb);		// Componente ambiente
	glLightfv(GL_LIGHT0, GL_DIFFUSE,  LightDif);		// Componente difusa
	glLightfv(GL_LIGHT0, GL_SPECULAR, LightSpc);		// Componente especular
	
	glEnable(GL_NORMALIZE);

	glPushMatrix();
		glTranslatef(player1.PosicionObj.x, player1.PosicionObj.y, player1.PosicionObj.z);
		glRotatef(player1.AngObj, 0.0f, 1.0f, 0.0f);
		glScalef(player1.escalaX,player1.escalaY,player1.escalaZ);
		DibujaPersonaje();
	glPopMatrix();

	glDisable(GL_NORMALIZE);
	
	lightPosition.x=LightPos[0];
	lightPosition.y=LightPos[1];
	lightPosition.z=LightPos[2];
	DibujaLuz(lightPosition);
	
	DibujaTextos();
	ActualizaAnimText();

	CalculateFrameRate();

	return TRUE;
}

GLvoid KillGLWindow(GLvoid)								// Elimina la ventana apropiadamente
{
	if (hRC)											// Si existe un contexto de renderizado...
	{
		if (!wglMakeCurrent(NULL,NULL))					// Si no se pueden liberar los contextos DC y RC...
		{
			MessageBox(NULL,"Falla al liberar DC y RC.","Error de finalización",MB_OK | MB_ICONINFORMATION);
		}

		if (!wglDeleteContext(hRC))						// Si no se puede eliminar el RC?
		{
			MessageBox(NULL,"Falla al liberar el contexto de renderizado.","Error de finalización",MB_OK | MB_ICONINFORMATION);
		}
		hRC=NULL;										// Se pone RC en NULL
	}

	if (hDC && !ReleaseDC(hWnd,hDC))					// Si no se puede eliminar el DC
	{
		MessageBox(NULL,"Falla al liberar el contexto de renderizado.","Error de finalización",MB_OK | MB_ICONINFORMATION);
		hDC=NULL;										// Se pone DC en NULL
	}

	if (hWnd && !DestroyWindow(hWnd))					// Si no se puede destruir la ventana
	{
		MessageBox(NULL,"No se pudo liberar hWnd.","Error de finalización",MB_OK | MB_ICONINFORMATION);
		hWnd=NULL;										// Se pone hWnd en NULL
	}

	if (!UnregisterClass("OpenGL",hInstance))			// Si no se puede eliminar el registro de la clase
	{
		MessageBox(NULL,"No se pudo eliminar el registro de la clase.","Error de finalización",MB_OK | MB_ICONINFORMATION);
		hInstance=NULL;									// Se pone hInstance en NULL
	}
}

//	Este código crea la ventana de OpenGL.  Parámetros:					
//	title			- Titulo en la parte superior de la ventana			
//	width			- Ancho de la ventana								
//	height			- Alto de la ventana								
//	bits			- Número de bits a usar para el color (8/16/24/32)	
  
BOOL CreateGLWindow(char* title, int width, int height, int bits)
{
	GLuint	PixelFormat;				// Guarda el resultado despues de determinar el formato a usar
	WNDCLASS	wc;						// Estructura de la clase ventana
	DWORD		dwExStyle;				// Estilo extendido de ventana
	DWORD		dwStyle;				// Estilo de ventana
	RECT		WindowRect;				// Guarda los valores Superior Izquierdo / Inferior Derecho del rectángulo
	WindowRect.left=(long)0;			// Inicia el valor Izquierdo a 0
	WindowRect.right=(long)width;		// Inicia el valor Derecho al ancho especificado
	WindowRect.top=(long)0;				// Inicia el valor Superior a 0
	WindowRect.bottom=(long)height;		// Inicia el valor Inferior al alto especificado

	hInstance			= GetModuleHandle(NULL);				// Guarda una instancia de la ventana
	wc.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;	// Redibuja el contenido de la ventana al redimensionarla
	wc.lpfnWndProc		= (WNDPROC) WndProc;					// Maneja los mensajes para WndProc
	wc.cbClsExtra		= 0;									// Ningun dato extra para la clase
	wc.cbWndExtra		= 0;									// Ningun dato extra para la ventana
	wc.hInstance		= hInstance;							// Inicia la instancia
	wc.hIcon			= LoadIcon(NULL, IDI_WINLOGO);			// Carga el ícono por defecto
	wc.hCursor			= LoadCursor(NULL, IDC_ARROW);			// Carga el puntero de flecha
	wc.hbrBackground	= NULL;									// No se requiere ningun fondo
	wc.lpszMenuName		= NULL;									// No hay menú en la ventana
	wc.lpszClassName	= "OpenGL";								// Fija el nombre de la clase.

	if (!RegisterClass(&wc))									// Intenta registrar la clase de ventana
	{
		MessageBox(NULL,"Failed To Register The Window Class.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;											
	}
		
	dwExStyle=WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;					// Estilo extendido de ventana
	dwStyle=WS_OVERLAPPEDWINDOW;									// Estilo de ventana

	AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);		// Ajusta la ventana al tamaño especificado

	// Crea la ventana
	if (!(hWnd=CreateWindowEx(	dwExStyle,							// Estilo extendido para la ventana
								"OpenGL",							// Nombre de la clase
								title,								// Título de la ventana
								dwStyle |							// Definición del estilo de la ventana
								WS_CLIPSIBLINGS |					// Estilo requerido de la ventana
								WS_CLIPCHILDREN,					// Estilo requerido de la ventana
								0, 0,								// Posición de la ventana
								WindowRect.right-WindowRect.left,	// Calcula el ancho de la ventana
								WindowRect.bottom-WindowRect.top,	// Calcula el alto de la ventana
								NULL,								// No hay ventana superior
								NULL,								// No hay menú
								hInstance,							// Instancia
								NULL)))								// No se pasa nada a WM_CREATE
	{
		KillGLWindow();												// Resetea el despliegue
		MessageBox(NULL,"Error al crear la ventana.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								
	}

	static	PIXELFORMATDESCRIPTOR pfd=				// pfd Tells Windows How We Want Things To Be
	{
		sizeof(PIXELFORMATDESCRIPTOR),				// Size Of This Pixel Format Descriptor
		1,											// Version Number
		PFD_DRAW_TO_WINDOW |						// Format Must Support Window
		PFD_SUPPORT_OPENGL |						// Format Must Support OpenGL
		PFD_DOUBLEBUFFER,							// Must Support Double Buffering
		PFD_TYPE_RGBA,								// Request An RGBA Format
		bits,										// Select Our Color Depth
		0, 0, 0, 0, 0, 0,							// Color Bits Ignored
		0,											// No Alpha Buffer
		0,											// Shift Bit Ignored
		0,											// No Accumulation Buffer
		0, 0, 0, 0,									// Accumulation Bits Ignored
		16,											// 16Bit Z-Buffer (Depth Buffer)  
		1,											// 16Bit Stencil Buffer
		0,											// No Auxiliary Buffer
		PFD_MAIN_PLANE,								// Main Drawing Layer
		0,											// Reserved
		0, 0, 0										// Layer Masks Ignored
	};
	
	if (!(hDC=GetDC(hWnd)))							// Si no se creo el contexto de dispositivo...
	{
		KillGLWindow();								// Resetea el despliegue
		MessageBox(NULL,"No se puede crear un contexto de dispositivo GL.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								
	}

	if (!(PixelFormat=ChoosePixelFormat(hDC,&pfd)))	// Si Windows no encontró un formato de pixel compatible
	{
		KillGLWindow();								// Resetea el despliegue
		MessageBox(NULL,"No se puede encontrar un formato de pixel compatible.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								
	}

	if(!SetPixelFormat(hDC,PixelFormat,&pfd))		// Si no se pudo habilitar el formato de pixel
	{
		KillGLWindow();								// Resetea el despliegue
		MessageBox(NULL,"No se puede usar el formato de pixel.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								
	}

	if (!(hRC=wglCreateContext(hDC)))				// Si no se creo el contexto de renderizado
	{
		KillGLWindow();								// Resetea el despliegue
		MessageBox(NULL,"No se puede crear un contexto de renderizado GL.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								
	}

	if(!wglMakeCurrent(hDC,hRC))					// Si no se puede activar el contexto de renderizado
	{
		KillGLWindow();								// Resetea el despliegue
		MessageBox(NULL,"No se puede usar el contexto de renderizado GL.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								
	}

	ShowWindow(hWnd,SW_SHOW);				// Muestra la ventana
	SetForegroundWindow(hWnd);				// Le da la prioridad mas alta
	SetFocus(hWnd);							// Pasa el foco del teclado a la ventana
	ReSizeGLScene(width, height);			// Inicia la perspectiva para la ventana OGL

	if (!InitGL())							// Si no se inicializa la ventana creada
	{
		KillGLWindow();						// Resetea el despliegue
		MessageBox(NULL,"Falla en la inicialización.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								
	}

	return TRUE;							// Todo correcto
}

LRESULT CALLBACK WndProc(	HWND	hWnd,	// Manejador para esta ventana
							UINT	uMsg,	// Mensaje para esta ventana
							WPARAM	wParam,	// Información adicional del mensaje
							LPARAM	lParam)	// Información adicional del mensaje
{
	switch (uMsg)							// Revisa los mensajes de la ventana
	{
		case WM_ACTIVATE:					// Revisa el mensaje de activación de ventana
		{
			if (!HIWORD(wParam))			// Revisa el estado de minimización
			{
				active=TRUE;				// El programa está activo
			}
			else
			{
				active=FALSE;				// El programa no está activo
			}

			return 0;						// Regresa al ciclo de mensajes
		}

		case WM_SYSCOMMAND:					// Intercepta comandos del sistema
		{
			switch (wParam)					// Revisa llamadas del sistema
			{
				case SC_SCREENSAVE:			// ¿Screensaver tratando de iniciar?
				case SC_MONITORPOWER:		// ¿Monitor tratando de entrar a modo de ahorro de energía?
				return 0;					// Evita que suceda
			}
			break;							// Sale del caso
		}

		case WM_CLOSE:						// Si se recibe un mensaje de cerrar...
		{
			PostQuitMessage(0);				// Se manda el mensaje de salida
			return 0;						// y se regresa al ciclo
		}

		case WM_KEYDOWN:					// Si se está presionando una tecla...
		{
			keys[wParam] = TRUE;			// Si es así, se marca como TRUE
			return 0;						// y se regresa al ciclo
		}

		case WM_KEYUP:						// ¿Se ha soltado una tecla?
		{
			keys[wParam] = FALSE;			// Si es así, se marca como FALSE
			return 0;						// y se regresa al ciclo
		}

		case WM_SIZE:						// Si se redimensiona la ventana...
		{
			ReSizeGLScene(LOWORD(lParam),HIWORD(lParam));  	// LoWord=Width, HiWord=Height
			return 0;						// y se regresa al ciclo
		}
	}

	// Pasa todos los mensajes no considerados a DefWindowProc
	return DefWindowProc(hWnd,uMsg,wParam,lParam);
}

// Este es el punto de entrada al programa; la función principal 
int WINAPI WinMain(	HINSTANCE	hInstance,			// Instancia
					HINSTANCE	hPrevInstance,		// Instancia previa
					LPSTR		lpCmdLine,			// Parametros de la linea de comandos
					int			nCmdShow)			// Muestra el estado de la ventana
{
	MSG		msg;									// Estructura de mensajes de la ventana
	BOOL	done=FALSE;								// Variable booleana para salir del ciclo

	//variables de fmod
	FMOD_SYSTEM      *system;
	FMOD_RESULT       result;
    unsigned int      version;

	unsigned int ms = 0;
    unsigned int lenms = 0;
    int          playing = 0;
    int          paused = 0;
    int          channelsplaying = 0;

	// Crea la ventana OpenGL
	if (!CreateGLWindow("Computación Gráfica",640,480,32))
	{
		return 0;									// Salir del programa si la ventana no fue creada
	}

	//Se inicia el sistema de sonido de FMOD
	result = FMOD_System_Create(&system);
    ERRCHECK(result);

    result = FMOD_System_GetVersion(system, &version);
    ERRCHECK(result);

    if (version < FMOD_VERSION)
    {
        printf("Error!  You are using an old version of FMOD %08x.  This program requires %08x\n", version, FMOD_VERSION);
        return 0;
    }

    result = FMOD_System_Init(system, 32, FMOD_INIT_NORMAL, NULL);
    ERRCHECK(result);

	//Carga de los archivos de audio
    result = FMOD_System_CreateSound(system, "Audio/iteza.mp3", FMOD_HARDWARE, 0, &sound[0]);
    ERRCHECK(result);
    result = FMOD_Sound_SetMode(sound[0], FMOD_LOOP_NORMAL); //Se repetira en un loop
    ERRCHECK(result);										

    result = FMOD_System_CreateSound(system, "Audio/explode.wav", FMOD_SOFTWARE, 0, &sound[1]);
    ERRCHECK(result);

	//Asignación a canales y configuración
	//Música de fondo
	result = FMOD_System_PlaySound(system, FMOD_CHANNEL_FREE, sound[0], 0, &channel[0]);
    ERRCHECK(result);
	result = FMOD_Channel_SetPaused(channel[0], FALSE); //Inicialmente activo
	ERRCHECK(result);
	result = FMOD_Channel_SetVolume(channel[0], 0.3f);
	ERRCHECK(result);

	result = FMOD_System_PlaySound(system, FMOD_CHANNEL_FREE, sound[1], 0, &channel[1]);
    ERRCHECK(result);
	result = FMOD_Channel_SetPaused(channel[1], TRUE); //Inicialmente pausado
	ERRCHECK(result);

	TimerInit();

	while(!done)									// Mientras done=FALSE
	{
		if (PeekMessage(&msg,NULL,0,0,PM_REMOVE))	// Revisa si hay mensajes en espera
		{
			if (msg.message==WM_QUIT)				// Si se ha recibido el mensje de salir...
			{
				done=TRUE;							// Entonces done=TRUE
			}
			else									// Si no, Procesa los mensajes de la ventana
			{
				TranslateMessage(&msg);				// Traduce el mensaje
				DispatchMessage(&msg);				// Envia el mensaje
			}
		}
		else										// Si no hay mensajes...
		{
			FMOD_System_Update(system);

			// Dibuja la escena. 
			if (active)								// Si está activo el programa...
			{
				float start=TimerGetTime();

				if (keys[VK_ESCAPE])				// Si se ha presionado ESC
				{
					done=TRUE;						// ESC indica el termino del programa
				}
				else								// De lo contrario, actualiza la pantalla
				{
					RenderizaEscena();				// Dibuja la escena
					if(play)
					{
						if(tipoAnim == 1)
							animacion(KeyFrame1, maxKF1 , 18);
					}
					SwapBuffers(hDC);				// Intercambia los Buffers (Double Buffering)
				}

				if(!ManejaTeclado()) return 0;
				ManejaTeclado(system, result);

				while(TimerGetTime()<start+float(steps[adjust]*2.0f)) {}	// Desperdicia ciclos si es demasiado rápido
			}
			
		}
	}

	// Finalización del programa
	//DescargaTexturas();
	DescargaModelos();
	DestruyeListas();
	Font.DestroyFont();
	LiberaSonido(system, result);
	KillGLWindow();									// Destruye la ventana
	return (msg.wParam);							// Sale del programa
}

int ManejaTeclado()
{
	if(GetKeyState(VK_UP) & 0x80)
	{
		player1.Dir=1;

		ControlPersonaje(3);
		
		if(play==false)
		{
			player1modelo.Angt1    = KeyFrame1[0].Angt1;
			player1modelo.Angt2    = KeyFrame1[0].Angt2;
			player1modelo.Angc1    = KeyFrame1[0].Angc1;
			player1modelo.Angc2    = KeyFrame1[0].Angc2;
			player1modelo.Angbi1   = KeyFrame1[0].Angbi1;
			player1modelo.Angbi2   = KeyFrame1[0].Angbi2;
			player1modelo.Angbib   = KeyFrame1[0].Angbib;
			player1modelo.Angbd1   = KeyFrame1[0].Angbd1;
			player1modelo.Angbd2   = KeyFrame1[0].Angbd2;
			player1modelo.Angbdb   = KeyFrame1[0].Angbdb;
			player1modelo.Angpizq  = KeyFrame1[0].Angpizq;
			player1modelo.Angpder  = KeyFrame1[0].Angpder;
			player1modelo.Angpizqb = KeyFrame1[0].Angpizqb;
			player1modelo.Angpderb = KeyFrame1[0].Angpderb;
			player1modelo.Angpi    = KeyFrame1[0].Angpi;
			player1modelo.Angpd    = KeyFrame1[0].Angpd;
			player1modelo.Xtor     = KeyFrame1[0].Xtor;
			player1modelo.Ytor     = KeyFrame1[0].Ytor;
			player1modelo.Ztor     = KeyFrame1[0].Ztor;

			play=true;
			playIndex=0;
			tipoAnim=1;
			
		}
	}

	if(GetKeyState(VK_DOWN) & 0x80)
	{
		player1.Dir=2;

		ControlPersonaje(4);

		if(play==false)
		{
			player1modelo.Angt1    = KeyFrame1[0].Angt1;
			player1modelo.Angt2    = KeyFrame1[0].Angt2;
			player1modelo.Angc1    = KeyFrame1[0].Angc1;
			player1modelo.Angc2    = KeyFrame1[0].Angc2;
			player1modelo.Angbi1   = KeyFrame1[0].Angbi1;
			player1modelo.Angbi2   = KeyFrame1[0].Angbi2;
			player1modelo.Angbib   = KeyFrame1[0].Angbib;
			player1modelo.Angbd1   = KeyFrame1[0].Angbd1;
			player1modelo.Angbd2   = KeyFrame1[0].Angbd2;
			player1modelo.Angbdb   = KeyFrame1[0].Angbdb;
			player1modelo.Angpizq  = KeyFrame1[0].Angpizq;
			player1modelo.Angpder  = KeyFrame1[0].Angpder;
			player1modelo.Angpizqb = KeyFrame1[0].Angpizqb;
			player1modelo.Angpderb = KeyFrame1[0].Angpderb;
			player1modelo.Angpi    = KeyFrame1[0].Angpi;
			player1modelo.Angpd    = KeyFrame1[0].Angpd;
			player1modelo.Xtor     = KeyFrame1[0].Xtor;
			player1modelo.Ytor     = KeyFrame1[0].Ytor;
			player1modelo.Ztor     = KeyFrame1[0].Ztor;

			play=true;
			playIndex=0;
			tipoAnim=1;
			
		}
	}

	if(GetKeyState(VK_LEFT) & 0x80)
	{
		ControlPersonaje(2);

		if(play==false)
		{
			player1modelo.Angt1    = KeyFrame1[0].Angt1;
			player1modelo.Angt2    = KeyFrame1[0].Angt2;
			player1modelo.Angc1    = KeyFrame1[0].Angc1;
			player1modelo.Angc2    = KeyFrame1[0].Angc2;
			player1modelo.Angbi1   = KeyFrame1[0].Angbi1;
			player1modelo.Angbi2   = KeyFrame1[0].Angbi2;
			player1modelo.Angbib   = KeyFrame1[0].Angbib;
			player1modelo.Angbd1   = KeyFrame1[0].Angbd1;
			player1modelo.Angbd2   = KeyFrame1[0].Angbd2;
			player1modelo.Angbdb   = KeyFrame1[0].Angbdb;
			player1modelo.Angpizq  = KeyFrame1[0].Angpizq;
			player1modelo.Angpder  = KeyFrame1[0].Angpder;
			player1modelo.Angpizqb = KeyFrame1[0].Angpizqb;
			player1modelo.Angpderb = KeyFrame1[0].Angpderb;
			player1modelo.Angpi    = KeyFrame1[0].Angpi;
			player1modelo.Angpd    = KeyFrame1[0].Angpd;
			player1modelo.Xtor     = KeyFrame1[0].Xtor;
			player1modelo.Ytor     = KeyFrame1[0].Ytor;
			player1modelo.Ztor     = KeyFrame1[0].Ztor;

			play=true;
			playIndex=0;
			tipoAnim=1;
			
		}
	}

	if(GetKeyState(VK_RIGHT) & 0x80)
	{
		ControlPersonaje(1);

		if(play==false)
		{
			player1modelo.Angt1    = KeyFrame1[0].Angt1;
			player1modelo.Angt2    = KeyFrame1[0].Angt2;
			player1modelo.Angc1    = KeyFrame1[0].Angc1;
			player1modelo.Angc2    = KeyFrame1[0].Angc2;
			player1modelo.Angbi1   = KeyFrame1[0].Angbi1;
			player1modelo.Angbi2   = KeyFrame1[0].Angbi2;
			player1modelo.Angbib   = KeyFrame1[0].Angbib;
			player1modelo.Angbd1   = KeyFrame1[0].Angbd1;
			player1modelo.Angbd2   = KeyFrame1[0].Angbd2;
			player1modelo.Angbdb   = KeyFrame1[0].Angbdb;
			player1modelo.Angpizq  = KeyFrame1[0].Angpizq;
			player1modelo.Angpder  = KeyFrame1[0].Angpder;
			player1modelo.Angpizqb = KeyFrame1[0].Angpizqb;
			player1modelo.Angpderb = KeyFrame1[0].Angpderb;
			player1modelo.Angpi    = KeyFrame1[0].Angpi;
			player1modelo.Angpd    = KeyFrame1[0].Angpd;
			player1modelo.Xtor     = KeyFrame1[0].Xtor;
			player1modelo.Ytor     = KeyFrame1[0].Ytor;
			player1modelo.Ztor     = KeyFrame1[0].Ztor;

			play=true;
			playIndex=0;
			tipoAnim=1;
			
		}
	}

	if(!(GetKeyState(VK_UP) & 0x80 || GetKeyState(VK_DOWN) & 0x80
		 || GetKeyState(VK_LEFT) & 0x80  || GetKeyState(VK_RIGHT) & 0x80))
	{
		player1.Dir=0;

		play=false;

		player1modelo.Angt1    = 0.0f;
		player1modelo.Angt2    = 0.0f;
		player1modelo.Angc1    = 0.0f;
		player1modelo.Angc2    = 0.0f;
		player1modelo.Angbi1   = 0.0f;
		player1modelo.Angbi2   = 0.0f;
		player1modelo.Angbib   = 0.0f;
		player1modelo.Angbd1   = 0.0f;
		player1modelo.Angbd2   = 0.0f;
		player1modelo.Angbdb   = 0.0f;
		player1modelo.Angpizq  = 0.0f;
		player1modelo.Angpder  = 0.0f;
		player1modelo.Angpizqb = 0.0f;
		player1modelo.Angpderb = 0.0f;
		player1modelo.Angpi    = 0.0f;
		player1modelo.Angpd    = 0.0f;
		player1modelo.Xtor     = 0.0f;
		player1modelo.Ytor     = 0.0f;
		player1modelo.Ztor     = 0.0f;
		
	}
		

	if(keys[VK_PRIOR])
	{
		if(player1.PosicionObj.y < 20.0f)
		{
			player1.PosicionObj.y+=0.4f;
			player1.CamaraPosAlt+=0.4f;
			player1.CamaraObjAlt+=0.4f;

			player1.PosicionCam.y=player1.CamaraPosAlt;
			player1.ObjetivoCam=player1.PosicionObj;
			player1.ObjetivoCam.y=player1.CamaraObjAlt;
		}
	}
	if(keys[VK_NEXT])
	{
		if(player1.PosicionObj.y > 0.0f)
		{
			player1.PosicionObj.y-=0.4f;
			player1.CamaraPosAlt-=0.4f;
			player1.CamaraObjAlt-=0.4f;

			player1.PosicionCam.y=player1.CamaraPosAlt;
			player1.ObjetivoCam=player1.PosicionObj;
			player1.ObjetivoCam.y=player1.CamaraObjAlt;
		}
	}

	if(keys[VK_HOME])
	{
		ControlPersonaje(5);
	}

	if(keys[VK_END])
	{
		ControlPersonaje(6);
	}

	//Controles de la iluminación
	if (keys['Z'])
		LightPos[0] += 1.0f; //Hacia la derecha

	if (keys['X'])
		LightPos[0] -= 1.0f; //Hacia la izquierda

	if (keys['C'])
		LightPos[1] += 1.0f; //Hacia arriba

	if (keys['V'])
		LightPos[1] -= 1.0f; //Hacia abajo

	if (keys['B'])
		LightPos[2] += 1.0f; //Hacia adelante

	if (keys['N'])
		LightPos[2] -= 1.0f; //Hacia atrás

	return TRUE;
}

//Control de sonidos por teclado
void ManejaTeclado(FMOD_SYSTEM *system, FMOD_RESULT result)
{
	if((GetAsyncKeyState(VK_SPACE)&1) ==1) //Otra forma de usar el teclado: Detecta el evento de pulsar la tecla
	{
		result = FMOD_System_PlaySound(system, FMOD_CHANNEL_FREE, sound[1], 0, &channel[1]);
        ERRCHECK(result);
	}
	
}