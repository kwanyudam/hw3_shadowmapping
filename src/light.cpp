#include "../include/light.h"

Light::Light(GLuint lightID,
		const Eigen::Vector3f position,
		const Eigen::Vector3f ambient,
		const Eigen::Vector3f diffuse,
		const Eigen::Vector3f specular ){
	_position = position;
	_ambient = ambient;
	_diffuse = diffuse;
	_specular = specular;
};

Light::render(){
	return;
};