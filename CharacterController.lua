CharacterController.lua

require("class")
ScriptComponent = class()

--this update script will be constantly running when an object with this script is in the scene.
function ScriptComponent:Update(this)
-- using the merlin InputManager whenever a key is pressed that corresponds to a function a LUAVector3 class object
-- is passed to the Move function of character controller.
	if Merlin.InputManager.GetKey("WalkCharacterForward") then
	this:GetCharacterController():Walk(Merlin.Vector3(0,0,1))
	end
	if Merlin.InputManager.GetKey("WalkCharacterLeft") then
	this:GetCharacterController():Walk(Merlin.Vector3(-1,0,0))
	end
		if Merlin.InputManager.GetKey("WalkCharacterRight") then
	this:GetCharacterController():Walk(Merlin.Vector3(1,0,0))
	end
		if Merlin.InputManager.GetKey("WalkCharacterBackwards") then
	this:GetCharacterController():Walk(Merlin.Vector3(0,0,-1))
	end

-- as there is already a rotate gameobject script with this was utalised to allow the knight object rotate when it
-- has this script attached. using the same GetKey method that is used above.
		if Merlin.InputManager.GetKey("TurnCharacterRight") then
	this:GetTransform():RotateAboutY(0.1)
	end
		if Merlin.InputManager.GetKey("TurnCharacterLeft") then
	this:GetTransform():RotateAboutY(-0.1)
	end
end

InputManager.h

#pragma once
#include "glfw3.h"
#include "lua.hpp"
#include "luabridge.h"
#include <string>
#include <map>
#include "glm.hpp"

using namespace std;


namespace Merlin {

	struct Position {
		double x, y;
	};

	class InputManager
	{
	private:
		static GLFWwindow* window;
		static map < string, int > keyMappings;
		static Position currentCursor;
		static Position previousCursor;

		static const int MERLIN_MOUSE_MOVE_X = 10001;

	public:
		static void Initialise(GLFWwindow* win) {
			window = win;
			keyMappings["CameraZoomIn"] = GLFW_KEY_Z;
			keyMappings["CameraZoomOut"] = GLFW_KEY_X;
			keyMappings["CameraYawLeft"] = GLFW_KEY_LEFT;
			keyMappings["CameraYawRight"] = GLFW_KEY_RIGHT;
			keyMappings["CameraPitchUp"] = GLFW_KEY_UP;
			keyMappings["CameraPitchDown"] = GLFW_KEY_DOWN;
			keyMappings["Xaxis"] = MERLIN_MOUSE_MOVE_X;

			// the Key mappings for walking and turning the character.
			keyMappings["WalkCharacterForward"] = GLFW_KEY_W;
			keyMappings["WalkCharacterLeft"] = GLFW_KEY_A;
			keyMappings["WalkCharacterRight"] = GLFW_KEY_D;
			keyMappings["WalkCharacterBackwards"] = GLFW_KEY_S;
			keyMappings["TurnCharacterRight"] = GLFW_KEY_E;
			keyMappings["TurnCharacterLeft"] = GLFW_KEY_Q;

		}

		static bool GetKey(string key) {
			if (glfwGetKey(window, keyMappings[key]) == GLFW_PRESS) {
				return true;
			}
			return false;
		}

		static float GetAxis(string key) {
			if (keyMappings[key] == MERLIN_MOUSE_MOVE_X) {
				glfwGetCursorPos(window, &currentCursor.x, &currentCursor.y);
				return glm::clamp(float(currentCursor.x - previousCursor.x), -1.0f, 1.0f);
			}
			else return 0.0f;
		}

		static void RecordLastCursorPosition() {
			glfwGetCursorPos(window, &previousCursor.x, &previousCursor.y);
		}

		static void Register(lua_State* L) {
			using namespace luabridge;
			getGlobalNamespace(L)
				.beginNamespace("Merlin")
				.beginClass<InputManager>("InputManager")
				.addStaticFunction("GetKey", &InputManager::GetKey)
				.addStaticFunction("GetAxis", &InputManager::GetAxis)
				.endClass()
				.endNamespace();
		}
	};
}
