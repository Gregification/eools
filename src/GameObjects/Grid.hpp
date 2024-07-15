#pragma once

#include <vector>
#include <set>

#include "../NetGameObject.hpp"

#ifdef DrawText
#undef DrawText
#endif

class Grid : public NetGameObject<Grid> {
	friend class Camera;

	public:
		NetGameObject::NetGameObject;

		Vec2 gridPos;

		struct GOObj {
			Instance_Id id;
			GameObjPtr go;
		};

		void Draw(Canvas& c, Transformation_2D& trf) const override;

		void packMessage(Message&, MsgDiffType = 0) override;
		void unpackMessage(Message&, MsgDiffType = 0) override;

		void Update(float) override {};
		void FixedUpdate(float) override {};

		/*adds a object by id, overwrites existing if it exists. returns true of overwritten*/
		void AddObject(GameObjPtr);
		/*removes obj by id, returns true if found*/
		bool RemoveObject(Instance_Id);
		/*finds object using instance id, if it exists on grid*/
		std::optional<GameObjPtr> FindObject(Instance_Id);
		/*clears objs*/
		void RemoveAllObjects();

		const std::vector<GOObj> getObjVec() const;

	private:
		using GameObject::Update;
		using GameObject::FixedUpdate;

		//hybrid approach, objs are stored in both a map and vector
		//map : need fast look up capabilities for processing network packets
		//vector: need fast itterations for rendering & game processes
		// - yes it'll be massive but hardware skill issue or something
		std::vector<GOObj> _go_vec;
		std::unordered_map<Instance_Id, GameObjPtr> _go_map;
};