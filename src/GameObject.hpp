#pragma once

#include <ftxui/dom/canvas.hpp>
#include <ftxui/screen/color.hpp>

#include "NetMessageType.hpp"
#include "GameStructs.hpp"
#include "Game_common.hpp"
#include "Body.hpp"

using namespace gs;
using namespace ftxui;

class GameObject : public Body {
	public:
		GameObject(id_t id) :
				Body(),
				id(id),
				needNetUpdate(false)
			{};
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

		virtual void Draw(Canvas& c, const Vec2& offset, float scale) const;
		
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

typedef struct GId_pair : std::pair<id_t, std::shared_ptr<GameObject>> {
	static id_t & util_packArray(GId_pair& obj) {
		return obj.first;
	}

	static GId_pair util_unpackArray(net::message<NetMsgType>& msg) {
		GId_pair ret = {};
		msg >> ret.first;
		ret.second = NULL;

		return ret;
	}
	
	friend bool operator == (const GId_pair& a, const GId_pair& b) {
		return a.first == b.first && a.first != NAN;
	}
} GId_pair;

template<typename T>
class GameObjectFactory {
	public:
		GameObjectFactory() {
			static_assert(std::is_base_of<GameObject, T>::value, "attempted to register non gameobject class to game object factory");

			ClassList.push_back([]() {
					return std::make_unique<T>();
				});
			index = nextIdx++;
		}

		static std::unique_ptr<GameObject> createObject(size_t id) {
			assert(id < ClassList.size());

			return std::move(ClassList[id]());
		}

	public:
		static std::vector<std::function<std::unique_ptr<GameObject>()>> ClassList;
		static std::atomic<size_t> nextIdx;

		size_t index;
};

template<typename T>
std::vector<std::function<std::unique_ptr<GameObject>()>> GameObjectFactory<T>::ClassList;
template<typename T>
std::atomic<size_t> GameObjectFactory<T>::nextIdx(0);