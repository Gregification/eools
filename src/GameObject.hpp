#pragma once

#include <ftxui/dom/canvas.hpp>
#include <ftxui/screen/color.hpp>

#include "NetMessageType.hpp"
#include "GameStructs.hpp"
#include "Game_common.hpp"
#include "Body.hpp"

using namespace gs;
using namespace ftxui;

class GameObject : virtual public Body {
	public:
		GameObject(id_t id) :
			Body(),
			id(id),
			needNetUpdate(false)
		{
			
		}
		virtual ~GameObject() = default;

	public:
		id_t id;
		bool needNetUpdate;

	public:
		virtual void Update(float dt) {

		}

		virtual void PhysUpdate(float dt) {
			transform.PhysUpdate(dt);
		}

		virtual void Draw(Canvas& c, Transformation_2D& transform) const;
		
		//PACK CURRENT CLASS FIRST, SUPER CALSS LAST
		virtual void packMessage(net::message<NetMsgType>& msg) { 
			msg << transform;
		}

		//UNPACK SUPER CLASS FIRST
		virtual void unpackMessage(net::message<NetMsgType>& msg) {
			msg >> transform;
		}

		virtual std::string GetDescription() const;

		virtual bool NeedNetUpdate();
};

class GameObjectFactory {
	public:
		template<class T>
		GameObjectFactory(T* dummy) :
			index(nextIdx++)
		{
			static_assert(std::is_base_of<GameObject, T>::value, "attempted to register non gameobject class to the game object factory");

			ClassList.push_back([]() {
					return std::make_shared<T>();
				});
		}

		static std::shared_ptr<GameObject> getInstance(size_t id) {
			return std::move(ClassList[id]());
		}

	public:
		static std::vector<std::function<std::shared_ptr<GameObject>()>> ClassList;
		static size_t nextIdx;

		const size_t index;
};
