#include "GameObject.hpp"
#include "IdGen.hpp"
#include "GameObjectFactory.hpp"

void GameObject::Update(float dt) {

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


void GameObject::FixedUpdate(float dt) {
	static const SyncTarget syncTarg{ 
		.class_id = IdGen<GameObject>::gof.class_id, 
		.diff = DEFAULT_MsgDiff_EVERYTHING};
	addSynchronizationTarget(syncTarg);

	transform.Update(dt);
}

void GameObject::Draw(Canvas& c, Transformation_2D t) const {
	t.mat = t.mul(transform.getRotationTransformation());

	Vec2_f a = t.applyTo(verticies[verticies.size() - 1] + transform.position);
	for (auto& tb : verticies) {
		Vec2_f b = t.applyTo(tb + transform.position);

		c.DrawBlockLine(a.x, a.y, b.x, b.y, Color::Blue);

		a = b;
	}
};

Class_Id GameObject::GetClassId() const {
	return IdGen<GameObject>::gof.class_id;
}

bool GameObject::ContainsPoint(const Vec2_f& p) const {
	return gs::IsPointInPoly(verticies, p - transform.position, transform.getRotationTransformation());
}

void GameObject::packMessage(Message& msg, MsgDiffType) {
	msg << transform;
	msg << mass;
}

void GameObject::unpackMessage(Message& msg, MsgDiffType) {
	msg >> mass;
	msg >> transform;
}


std::string GameObject::GetDescription() const {
	return "default gameobject description";
}

void GameObject::setId(Instance_Id newId) {
	_instId = newId;
}
