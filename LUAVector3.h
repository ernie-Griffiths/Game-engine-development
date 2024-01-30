LUAVector3.h
#include "lauxlib.h"
#include "lualib.h"
#include "LuaBridge.h"

namespace Merlin {

	// Creating a class that will be specifically implimented within LUA
	// this class represents a Vector 3, as it will need to pass three sets of values within the class.
	class LUAVector3 {
	public:
		float x, y, z;
		LUAVector3() { x = 0; y = 0; x = 0; }
		LUAVector3(float x, float y, float z) :x(x), y(y), z(z) {}

		static void Register(lua_State* L) {
			using namespace luabridge;
			getGlobalNamespace(L)
				.beginNamespace("Merlin")
				.beginClass<LUAVector3>("Vector3")
				.addConstructor<void(*)(float, float, float)>()
				.endClass()
				.endNamespace();
		}
	};
}
