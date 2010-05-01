#include "Font.h"

CTga Text[2];
GLuint base;

CFont::CFont()
{

}

CFont::~CFont()
{
	
}

void CFont::BuildFont(GLvoid)
{
	Text[0].LoadTGA("Texturas/fuente.tga");

	base=glGenLists(256);
	glBindTexture(GL_TEXTURE_2D, Text[0].texID);
	for (int loop=0; loop<256; loop++)
	{
		float cx=float(loop%16)/16.0f;
		float cy=float(loop/16)/16.0f;

		glNewList(base+loop,GL_COMPILE);
			glBegin(GL_QUADS);
				glTexCoord2f(cx,         1.0f-cy-0.0625f); glVertex2i( 0, 0);
				glTexCoord2f(cx+0.0625f, 1.0f-cy-0.0625f); glVertex2i(16, 0);
				glTexCoord2f(cx+0.0625f, 1.0f-cy);		   glVertex2i(16,16);
				glTexCoord2f(cx,         1.0f-cy);		   glVertex2i( 0,16);
			glEnd();
			glTranslated(10,0,0);		//La distancia hacia la derecha para dibujar el siguiente caracter
		glEndList();
	}
}

void CFont::glPrint(float escala, GLint x, GLint y, const char *string, ...)
{
	char		text[256];
	va_list		ap;

	if (string == NULL)
		return;

	va_start(ap, string);
	    vsprintf(text, string, ap);
	va_end(ap);

	glBindTexture(GL_TEXTURE_2D, Text[0].texID);
	glPushMatrix();
	glLoadIdentity();
	glTranslated(x,y,0);
	glListBase(base-32);
	glScalef(escala,escala,1.0f);
	glCallLists(strlen(text), GL_UNSIGNED_BYTE, text);
	glPopMatrix();
}

void CFont::DestroyFont()
{
	Text[0].Elimina();
	glDeleteLists(base,256);
}