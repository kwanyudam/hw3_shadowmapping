#pragma once
#include "../external/Eigen/Core"
#include "../external/Eigen/Geometry"	

#include <stdio.h>
#include <iostream>
#include <fstream>

#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include <GL/glext.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include <math.h>
#define _USE_MATH_DEFINES

using namespace std;

typedef Eigen::Vector3f V3;
typedef Eigen::Vector4f V4;
typedef Eigen::Matrix3f M3;
typedef Eigen::Matrix4f M4;

struct LightObject{
	GLuint lightID;
	V4 pos;
	V3 lookup;
	V3 lookat;
	V4 ambient;
	V4 diffuse;
	V4 specular;
};


class ShadowMap{
private:	
	int windowWidth, windowHeight;
public:
	GLuint fboID;
	GLuint rboID;
	GLuint rTexID;
	ShadowMap(){}
	ShadowMap(int width, int height, int renderType){
		windowWidth = width;
		windowHeight = height;
		if(renderType==0){
			glGenTextures(2, &rTexID);
			glBindTexture(GL_TEXTURE_2D, rTexID);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
			glBindTexture(GL_TEXTURE_2D, 0);

			glGenFramebuffersEXT(2, &fboID);
			glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fboID);

			glGenRenderbuffersEXT(2, &rboID);
			glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, rboID);
			glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT, width, height);

			glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, 0);

			glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, rTexID, 0);
			glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, rboID);

			if(glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT)==GL_FRAMEBUFFER_COMPLETE_EXT)
				cout<<"Render fboID Binding Success!\n";
			else
				cout<<"Error : "<<glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT)<<"\n";

			glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
		}else if(renderType == 1){
			
			glGenTextures(1, &rTexID);
			glBindTexture(GL_TEXTURE_2D, rTexID);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, windowWidth, windowHeight, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, 0);
			glBindTexture(GL_TEXTURE_2D, 0);

			glGenFramebuffersEXT(1, &fboID);
			glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fboID);
			glDrawBuffer(GL_NONE);
			glReadBuffer(GL_NONE);

			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, rTexID, 0);

			if(glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT)==GL_FRAMEBUFFER_COMPLETE_EXT)
				cout<<"Depth fboID Binding Success!\n";
			else{
				cout<<"Depth FBO Error : "<<glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT)<<"\n";
			}

			glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);		
		}
	}

	void beginShadowMap(){

		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fboID);
	}

	void endShadowMap(){
		glBindFramebuffer(GL_FRAMEBUFFER_EXT, 0);

		glBindTexture(GL_TEXTURE_2D, rTexID);
		glGenerateMipmap(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0);
	}



};