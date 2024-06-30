#pragma once

#include <vector>
#include <set>
#include <vulkan/vulkan.hpp>

#include "../NetGameObject.hpp"

class Grid : public NetGameObject<Grid> {
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

		/*finds object using instance id, if it exists on grid*/
		std::optional<std::shared_ptr<GameObject>> getObject(Instance_Id);
		/*removes obj by id, returns the removed object if found*/
		std::optional<std::shared_ptr<GameObject>> removeObject(Instance_Id);

	private:
		std::vector<std::pair<Instance_Id, std::shared_ptr<GameObject>>> gameObjects;
};