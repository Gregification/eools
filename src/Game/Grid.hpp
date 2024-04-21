#pragma once

#include <vector>
#include <set>
#include <vulkan/vulkan.hpp>

#include "../NetGameObject.hpp"

class GameMap;

class Grid : virtual public NetGameObject<Grid> {
	friend class GameMap;
	public:
		Grid() : gridPos(0,0) {}
		~Grid() = default;

		inst_id gridId = BAD_ID;
		Vec2 gridPos;

	public:

		void Update(float dt) override;

		void Draw(Canvas& c, Transformation_2D& trf) const override;

		void packMessage(net::message<NetMsgType>& msg) override;

		void unpackMessage(net::message<NetMsgType>& msg) override;

		virtual bool NeedNetUpdate() override;

		void addGameObject(std::shared_ptr<GameObject>& go);
		std::shared_ptr<GameObject> getObject(inst_id);
		std::shared_ptr<GameObject> removeObject(inst_id);

	private:
		std::unordered_map<inst_id, std::shared_ptr<GameObject>> gameObjects;
};