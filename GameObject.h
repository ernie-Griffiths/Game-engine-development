GameObject.h
#pragma once
#include "Object.h"
#include <string>
#include "LinkedList.h"
#include "Component.h"
#include "Transform.h"
#include "glew.h"
#include "glut.h"
#include "lua.hpp"
#include "luabridge.h"
#include "RigidBody.h"
#include "Script.h"
#include <CharacterController.h>

using namespace std;


namespace Merlin {

	class CharacterController;

	class GameObject :public Object
	{
	private:
		string name;
		string tag;
		bool active;
		Transform* transform;
		RigidBody* rigidBody;

		// A character controller class was added to the game object.
		CharacterController* characterController;

		Script* script;

	public:
		GameObject* next;
	public:
		GameObject(string name);
		GameObject(string name, string tag, glm::vec3 position = glm::vec3(0, 0, 0), glm::vec3 rotation = glm::vec3(0, 0, 0), glm::vec3  scale = glm::vec3(1, 1, 1));
		~GameObject();
		void Initialise();
		void Update();
		string GetName() { return name;}
		string GetTag() { return tag; }
		void SetTag(string tag) { this->tag = tag; }
		void Destroy() { active = false; }
		bool isActive() { return active; }

		LinkedList<Component>* components;

		void AddComponent(Component* component);

		template<typename T> Component* GetComponent();
		Transform* GetTransform() { return transform; }
		RigidBody* GetRigidBody() { return rigidBody; }


// this function allowed the Get Character controller to be passed where it is needed.
		CharacterController* GetCharacterController() { return characterController; }

		Script* GetScript() { return script; }

		static void Register(lua_State* L);


	};

}

