#pragma once

#include <vector>
#include <set>

#include "Game_common.hpp"
#include "GameObject.hpp"

class Grid : public GameObject {
	public:
		Grid() : GameObject(NAN), GameObjects(0), gridPos(0,0) {}
		~Grid() = default;

		const static GameObjectFactory<Grid> gof;

		id_t gridId = NAN;
		Vec2 gridPos;

		std::vector<GId_pair> GameObjects;

	public:
		//adds gameobject to grid, if assign id if not so already
		void addGameObject(GameObject& go);

	public:
		void Update(time_t dealtaTime, time_t currTime) override;

		void Draw(Canvas& c, const Vec2& offset, float scale) const override;

		void packMessage(net::message<NetMsgType>& msg) override;

		void unpackMessage(net::message<NetMsgType>& msg) override;

		virtual bool NeedNetUpdate() override;
};