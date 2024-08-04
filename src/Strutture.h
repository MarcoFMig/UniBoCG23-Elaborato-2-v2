#pragma once
#include "Lib.h"
#include "enum_illuminazione.h" 
#include <string>

#define DEFAULT_COLLISION_TOGGLE	true
#define DEFAULT_COLLISION_RADIUS	2.0f

typedef struct {
	string name;
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	GLfloat shininess;
} Material;

typedef struct {
	vector<vec3> vertici;
	vector<vec4> colori;
	vector <GLuint>  indici;
	vector<vec3> normali;
	vector<vec2> texCoords;
	mat4 Model;
	int sceltaVS;
	GLuint VAO;
	GLuint VBO_G;
	GLuint VBO_C;
	GLuint VBO_normali;
	GLuint EBO_indici;
	GLuint VBO_coord_texture;
	string nome;
	ShadingType shading;
	MaterialType material;
	vec4 ancora_obj;
	vec4 ancora_world;
	bool collisionEnabled = DEFAULT_COLLISION_TOGGLE;
	float collisionSphereRadius = DEFAULT_COLLISION_RADIUS;
} Mesh;

typedef struct {
	vector<vec3> vertici;
	vector<vec4> colori;
	vector <GLuint>  indici;
	vector<vec3> normali;
	vector<vec2> texCoords;
	mat4 ModelM;
	int sceltaVS;
	int sceltaFS;
	GLuint VAO;
	GLuint VBO_G;
	GLuint VBO_C;
	GLuint VBO_normali;
	GLuint EBO_indici;
	GLuint VBO_coord_texture;
	ShadingType shading;
	Material materiale;
	string nome;
	vec4 ancora_obj;
	vec4 ancora_world;
	bool collisionEnabled = DEFAULT_COLLISION_TOGGLE;
	float collisionSphereRadius = DEFAULT_COLLISION_RADIUS;
} MeshObj;

//Gestione Vista

typedef struct {
	vec4 position;
	vec4 target;
	vec4 upVector;
	vec4 direction;
} ViewSetup;

//gestione proiezione
typedef struct {
	float fovY, aspect, near_plane, far_plane;
} PerspectiveSetup;


//Gestione Testo
struct Character {
	unsigned int TextureID; // ID handle of the glyph texture
	glm::ivec2   Size;      // Size of glyph
	glm::ivec2   Bearing;   // Offset from baseline to left/top of glyph
	unsigned int Advance;   // Horizontal offset to advance to next glyph
};

typedef struct {
	string name;
	int value;
} Shader;



typedef struct {
	glm::vec3 position;
	glm::vec3 color;
	GLfloat power;
} point_light;

typedef struct {
	GLuint light_position_pointer;
	GLuint light_color_pointer;
	GLuint light_power_pointer;
	GLuint material_diffuse;
	GLuint material_ambient;
	GLuint material_specular;
	GLuint material_shininess;
} LightShaderUniform;

