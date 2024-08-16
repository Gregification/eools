#include "GameObject.hpp"
#include "IdGen.hpp"
#include "GameObjectFactory.hpp"

void GameObject::Update(const float& dt) {

}

//adds target if it didnt exist, overwrites if the ttl is shorter
void GameObject::addSynchronizationTarget(SyncTarget st) {
	_syncCollection.insert(st);
}

const SyncCollection& GameObject::getSynchronizationTargets(){
	return _syncCollection;
}

void GameObject::clearSynchronizationTargets() {
	_syncCollection.clear();
}

void GameObject::FixedUpdate(const float& dt) {
	transform.Update(dt);
}

void GameObject::Draw(Canvas& c, const Transformation_2D& t) {
	//verticies should never be empty
	Vec2_f a = t.applyTo(
		transform.rotation.getUTD().applyTo(verticies[verticies.size() - 1])
		+ transform.position
	);

	for (auto& tb : verticies) {
		Vec2_f b = t.applyTo(transform.rotation.applyTo(tb) + transform.position);

		c.DrawBlockLine(a.x, a.y, b.x, b.y, Color::Blue);

		a = std::move(b);
	}
};

Class_Id GameObject::GetClassId() const {
	return IdGen<GameObject>::gof.class_id;
}

bool GameObject::containsPoint(const Vec2_f& p) const {
	return gs::IsPointInPoly(verticies, p, transform.rotation);
}

void GameObject::packMessage(Message& msg, MsgDiffType) {
	msg << transform;
	msg << mass;
}

void GameObject::unpackMessage(Message& msg, MsgDiffType) {
	msg >> mass;
	msg >> transform;
}

const std::string& GameObject::GetDescription() const {
	return "default gameobject description";
}

void GameObject::setId(Instance_Id newId) {
	_instId = newId;
}
