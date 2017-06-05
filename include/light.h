#include "../external/Eigen/Core"
#include "../external/Eigen/Geometry"

class Light{
private:
	Eigen::Vector3f _position;
	Eigen::Vector3f _ambient;
	Eigen::Vector3f _diffuse;
	Eigen::Vector3f _specular;
public:
	Light(){};
	Light(GLuint lightID,
		const Eigen::Vector3f position,
		const Eigen::Vector3f ambient,
		const Eigen::Vector3f diffuse,
		const Eigen::Vector3f specular);
	void render();
};