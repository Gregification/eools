#include "IdGen.hpp"

/*
* im going to lose my mind.
* defining gof in the header file & cpp file causes a redefinition error (expected)
* but if you remove the definition in the header and define it in the cpp file (this file)
* it says a definiton cant be found????
*  seems to only check the cpp file if all is defined???
*/
//template<class T> const GameObjectFactory IdGen<T>::gof = GameObjectFactory(static_cast<T*>(nullptr));
