#pragma once

#include <vector>
#include <set>
#include <vulkan/vulkan.hpp>

#include "../NetGameObject.hpp"
#include "../IdGen.hpp"

class GameMap;

class Grid : public NetGameObject<Grid> {
	friend class GameMap;
	public:
		NetGameObject::NetGameObject;

		Instance_Id gridId = BAD_ID;
		Vec2 gridPos;

	public:

		void Update(float dt) override;

		void Draw(Canvas& c, Transformation_2D& trf) const override;

		void packMessage(net::message<NetMsgType>& msg) override;

		void unpackMessage(net::message<NetMsgType>& msg) override;

		void addGameObject(std::shared_ptr<GameObject>& go);

		std::shared_ptr<GameObject> getObject(Instance_Id);
		std::shared_ptr<GameObject> removeObject(Instance_Id);

	private:
		std::unordered_map<Instance_Id, std::shared_ptr<GameObject>> gameObjects;
};