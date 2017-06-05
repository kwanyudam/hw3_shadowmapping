#include <iostream>
#include <fstream>

using namespace std;

class Object{
private:
	Eigen::Vector3d vertices[20000];
	Eigen::Vector3d normals[20000];
	Eigen::Vector3i faces[20000];

	int vertexCount;
	int faceCount;
	double _scale;


public:
	Object(){};
	Object(const char* filename, double scale){
		_scale = scale;

		cout<<"Loading Object : "<<filename<<" ....\n";
		vertexCount=0;
		faceCount=0;

		for(int i=0;i<20000;i++){
			normals[i] = Eigen::Vector3d::Zero();
		}

		ifstream fin(filename);

		char s;
		int i;
		while(!fin.eof()){
			fin>>s;
			if(s=='v'){
				fin>>vertices[vertexCount][0]>>vertices[vertexCount][1]>>vertices[vertexCount][2];
				vertexCount++;
			}else if(s=='f'){
				fin>>faces[faceCount][0]>>faces[faceCount][1]>>faces[faceCount][2];

				Eigen::Vector3d tpn1, tpn2, tpn3;
				tpn1 = vertices[faces[faceCount][0]-1] - vertices[faces[faceCount][1]-1];
				tpn2 = vertices[faces[faceCount][1]-1] - vertices[faces[faceCount][2]-1];
				tpn3 = tpn1.cross(tpn2);
				tpn3.normalize();

				normals[faces[faceCount][0]-1] += tpn3;
				normals[faces[faceCount][1]-1] += tpn3;
				normals[faces[faceCount][2]-1] += tpn3;

				faceCount++;
			}else
				break;
		}

		for(int i=0;i<vertexCount;i++){
			normals[i].normalize();
		}

		cout<<"==Loading Complete!!==\n";
		cout<<"Vertices : "<<vertexCount<<"\n";
		cout<<"Faces : "<<faceCount<<"\n";
		cout<<"======================\n";
	};
	void render(){
		//glColor3f(0.8, 0.8, 0.8);
		GLfloat green[] = {0, 1, 0, 1};
		glMaterialfv(GL_FRONT, GL_AMBIENT, green);
		glPushMatrix();

		glScalef(_scale, _scale, _scale);
		glTranslatef(0, 0, 0);

		for(int i=0;i<faceCount;i++){
			glBegin(GL_POLYGON);
			for(int j=0;j<3;j++){
				glVertex3f(vertices[faces[i][j]-1][0], vertices[faces[i][j]-1][1], vertices[faces[i][j]-1][2]);
				glNormal3f(normals[faces[i][j]-1][0], normals[faces[i][j]-1][1], normals[faces[i][j]-1][2]);
			}
			glEnd();
		}
		glPopMatrix();
	}
};