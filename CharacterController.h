CharacterController.h

#include "lauxlib.h"
#include "lualib.h"
#include "glm.hpp"
#include "LUAVector3.h"

namespace Merlin {

	class CharacterController :public Component
	{
	private:


		// the height and radius values that will be used to set the capsule shape.
		int radius;
		int height;

		// Bullet classes
		btCapsuleShape* shape;
		btDefaultMotionState* motionState;
		btPairCachingGhostObject* ghost;
		btKinematicCharacterController* controller;
		btDiscreteDynamicsWorld* physicsWorld;
		btRigidBody* pBody;

		//transform position and a vector storage of the position.
		Transform* transform;
		glm::vec3 position;

		//Game object class.
		GameObject* gameObject;

	public:
		// constructor for LUA 
		CharacterController();

		//Constructor for Level.
		CharacterController(GameObject* gameObject, btDiscreteDynamicsWorld* pWorld, btScalar mass, int height, int radius, btVector3* scale = NULL, btVector3* offset = NULL);

		//deconstructor.
		~CharacterController();

		// the Walk function.
		void Walk(LUAVector3* moveDir);


		void Initialise();
		void Update();


		// the function to register the Lua functions within the Lua script
		static void Register(lua_State* L);

	};

}


