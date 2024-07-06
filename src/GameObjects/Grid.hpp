#pragma once

#include <vector>
#include <set>

#include "../NetGameObject.hpp"

class Grid : public NetGameObject<Grid> {
	public:
		NetGameObject::NetGameObject;

		time_t lastUpdate, lastUpdate_fixed;

		Vec2 gridPos;

		struct GOObj {
			Instance_Id id;
			GameObjPtr go;
		};

		void Update(float dt) override;

		void Draw(Canvas& c, Transformation_2D& trf) const override;

		void packMessage(Message& msg) override;
		void unpackMessage(Message& msg) override;

		void RemoveAllObjects();
		
		/*adds a object by id, overwrites existing if it exists. returns true of overwritten*/
		void AddObject(GameObjPtr);

		/*finds object using instance id, if it exists on grid*/
		std::optional<GameObjPtr> FindObject(Instance_Id);

		/*removes obj by id, returns true if found*/
		bool RemoveObject(Instance_Id);

	private:
		//hybrid approach, objs are stored in both a map and vector
		//map : need fast look up capabilities for processing network packets
		//vector: need fast itterations for rendering & game processes
		// - yes it'll be massive but hardware skill issue or something
		std::vector<GOObj> _go_vec;
		std::unordered_map<Instance_Id, GameObjPtr> _go_map;
};