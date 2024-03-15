#pragma once

#include <vector>
#include <set>

#include "GameStructs.hpp"
#include "GameObject.hpp"

class Grid : virtual GameObject{
	public:
		Grid() : GameObject(NAN) {}

		id_t gridId;
		std::vector<GameObject> objects;

	public:
		void Draw(Canvas& c, Dimensions zoom) const override {
			GameObject::Draw(c, zoom);
		}

		void packMessage(net::message<NetMsgType>& msg) const override {

			GameObject::packMessage(msg);
		}

		void unpackMessage(net::message<NetMsgType>& msg) override {
			GameObject::unpackMessage(msg);

		}
};