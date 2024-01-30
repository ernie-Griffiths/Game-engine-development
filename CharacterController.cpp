CharacterController.cpp
#include "CharacterController.h"
#include "Transform.h"
#include <string>
#include <map>

namespace Merlin {
	// the constructor that is being passed to lua
	CharacterController::CharacterController() {
	
	}
	
	// the constructor that is being passed to the level.
	CharacterController::CharacterController(GameObject* gameObject, btDiscreteDynamicsWorld* pWorld, btScalar mass, int height, int radius, btVector3* scale, btVector3* offset):Component(gameObject)
	{
		
		// grabbing the variables passed.
		this->height = height;
		this->radius = radius;
		this->physicsWorld = pWorld;
		this->gameObject = gameObject;

		// supports rigid transforms with only translation and rotation
		btTransform startTransform;
		startTransform.setIdentity();
	
		//getting the current transform of the gameobject
		transform = gameObject->GetTransform();


		//using variables passed creates a capsuale shape which will be used in conjunction with the 
		shape = new btCapsuleShape(radius, height);

		// creating a CachingGhostobject, this will be attached to the character controller.
		ghost = new btPairCachingGhostObject();

		if (scale == NULL) {
			// Scale the physics object collsion's shape based on the rendered object's scale
			shape->setLocalScaling(btVector3(transform->GetScale().x, transform->GetScale().y, transform->GetScale().z));
		}
		else {
			shape->setLocalScaling(*scale);
		}
	
		//setting the ghost objects transform to the current objects transform.  as this would set it to the origin of the object an offset to bring it to the middle is needed.
		ghost->setWorldTransform(btTransform(btQuaternion(0, 0, 0, 1), btVector3(position.x, (position.y + 100), position.z)));
		
		// setting the ghost objects shape to the already defined capsuale called shape
		ghost->setCollisionShape(shape);

		// instansiating the btKinematicCHaracter controller with the shape of the object as wellas the ghost object that has a colider mesh attached.
		controller = new btKinematicCharacterController(ghost, shape, 0.5);

		// adding the ghost to the physics world.
		physicsWorld->addCollisionObject(ghost);
		
		// adding the character controller to the physics world
		physicsWorld->addCharacter(controller);
	}

	// the deconstructor.
	CharacterController::~CharacterController(void) 
	{
	
	}

	void CharacterController::Initialise() 
	{
		// grabbing the current position and rotation of this object.
		position = transform->GetPosition();
		cout << " current position is " << position.x << " " << position.y << " " << position.z;
	}



	void CharacterController::Walk(LUAVector3* moveDir) {
		//changing the x,y,z values of the glm vector position to then be set as the new transform.
		position.x += moveDir->x;
		position.y += moveDir->y;
		position.z += moveDir->z;
		gameObject->GetTransform()->SetPosition(position);
	}


	void CharacterController::Update() 
	{
		// setting the Ghost Objects transform and rotation to the current rotation of the GameObject.
	ghost->setWorldTransform(btTransform(btQuaternion(transform->GetRotation().x, transform->GetRotation().y, transform->GetRotation().z, 1), btVector3(transform->GetPosition().x, (transform->GetPosition().y + 100), transform->GetPosition().z)));
	}

	void CharacterController::Register(lua_State* L) {

		// setting up the functions that will be used within the Lua Script.
		using namespace luabridge;
		getGlobalNamespace(L)
			.beginNamespace("Merlin")
			.beginClass<Component>("Component")
			.endClass()
			.deriveClass<CharacterController, Component>("CharacterController")
			.addConstructor<void(*)()>()
			.addFunction("Walk", &CharacterController::Walk)
			.endClass()
			.endNamespace();
	}

}
