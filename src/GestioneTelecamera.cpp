//Movimento della telecamera
#include "Lib.h"
#include "Strutture.h"

extern vector<Mesh> Scena;
extern ViewSetup SetupTelecamera;
extern float cameraSpeed;
extern int height, width;
extern bool firstMouse;
extern float lastX, lastY,Theta, Phi;

bool isCameraColliding(vec4 newPosition) {
  for (Mesh& sceneObject : Scena) {
    if (!sceneObject.collisionEnabled) {
      return false;
    }
    vec3 positionDifference = newPosition - sceneObject.ancora_world;
    float distance = length(positionDifference);
    printf("Distance: %f | ObjectName: %s\n", distance, sceneObject.nome.c_str());
    return distance < sceneObject.collisionSphereRadius;
  }
  return false;
}

// Camera movement section
void moveCameraForward() {
	vec4 direction = SetupTelecamera.target - SetupTelecamera.position;
  vec4 newPosition = SetupTelecamera.position + (direction * cameraSpeed);
  if (isCameraColliding(newPosition)) {
    return;
  }
  SetupTelecamera.position = newPosition;
}
void moveCameraBack() {
	vec4 direction = SetupTelecamera.target - SetupTelecamera.position;
  vec4 newPosition = SetupTelecamera.position - (direction * cameraSpeed);
  if (isCameraColliding(newPosition)) {
    return;
  }
  SetupTelecamera.position = newPosition;
}
void moveCameraLeft() {
  // Determining movement direction based on target
	vec3 direction = SetupTelecamera.target - SetupTelecamera.position;
  // Determines the vector perpendicular to the up and the direction vector
	vec3 slide_vector = cross(direction, vec3(SetupTelecamera.upVector)) * cameraSpeed;
  // Moves everything horizontally
  vec4 newPosition = SetupTelecamera.position - vec4(slide_vector, 0.0f);
	vec4 newTarget = SetupTelecamera.target - vec4(slide_vector, 0.0f);
  if (isCameraColliding(newPosition)) {
    return;
  }
  SetupTelecamera.position = newPosition;
  SetupTelecamera.target = newTarget;
}
void moveCameraRight() {
	vec3 direction = SetupTelecamera.target - SetupTelecamera.position;
	vec3 slide_vector = normalize(cross(direction, vec3(SetupTelecamera.upVector))) * cameraSpeed;
  vec4 newPosition = SetupTelecamera.position + vec4(slide_vector, 0.0f);
	vec4 newTarget = SetupTelecamera.target + vec4(slide_vector, 0.0f);
  if (isCameraColliding(newPosition)) {
    return;
  }
  SetupTelecamera.position = newPosition;
  SetupTelecamera.target = newTarget;
}
void moveCameraUp() {
	vec3 direction = SetupTelecamera.target - SetupTelecamera.position;
	vec3 slide_vector = normalize(glm::cross(direction, vec3(SetupTelecamera.upVector)));
	vec3 upDirection = cross(direction, slide_vector) * cameraSpeed;
  vec4 newPosition = SetupTelecamera.position - vec4(upDirection, 0.0f);
	vec4 newTarget = SetupTelecamera.target - vec4(upDirection, 0.0f);
  if (isCameraColliding(newPosition)) {
    return;
  }
  SetupTelecamera.position = newPosition;
  SetupTelecamera.target = newTarget;
}
void moveCameraDown() {
	vec4 direction = SetupTelecamera.target - SetupTelecamera.position;
	vec3 slide_vector = normalize(cross(vec3(direction), vec3(SetupTelecamera.upVector)));
	vec3 upDirection = cross(vec3(direction), slide_vector) * cameraSpeed;

	vec4 newPosition = SetupTelecamera.position + vec4(upDirection, 0.0f);
	vec4 newTarget = SetupTelecamera.target + vec4(upDirection, 0.0f);
  if (isCameraColliding(newPosition)) {
    return;
  }
  SetupTelecamera.position = newPosition;
  SetupTelecamera.target = newTarget;
}


/**
 * Updates camera position based on passive mouse movement.
 */
void my_passive_mouse(int xpos, int ypos) {
  float windowCenterX = width / 2.0f;
  float windowCenterY = height / 2.0f;
	ypos = height - ypos;
  float alpha = 0.05f;

	float xoffset = xpos - windowCenterX;
	float yoffset = ypos - windowCenterY;
	lastX = xpos;
	lastY = ypos;
	xoffset *= alpha;
	yoffset *= alpha;
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
	glutWarpPointer((int) width / 2.0f, height / 2.0f);
	glutPostRedisplay();
}
