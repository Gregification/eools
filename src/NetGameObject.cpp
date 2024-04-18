#include "NetGameObject.hpp"

template<class T>
const GameObjectFactory NetGameObject<T>::gof((static_cast<T*>(nullptr)));