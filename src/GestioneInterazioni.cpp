#include "Lib.h"
#include "geometria.h"
#include "Gestione_VAO.h"
#include "GestioneTelecamera.h"
#include "enum.h"
extern bool visualizzaAncora;
extern int cont_cubi,cont_pir;
extern vector<Mesh> Scena;
extern string stringa_asse, Operazione;
extern vec3 asse;
extern mat4 Projection, View;
extern ViewSetup SetupTelecamera;
extern bool clickObject;
extern Mesh  Cubo, Piramide, Piano;
extern int selected_obj;
extern int height, width;
extern float raggio_sfera;
extern bool clickObject, moving_trackball;
extern int last_mouse_pos_X, last_mouse_pos_Y;
extern ViewSetup SetupTelecamera;
extern mat4 rotation_matrix;

/**
 * Recalculates model's matrix,
 * this function is mainly used to perform some transformations to existing objects in the scene.
 * @param translation_vector  the translation vector
 * @param rotation_vector     the rotation vector
 * @param angle               
 * @param scale_factor        
 */
void modifyModelMatrix(vec3 translation_vector, vec3 rotation_vector, GLfloat angle, GLfloat scale_factor) {
	mat4 traslation = glm::translate(glm::mat4(1), translation_vector);
	mat4 scale = glm::scale(glm::mat4(1), glm::vec3(scale_factor, scale_factor, scale_factor));
	mat4 rotation = glm::rotate(glm::mat4(1), angle, rotation_vector);
	Scena[selected_obj].Model = Scena[selected_obj].Model * scale * rotation * traslation;
	glutPostRedisplay();
}

void toggleBoundingSphere() {
  Scena[selected_obj].collisionEnabled = !Scena[selected_obj].collisionEnabled;
  printf("%s bounding sphere\n", Scena[selected_obj].collisionEnabled ? "Enabled" : "Disabled");
}
void increaseBoundingSphereRange(bool increase) {
  float delta = round(0.05f * (increase
    ? 1
    : -1)* 100.0f) / 100.0f;
  Scena[selected_obj].collisionSphereRadius += delta;
  printf("Setting collision sphere radius to: %.2f\n", Scena[selected_obj].collisionSphereRadius);
}

void keyboardPressedEvent(unsigned char key, int x, int y) {
	char* intStr;
	string str;
	switch (key) {
    // Camera movement section
    case 'w':
      moveCameraForward();
    break;
    case 's':
      moveCameraBack();
    break;
    case 'a':
      moveCameraLeft();
    break;
    case 'd':
      moveCameraRight();
    break;
    // End camera movement sections
    // Show object anchors
    case 'u':
      toggleBoundingSphere();
    break;
    case 'j':
      increaseBoundingSphereRange(true);
    break;
    case 'm':
      increaseBoundingSphereRange(false);
    break;
    case 'v':
      visualizzaAncora = TRUE;
    break;
    // Activate translation transformations
    case 'g':
      OperationMode = TRASLATING;
      Operazione = "TRASLAZIONE";
    break;
    // Activate rotation transformations
    case 'r':
      OperationMode = ROTATING;
      Operazione = "ROTAZIONE";
    break;
    // Activate scale transformations
    case 'S':
      OperationMode = SCALING;
      Operazione = "SCALATURA";
    break;
    // Quit program
    case 27:
      glutLeaveMainLoop();
    break;
    // Target axis selections (for previous transformations)
    case 'x':
      WorkingAxis = X;
      stringa_asse = " Asse X";
    break;
    case 'y':
      WorkingAxis = Y;
      stringa_asse = " Asse Y";
    break;
    case 'z':
      WorkingAxis = Z;
      stringa_asse = " Asse Z";
    break;
    default:
    break;
	}
  // Transformation input elaboration
	switch (WorkingAxis) {
    case X:	asse = glm::vec3(1.0f, 0.0f, 0.0f);
    break;
    case Y: asse = glm::vec3(0.0f, 1.0f, 0.0f);
    break;
    case Z: asse = glm::vec3(0.0f, 0.0f, 1.0f);
    break;
    default:
    break;
	}
	float amount = 0.01f;
  switch (key) {
    case ('+'):
		  amount *= 1;
      break;
    case ('-'):
      amount *= -1;
      break;
  }
	switch (OperationMode) {
    case TRASLATING:
      modifyModelMatrix(asse * amount, asse, 0.0f, 1.0f);
    break;
    case ROTATING:
      modifyModelMatrix(glm::vec3(0), asse, amount * 2.0f, 1.0f);
    break;
    case SCALING:
      modifyModelMatrix(glm::vec3(0), asse, 0.0f, 1.0f + amount);
    break;
	}
	glutPostRedisplay();
}

void keyboardReleasedEvent(unsigned char key, int x, int y){
		switch (key)	{
      // Anchor view toggle
      case 'v':
        visualizzaAncora = FALSE;
      break;
      default:
      break;
		}
	glutPostRedisplay();
}

/**
 * Gets ray direction using the mouse click.
 * @param mouse_x the x coordinate of the mouse on screen
 * @param mouse_x the y coordinate of the mouse on screen
 * @returns       a ray direction in normalized world coordinates
 */
vec3 get_ray_from_mouse(float mouse_x, float mouse_y) {
	mouse_y = height - mouse_y;
	float ndc_x = (2.0f * mouse_x) / width - 1.0;
	float ndc_y = (2.0f * mouse_y) / height - 1.0;
	float ndc_z = -1.0f;
	// Clip space
	vec4 P_clip = vec4(ndc_x, ndc_y, ndc_z, 1.0);
	// Clip to eye space coordinates
	vec4 ViewModelp = inverse(Projection) * P_clip;
	ViewModelp.w = 1.0f;
	// Eye space to world coordinates
	vec4 Pw = inverse(View) * ViewModelp;
	vec3 ray_wor = normalize(vec3(Pw) - vec3(SetupTelecamera.position));
	return ray_wor;
}

/**
 * Checks for intersections with a sphere of another object.
 * @param O                     ray origin
 * @param d                     normalized ray direction
 * @param sphere_centre_wor     sphere centre in world coordinates
 * @param sphere_radius         radius of the sphere
 * @param intersection_distance value representing the intersection distance
 * @returns                     true if there are intersections IN FRONT
 */
bool ray_sphere(vec3 O, vec3 d, vec3 sphere_centre_wor, float sphere_radius, float* intersection_distance) {
	//Calcoliamo O-C
	vec3 dist_sfera = O - sphere_centre_wor;
	float b = dot(dist_sfera, d);
	float cc = dot(dist_sfera, dist_sfera) - sphere_radius * sphere_radius;
	float delta = b * b - cc;
  // Discriminant less than 0, the ray does not intersect with the sphere
	if (delta < 0) {
		return false;
  }
	// Discriminant greater than 0, the ray cuts trough the sphere, we need to get the closest point
	if (delta > 0.0f) {
		float t_a = -b + sqrt(delta);
		float t_b = -b - sqrt(delta);
		*intersection_distance = t_b;
    // Ignoring intersections behind
		if (t_a < 0.0) {
			if (t_b < 0)
				return false;
		}
		return true;
	}
  // Discriminant equals 0, the ray touches the sphere
	if (delta == 0.0f) {
		float t = -b + sqrt(delta);
		if (t < 0.0f)
			return false;
		*intersection_distance = t;
		return true;
	}
	return false;
}

void mouse(int button, int state, int x, int y) {
	switch (button) {
    // Scene navigation mode
    case GLUT_LEFT_BUTTON:
      if (state == GLUT_DOWN) { moving_trackball = true; }
      if (state == GLUT_UP) { moving_trackball = false; }
      OperationMode = NAVIGATION;
      last_mouse_pos_X = x;
      last_mouse_pos_Y = y;
    break;
    case GLUT_RIGHT_BUTTON:
      // If ctrl is pressed, object selection mode is enabled
      if (state == GLUT_DOWN && glutGetModifiers() == GLUT_ACTIVE_CTRL) {
        float xmouse = x;
        float ymouse = y;
        vec3 ray_wor = get_ray_from_mouse(xmouse, ymouse);

        selected_obj = -1;
        float closest_intersection = 0.0f;
        for (int i = 0; i < Scena.size(); i++) {
          float t_dist = 0.0f;
          if (ray_sphere(SetupTelecamera.position, ray_wor, Scena[i].ancora_world, raggio_sfera, &t_dist)) {
            if (selected_obj == -1 || t_dist < closest_intersection) {
              selected_obj = i;
              closest_intersection = t_dist;
            }
          }
        }
        if (selected_obj > -1) {
          printf("\nOggetto selezionato\t%d -> %s \n", selected_obj, Scena[selected_obj].nome.c_str());
        }
      }
    break;
    default:
    break;
	}
	glutPostRedisplay();
}

/**
 * Virtual trackball function
 * @param x x coodrinate of the mouse
 * @param y y coordinate of the mouse
 * @returns normalized click point in the trackball sphere
 */
glm::vec3 getTrackBallPoint(float x, float y) {
	float zTemp;
	glm::vec3 point;
  // Using width and height of the window to map the click on -1 and 1 coords
	point.x = (2.0f * x - width) / width;
	point.y = (height - 2.0f * y) / height;
	// Z coord calculation based on x^2 + y^2 + z^2 = r^2
	zTemp = 1.0f - pow(point.x, 2.0f) - pow(point.y, 2.0f);
	if (zTemp < 0) {
		point.z = 0;
  }	else {
		point.z = sqrt(zTemp);
  }
  // Normalization between -1 and 1 of the point
	return normalize(point);
}

/**
 * Moves camera while holding on a target.
 * @param x x coodrinate of the mouse
 * @param y y coordinate of the mouse
 */
void mouseActiveMotion(int x, int y) {
	float velocity = 7.0;
	if (!moving_trackball) {
		return;
	}
	vec3 p2 = getTrackBallPoint(x, y);
	vec3 p1 = getTrackBallPoint(last_mouse_pos_X, last_mouse_pos_Y);
	float dx, dy, dz;
	dx = p2.x - p1.x;
	dy = p2.y - p1.y;
	dz = p2.z - p1.z;
	if (dx || dy || dz) {
		float angle = acos(dot(p1, p2))*velocity;
		vec3 rotation_vec = glm::cross(p1, p2);
		SetupTelecamera.direction = SetupTelecamera.position -SetupTelecamera.target;
		SetupTelecamera.position = SetupTelecamera.target + glm::rotate(glm::mat4(1.0f), glm::radians(-angle), rotation_vec) * SetupTelecamera.direction;
	}
	last_mouse_pos_X = x; last_mouse_pos_Y = y;
	glutPostRedisplay();
}
