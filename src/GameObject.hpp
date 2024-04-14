#pragma once

#include <ftxui/dom/canvas.hpp>
#include <ftxui/screen/color.hpp>
#include <unordered_map>
#include <map>

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
		virtual void Update(float dt)	{}

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
		
		virtual ID getID() const;

		virtual bool NeedNetUpdate();
};

class GameObjectFactory {
	public:
		template<class T>
		GameObjectFactory(T* dummy) :
			class_id(nextIdx++)
		{
			static_assert(std::is_base_of<GameObject, T>::value, "attempted to register non gameobject class to the game object factory");

			std::function<std::shared_ptr<GameObject>()> initFunc = []() { return std::make_shared<T>(); };

			ClassList.insert(std::map<cid_t, std::function<std::shared_ptr<GameObject>()>>::value_type(
				class_id,
				initFunc
			));
		}

		static std::shared_ptr<GameObject> getInstance(cid_t id) {
			return std::move((ClassList.find(id)->second)());
		}

		const cid_t class_id;

		inline cid_t classCount() { return nextIdx; }

	private:
		static std::unordered_map<cid_t, std::function<std::shared_ptr<GameObject>()>> ClassList;
		static cid_t nextIdx;

};
