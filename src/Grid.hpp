#pragma once

#include <vector>
#include <set>

#include "GameObject.hpp"

class Grid : public GameObject {
	friend class GameMap;
	public:
		const static GameObjectFactory gof;

		Grid() : GameObject(BAD_ID), gridPos(0,0) {}
		~Grid() = default;


		id_t gridId = BAD_ID;
		Vec2 gridPos;

	public:

		void Update(float dt) override;

		void Draw(Canvas& c, Transformation_2D& transform) const override;

		void packMessage(net::message<NetMsgType>& msg) override;

		void unpackMessage(net::message<NetMsgType>& msg) override;

		virtual bool NeedNetUpdate() override;

		void addGameObject(std::shared_ptr<GameObject>& go);
		std::shared_ptr<GameObject> getObject(id_t);
		std::shared_ptr<GameObject> removeObject(id_t);

	private:
		std::unordered_map<id_t, std::shared_ptr<GameObject>> gameObjects;
};