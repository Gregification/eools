#pragma once

#include <functional>
#include <memory>
#include <vector>

#include "Game_common.hpp"
#include "GameObject.hpp"

class GameObject;

class GameObjectFactory {
	using InstanceConstructor = std::function<std::shared_ptr<GameObject>()>;

public:
	template<class T>
	GameObjectFactory(T* dummy) 
		: class_id(nextIdx++)
	{
		static_assert(std::is_base_of<GameObject, T>::value, "attempted to register non gameobject class to the game object factory");

		//this dosent actually modify the ClassList vector before runtime(even though it does execute)
		//see
		//	https://media.discordapp.net/attachments/1257155491663187978/1257157911122018384/image.png?ex=66840bca&is=6682ba4a&hm=0f81408680cfe7282083240f40eadbe6052994b19b6a01ada82ee7f92ba365c9&=&format=webp&quality=lossless
		//	https://media.discordapp.net/attachments/1257155491663187978/1257157911386525717/image.png?ex=66840bca&is=6682ba4a&hm=6ba0710cea198a0c0bb3133ed83c700f8bcfe8975c588e52b8de5e898e8d10d4&=&format=webp&quality=lossless
		/*ClassList.push_back([]() {
			return std::make_shared<T>(); 
		});*/

	}

	static Class_Id nextIdx;

	const Class_Id class_id;

	static std::shared_ptr<GameObject> getInstance(Class_Id id) {
		return ClassList[id]();
	}

	static void registerClass(Class_Id cid, InstanceConstructor ic) {
		assert(cid < nextIdx);
		if (ClassList.size() < nextIdx)
			ClassList.resize(nextIdx);

		ClassList[cid] = ic;
	}

private:
	static std::vector<InstanceConstructor> ClassList;
};
