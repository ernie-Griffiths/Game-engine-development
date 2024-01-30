Level.cpp
#include "Level.h"
#include "Camera.h"
#include "Light.h"
#include "Shader.h"
#include "material.h"
#include "MeshRenderer.h"
#include "CubeMesh.h"
#include "..\Core\FBXMesh.h"
#include "Script.h"
#include "SphereMesh.h"



namespace Merlin {

	Level::~Level()
	{
		delete gameObjects;
	}


	Level::Level(string name, lua_State* L)
	{
		this->name = name;
		gameObjects = new LinkedList<GameObject>();
		physics = new Physics(gameObjects);

		Shader* materialShader = new Shader("../Assets/Shaders/PointLight.vert", "../Assets/Shaders/PointLight.frag");
		Material* redMaterial = new Material(materialShader, NULL, Material::redPlasticMaterial);



		GameObject* camera = new GameObject("Main Camera", "camera", glm::vec3(0, 0, 450));
		// Pass near and far planes as arguments
		Camera* cameraComponent = new Camera(camera, 0.1, 10000.0);

		camera->AddComponent(cameraComponent);

		camera->AddComponent(new Script(camera, "../Assets/Scripts/CameraScript.lua", L));

		// Assign primary camera.
		Graphics::SetMainCamera(cameraComponent);



		GameObject* light = new GameObject("Light", "light", glm::vec3(0, 0, 100));
		Light* lightComponent = new Light(light, Light::duskLight);
		light->AddComponent(lightComponent);
		gameObjects->Insert(light);
		Graphics::SetLight(lightComponent);


		Shader* textureShader = new Shader("../Assets/Shaders/TexturedPointLight.vert", "../Assets/Shaders/TexturedPointLight.frag");
		Material* crateMaterial = new Material(textureShader, "../Assets/Textures/crate.bmp", Material::woodMaterial);

		Material* tableMaterial = new Material(textureShader, "../Assets/Textures/floor.bmp", Material::woodMaterial);

		Material* floorMaterial = new Material(textureShader, "../Assets/Textures/floor.bmp", Material::brassMaterial);
		Material* wallMaterial = new Material(textureShader, "../Assets/Textures/wall.bmp", Material::whiteMattMaterial);


		GameObject* box = new GameObject("box", "", glm::vec3(180.0, -90.0, -100.0), glm::vec3(0, 0, 0), glm::vec3(50, 50, 50));
		box->AddComponent(new MeshRenderer(box, new CubeMesh(), crateMaterial));
		box->AddComponent(new Script(box, "../Assets/Scripts/BoxScript.lua", L));
		box->AddComponent(new RigidBody(box, new btBoxShape(btVector3(btScalar(1.0), btScalar(1.0), btScalar(1.0))), physics->GetPhysicsWorld(), 0));
		gameObjects->Insert(box);



		//																		Position					    Scale
		GameObject* floor = new GameObject("Floor", "room", glm::vec3(0, -150, 0), glm::vec3(0, 0, 0), glm::vec3(290, 10, 200));
		//                       create a mesh render   mesh      material
		floor->AddComponent(new RigidBody(floor, new btBoxShape(btVector3(btScalar(1.0), btScalar(1.0), btScalar(1.0))),physics->GetPhysicsWorld(),0));
		floor->AddComponent(new MeshRenderer(floor, new CubeMesh(), floorMaterial));
		gameObjects->Insert(floor);

		GameObject* backWall = new GameObject("Back Wall", "room", glm::vec3(0, 0, -200), glm::vec3(0, 0, 0), glm::vec3(400, 160, 10));
		backWall->AddComponent(new MeshRenderer(backWall, new CubeMesh(), wallMaterial));
		backWall->AddComponent(new RigidBody(backWall, new btBoxShape(btVector3(btScalar(1.0), btScalar(1.0), btScalar(1.0))), physics->GetPhysicsWorld(), 0));
		gameObjects->Insert(backWall);

		GameObject* rightWall = new GameObject("Right wall", "room", glm::vec3(300, 0, 0), glm::vec3(0, 0, 0), glm::vec3(10, 160, 200));
		rightWall->AddComponent(new MeshRenderer(rightWall, new CubeMesh(), wallMaterial));
		rightWall->AddComponent(new RigidBody(rightWall, new btBoxShape(btVector3(btScalar(1.0), btScalar(1.0), btScalar(1.0))), physics->GetPhysicsWorld(), 0));
		gameObjects->Insert(rightWall);

		GameObject* leftWall = new GameObject("Left Wall", "room", glm::vec3(-300, 0, 0), glm::vec3(0, 0, 0), glm::vec3(10, 160, 200));
		leftWall->AddComponent(new MeshRenderer(leftWall, new CubeMesh(), wallMaterial));
		leftWall->AddComponent(new RigidBody(leftWall, new btBoxShape(btVector3(btScalar(1.0), btScalar(1.0), btScalar(1.0))), physics->GetPhysicsWorld(), 0));
		gameObjects->Insert(leftWall);

		GameObject* ceiling = new GameObject("Ceiling", "room", glm::vec3(0, 150, 0), glm::vec3(0, 0, 0), glm::vec3(290, 10, 200));
		ceiling->AddComponent(new MeshRenderer(ceiling, new CubeMesh(), floorMaterial));
		ceiling->AddComponent(new RigidBody(ceiling, new btBoxShape(btVector3(btScalar(1.0), btScalar(1.0), btScalar(1.0))), physics->GetPhysicsWorld(), 0));
		gameObjects->Insert(ceiling);

// the knight game object being added.
GameObject* knight = new GameObject("Knight", "ai", glm::vec3(-100, -150, 0));
// adding a mesh to the knight
knight->AddComponent(new MeshRenderer(knight, new FBXMesh("../Assets/Models/Peasant/Peasant_Man.fbx"), redMaterial));
//adding the script to the Knight.
knight->AddComponent(new Script(knight, "../Assets/Scripts/CharacterController.lua", L));
//adding the character controller component to 
knight->AddComponent(new CharacterController(knight, physics->GetPhysicsWorld(),0, 100, 50));
gameObjects->Insert(knight);


		GameObject* Table = new GameObject("Table", "", glm::vec3(-200, -150, 0), glm::vec3(-90, 0, 0));
		Table->AddComponent(new MeshRenderer(Table, new FBXMesh("../Assets/Models/Table/Table_Wood.FBX"), tableMaterial));
		gameObjects->Insert(Table);

	
		for (int i = 0; i < 25; i++) {
			GameObject* sphere = new GameObject("Sphere", "ball", glm::vec3(-250 + (i * 5), 0 , 100), glm::vec3(0, 0, 0), glm::vec3(15, 15, 15));
			sphere->AddComponent(new MeshRenderer(sphere, new SphereMesh(), redMaterial));
			sphere->AddComponent(new RigidBody(sphere, new btSphereShape(btScalar(1.0)), physics->GetPhysicsWorld(), 10));
			gameObjects->Insert(sphere);
		}

	}

	void Level::Initialise()
	{
		GameObject* obj = gameObjects->first;
		while (obj != NULL) {
			obj->Initialise();
			obj = obj->next;
		}
		Graphics::GetMainCamera()->Initialise();
	}

	string Level::GetName() {
		return name;
	}

	void Level::Update() {
		// Update the camera first
		Graphics::GetMainCamera()->gameObject->Update();
		// Update all game objects
		GameObject* obj = gameObjects->first;
		while (obj != NULL) {
			obj->Update();
			// Remove inactive GameObjects
			if (!obj->isActive()) {
				gameObjects->Delete(obj);
				GameObject* temp = obj->next;
				delete obj;
				obj = temp->next;
			}
			else {
				obj = obj->next;
			}
		}

	}
}
