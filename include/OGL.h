#pragma once

#ifdef _WIN32    	
	#include "glew.h"
#ifndef WIN32_LEAN_AND_MEAN
	#define WIN32_LEAN_AND_MEAN
#else
#endif
	#include <windows.h>
	#include <gl/gl.h>
	#include "glut.h"
	#include <stdio.h>
	#include <assert.h>
#else
	#include <GLUT/glut.h>
	#include <OpenGL/glext.h>
	#include <OpenGL/gl.h>
	#include <OpenGL/glu.h>	
#endif

class Object
{
public:
	Object():mnObjID(0){}
	GLuint getID()const{ return mnObjID; }
protected:
	GLuint mnObjID;
};
class Shader: public Object
{
public:
	Shader(){}
	void load(const char *sSource)
	{
		glShaderSource(getID(),1,&sSource,0);
		glCompileShader(getID());
		checkCompileStatus();
	}
	

	void loadFromFile(const char *sFileName)
	{
		char *vs = readTextFile(sFileName);
		const char * vv = vs;
		glShaderSource(getID(), 1, &vv,NULL);
		delete [] vs;
		glCompileShader(getID());
		checkCompileStatus();
	}
	void checkCompileStatus()
	{
		int bStatus;
		glGetShaderiv(getID(),GL_COMPILE_STATUS,&bStatus);
		//assert( bStatus );
		if( !bStatus ){
			int nInfoLogLength;
			glGetShaderiv(getID(),GL_INFO_LOG_LENGTH,&nInfoLogLength); 
			char *pInfoLog = new char[nInfoLogLength+1];
			glGetShaderInfoLog(getID(),nInfoLogLength, &nInfoLogLength,pInfoLog); 
			printf("%s",pInfoLog);
		} 
	}
	static char *readTextFile(const char *fn)
	{
		FILE *fp;
		char *content = NULL;
		int count=0;
		if (fn != NULL) {
			fopen_s(&fp,fn,"rt");
			if (fp != NULL) {
				fseek(fp, 0, SEEK_END);
				count = ftell(fp);
				rewind(fp);
				if (count > 0) {
					content = new char[count+1];//(char *)malloc(sizeof(char) * (count+1));
					count = (int)fread(content,sizeof(char),count,fp);
					content[count] = '\0';
				}
				fclose(fp);
			}
		}
		return content;
	}
//private:
};


class VShader: public Shader
{
public:
	VShader()
	{
		mnObjID = glCreateShader(GL_VERTEX_SHADER);
	}
	VShader(const char *sFileName){
		mnObjID = glCreateShader(GL_VERTEX_SHADER);
		loadFromFile(sFileName);
	}
};
class FShader: public Shader
{
public:
	FShader()
	{
		mnObjID = glCreateShader(GL_FRAGMENT_SHADER);
	}
	FShader(const char *sFileName){
		mnObjID = glCreateShader(GL_FRAGMENT_SHADER);
		loadFromFile(sFileName);
	}
};

class Program : public Object
{
public:
	Program(){
		mnObjID = glCreateProgram();
	}
	void attachAndLink(const VShader *pVS,const FShader *pFS)
	{
		if( pVS )
			glAttachShader(mnObjID,pVS->getID()); 
		if( pFS )
			glAttachShader(mnObjID,pFS->getID()); 
		glLinkProgram(mnObjID);
		int bStatus;
		glGetProgramiv(mnObjID,GL_LINK_STATUS,&bStatus); 
		assert( bStatus );
	}
	void useProgram(bool bTrueFalse){
		if( bTrueFalse )
			glUseProgram(mnObjID);
		else
			glUseProgram(0);
	}
	GLuint getAttribLocation(char *name)
	{
		return glGetAttribLocation(getID(),name);
	}
	GLuint getUniformLocation(const char *sName){
		return glGetUniformLocation(getID(),sName); 
	}
	void setUniform1i(const char *sName,int nValue){
		GLuint nLoc = getUniformLocation(sName);
		glUniform1i(nLoc,nValue);
		assert(GL_NO_ERROR == glGetError());
	}
	void setUniform2f(GLuint nLoc,float fX,float fY){
		glUniform2f(nLoc,fX,fY);
		assert(GL_NO_ERROR == glGetError());
	}
	void setUniform2f(const char *sName,float fX,float fY){
		GLuint nLoc = getUniformLocation(sName);
		glUniform2f(nLoc,fX,fY);
		assert(GL_NO_ERROR == glGetError());
	}

	void setUniform2fv(GLuint nLoc,int nCount,float *pVals){
		glUniform2fv(nLoc,nCount,pVals);
		assert(GL_NO_ERROR == glGetError());
	}
	void setUniform2fv(const char *sName,int nCount,float *pVals){
		GLuint nLoc = getUniformLocation(sName);
		glUniform2fv(nLoc,nCount,pVals);
		assert(GL_NO_ERROR == glGetError());
	}
	void setUniform3fv(GLuint nLoc,int nCount,float *pVals){
		glUniform3fv(nLoc,nCount,pVals);
		assert(GL_NO_ERROR == glGetError());
	}
	void setUniform3fv(const char *sName,int nCount,float *pVals){
		GLuint nLoc = getUniformLocation(sName);
		glUniform3fv(nLoc,nCount,pVals);
		assert(GL_NO_ERROR == glGetError());
	}
	void setUniform4fv(GLuint nLoc,int nCount,float *pVals){
		glUniform4fv(nLoc,nCount,pVals);
		assert(GL_NO_ERROR == glGetError());
	}
	void setUniform4fv(const char *sName,int nCount,float *pVals){
		GLuint nLoc = getUniformLocation(sName);
		glUniform4fv(nLoc,nCount,pVals);
		assert(GL_NO_ERROR == glGetError());
	}
};

class CompleteProgram : public Program
{
public:
	void loadFromFile(const char *sVertexShader,const char *sFragShader){
		mVShader.loadFromFile(sVertexShader);
		mFShader.loadFromFile(sFragShader);
		attachAndLink(&mVShader,&mFShader);
	}
	void loadFragmentShader(const char *sFragShader){
		mFShader.loadFromFile(sFragShader);
		attachAndLink(0,&mFShader);
	}
private:
	VShader mVShader;
	FShader mFShader;
};

class Texture2D : public Object
{
public:
	Texture2D(int nWidth,int nHeight,GLenum internalFormat):mnWidth(nWidth),mnHeight(nHeight){
		glGenTextures(1,&mnObjID);
		glBindTexture(GL_TEXTURE_2D, mnObjID);
		glTexImage2D(GL_TEXTURE_2D, 0,internalFormat,nWidth,nHeight, 
			          0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		
		//unsigned char *pP = new unsigned char[nWidth*nHeight*4];
		//memset(pP,128,nWidth*nHeight*4);
		//glPixelStorei(GL_UNPACK_ALIGNMENT,1);
		//glTexSubImage2D(GL_TEXTURE_2D,0,0,0,512,512,GL_RGBA,GL_UNSIGNED_BYTE,pP);
		//delete pP;
		glBindTexture(GL_TEXTURE_2D,0);		
	}
	int getWidth()const{ return mnWidth; }
	int getHeight()const{ return mnHeight; }
	void bind(){ 
		glBindTexture(GL_TEXTURE_2D, mnObjID); 
	}
	void unbind(){ 
		glBindTexture(GL_TEXTURE_2D, 0); 
	}	
private:
	int mnWidth;
	int mnHeight;
};

class FramebufferObject : public Object
{
public:
	FramebufferObject(){
		//glGenFramebuffers(1, &mnObjID);
		glGenFramebuffersEXT(1,&mnObjID);
	}
	~FramebufferObject(){
		//glDeleteFramebuffers(1, &mnObjID);
		glDeleteFramebuffersEXT(1, &mnObjID);
	}
	void bind(){ glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, mnObjID); }
	void unbind(){ glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0); }
	void attach(Texture2D& texture){
		//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,texture.getID(), 0);
		glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,texture.getID(), 0);
		GLuint status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT); 
        switch(status) {                                          
        case GL_FRAMEBUFFER_COMPLETE:                       
            break;                                                
        case GL_FRAMEBUFFER_UNSUPPORTED:                                
		    assert(0); // choose different formats
            break;   
		case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
			assert(0); // choose different formats
            break;   

		/* case FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT:
		 case FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT:
		 case FRAMEBUFFER_INCOMPLETE_FORMATS_EXT:
		 case FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT:
		 case FRAMEBUFFER_INCOMPLETE_READ_BUFFER_EXT:
		 case FRAMEBUFFER_UNSUPPORTED_EXT:*/

          default:                                                
            /* programming error; will fail on all hardware */    
            assert(0);                                            
        }
	}
};


class IndexBufferObject : public Object
{
public:
	IndexBufferObject(){
		glGenBuffers(1, &mnObjID);
		assert(GL_NO_ERROR == glGetError());
	}
	~IndexBufferObject(){
		unbind(); 
		glDeleteBuffers(GL_ELEMENT_ARRAY_BUFFER, &mnObjID);
		assert(GL_NO_ERROR == glGetError());
	}
	void bind(){ 
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mnObjID); 
		assert(GL_NO_ERROR == glGetError());
	}
	void unbind(){ 
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		assert(GL_NO_ERROR == glGetError());
	}
	void set(	GLsizeiptr nSize, const GLvoid * pData, GLenum usage ){
		//GL_STREAM_DRAW, GL_STATIC_DRAW, GL_DYNAMIC_DRAW
		glBufferData(GL_ELEMENT_ARRAY_BUFFER,nSize,pData,usage);
		assert(GL_NO_ERROR == glGetError());
	}
};


class VertexBufferObject : public Object
{
public:
	VertexBufferObject(){
		glGenBuffers(1, &mnObjID);
		assert(GL_NO_ERROR == glGetError());
	}
	~VertexBufferObject(){
		unbind(); 
		glDeleteBuffers(GL_ARRAY_BUFFER, &mnObjID);
		assert(GL_NO_ERROR == glGetError());
	}
	void bind(){ 
		glBindBuffer(GL_ARRAY_BUFFER, mnObjID); 
		assert(GL_NO_ERROR == glGetError());
	}
	void unbind(){ glBindBuffer(GL_ARRAY_BUFFER, 0);
				   assert(GL_NO_ERROR == glGetError());
	}
	void set(	GLsizeiptr nSize, const GLvoid * pData, GLenum usage ){
		//GL_STREAM_DRAW, GL_STATIC_DRAW, GL_DYNAMIC_DRAW
		glBufferData(GL_ARRAY_BUFFER,nSize,pData,usage);
		assert(GL_NO_ERROR == glGetError());
	}
};




    
