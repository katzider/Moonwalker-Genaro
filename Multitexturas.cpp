#include "Main.h"

// Our function pointers for the ARB multitexturing functions
PFNGLMULTITEXCOORD2FARBPROC		glMultiTexCoord2fARB	 = NULL;
PFNGLACTIVETEXTUREARBPROC		glActiveTextureARB		 = NULL;
PFNGLCLIENTACTIVETEXTUREPROC    glClientActiveTexture    = NULL;
PFNGLCLIENTACTIVETEXTUREARBPROC	glClientActiveTextureARB = NULL;


CMultitexturas::CMultitexturas()
{
	
}

CMultitexturas::~CMultitexturas()
{
	
}

void CMultitexturas::InitMultitext(HWND hWnd)
{
	// Here we initialize our multitexturing functions
	glActiveTextureARB	= (PFNGLACTIVETEXTUREARBPROC)		wglGetProcAddress("glActiveTextureARB");
	glMultiTexCoord2fARB= (PFNGLMULTITEXCOORD2FARBPROC)		wglGetProcAddress("glMultiTexCoord2fARB");
	glClientActiveTexture= (PFNGLCLIENTACTIVETEXTUREPROC)   wglGetProcAddress("glClientActiveTexture");
	glClientActiveTextureARB= (PFNGLCLIENTACTIVETEXTUREARBPROC)	wglGetProcAddress("glClientActiveTextureARB");

	// We should have our multitexturing functions defined and ready to go now, but let's make sure
	// that the current version of OpenGL is installed on the machine.  If the extension functions
	// could not be found, our function pointers will be NULL.
	if(!glActiveTextureARB || !glMultiTexCoord2fARB)
	{
		// Print an error message and quit.
		MessageBox(hWnd, "Your current setup does not support multitexturing", "Error", MB_OK);
		PostQuitMessage(0);
	}

}