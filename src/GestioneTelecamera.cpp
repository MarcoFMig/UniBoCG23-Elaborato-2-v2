//Movimento della telecamera
#include "Lib.h"
#include "Strutture.h"
extern ViewSetup SetupTelecamera;
extern float cameraSpeed;
extern int height, widht;
extern bool firstMouse;
extern float lastX, lastY,Theta, Phi;

// Camera movement section
void moveCameraForward() {
	vec4 direction = SetupTelecamera.target - SetupTelecamera.position;
	SetupTelecamera.position += direction * cameraSpeed;
}
void moveCameraBack() {
	vec4 direction = SetupTelecamera.target - SetupTelecamera.position;
	SetupTelecamera.position -= direction * cameraSpeed;
}
void moveCameraLeft() {
  // Determining movement direction based on target
	vec3 direction = SetupTelecamera.target - SetupTelecamera.position;
  // Determines the vector perpendicular to the up and the direction vector
	vec3 slide_vector = cross(direction, vec3(SetupTelecamera.upVector)) * cameraSpeed;
  // Moves everything horizontally
	SetupTelecamera.position -= vec4(slide_vector, 0.0f);
	SetupTelecamera.target -= vec4(slide_vector, 0.0f);
}
void moveCameraRight() {
	vec3 direction = SetupTelecamera.target - SetupTelecamera.position;
	vec3 slide_vector = normalize(cross(direction, vec3(SetupTelecamera.upVector))) * cameraSpeed;
	SetupTelecamera.position += vec4(slide_vector, 0.0f);
	SetupTelecamera.target += vec4(slide_vector, 0.0f);
}
void moveCameraUp() {
	vec3 direction = SetupTelecamera.target - SetupTelecamera.position;
	vec3 slide_vector = normalize(glm::cross(direction, vec3(SetupTelecamera.upVector)));
	vec3 upDirection = cross(direction, slide_vector) * cameraSpeed;
	SetupTelecamera.position -= vec4(upDirection, 0.0f);
	SetupTelecamera.target -= vec4(upDirection, 0.0f);
}
void moveCameraDown() {
	vec4 direction = SetupTelecamera.target - SetupTelecamera.position;
	vec3 slide_vector = normalize(cross(vec3(direction), vec3(SetupTelecamera.upVector)));
	vec3 upDirection = cross(vec3(direction), slide_vector) * cameraSpeed;
	SetupTelecamera.position += vec4(upDirection, 0.0f);
	SetupTelecamera.target += vec4(upDirection, 0.0f);
}

/**
 * Updates camera position based on passive mouse movement.
 */
void my_passive_mouse(int xpos, int ypos) {
	float alfa = 0.05;
	ypos = height - ypos;
	if (firstMouse)	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = ypos - lastY;
	lastX = xpos;
	lastY = ypos;
	xoffset *= alfa;
	yoffset *= alfa;
	Theta += xoffset;   //Aggiorno l'angolo Theta sommandogli l'offset della posizione x del mouse
	Phi += yoffset;  //Aggiorno l'angolo Phi sommandogli l'offset della posizione y del mouse 

	// Facciamo si' che l'angolo di Phi vari tra -90 e 90.
	if (Phi > 89.0f) {
		Phi = 89.0f;
	}
	if (Phi < -89.0f) {
		Phi = -89.0f;
	}

	//Ricavo le coordinate x,y,z del punto che sulla sfera � individuato dagli angoli Theta e Phi
	vec3 front;
	front.x = cos(radians(Theta)) * cos(radians(Phi));
	front.y = sin(radians(Phi));
	front.z = sin(radians(Theta)) * cos(radians(Phi));
	//Considero la direzione normalizzata (nota la quarta componente uguale a 0 indica una direzione
	SetupTelecamera.direction = vec4(normalize(front), 0.0); //Aggiorno la direzione della telecamera
	SetupTelecamera.target = SetupTelecamera.position + SetupTelecamera.direction; //aggiorno il punto in cui guarda la telecamera
	glutPostRedisplay();
}
