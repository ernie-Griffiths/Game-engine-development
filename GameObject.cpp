GameObject.cpp
#include "GameObject.h"


namespace  Merlin {


	GameObject::GameObject(string name, string tag, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale) {
		cout << "Loading Game Object " << name << "into level. " << endl;
		this->active = true;
		this->name = name;
		this->tag = tag;

		rigidBody = 0;

		components = new LinkedList<Component>();
		transform = new Transform(this, position, rotation, scale);
		transform->gameObject = this;
		components->Insert((Component*)transform);

	}

	GameObject::GameObject(string name) : name(name) 
	{

		this->active = true;
		this->tag = "";

		components = new LinkedList<Component>();

	}

	GameObject::~GameObject() 
	{
		delete components;
	}
	
	void GameObject::Initialise() {
		for (int n = 0; n < components->Count(); n++) {
			components->Get(n)->Initialise();
		}
	}

	void GameObject::Update() {
		for (int n = 0; n < components->Count(); n++) {
			components->Get(n)->Update();
		}
	}

	void GameObject::AddComponent(Component* component) {
		component->gameObject = this;
		if (typeid(RigidBody) == typeid(*component)) rigidBody = (RigidBody*)component;
		// when adding components to a particular game object this was added to allow the function to successfully add a Character controller to the object
		// in this case the knight.
		if (typeid(CharacterController) == typeid(*component)) characterController = (CharacterController*)component;

		if (typeid(Script) == typeid(*component)) script = (Script*)component;

		components->Insert(component);
	}


	template<typename T>
	Component* GameObject::GetComponent() {
		for (int n = 0; n < components->Count(); n++) {
			Component* component = components->Get(n);
			if (typeid(T) == typeid(*component)) return component;
		}
		return NULL;
	}

	void GameObject::Register(lua_State* L) {
		using namespace luabridge;
		getGlobalNamespace(L)
			.beginNamespace("Merlin")
			.beginClass<Object>("Object")
			.addFunction("GetID", &Object::GetID)
			.endClass()
			.deriveClass<GameObject, Object>("GameObject")
			.addFunction("GetName", &GameObject::GetName)
			.addFunction("GetTag", &GameObject::GetTag)
			.addFunction("Destroy", &GameObject::Destroy)
			.addFunction("GetTransform", &GameObject::GetTransform)

	    	.addFunction("GetCharacterController", &GameObject::GetCharacterController)

			.endClass()
			.endNamespace();
	}


}

