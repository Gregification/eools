#pragma once

#include <unordered_map>
#include <functional>
#include <vector>
#include <typeindex>
#include <typeinfo>
#include <iostream>
#include <memory>

#include "../NetMessageType.hpp"

//typedef int Class_Id;
//typedef int MsgDiffType;
//typedef int Message;
//class Messageable {
//	virtual void packMessage(Message&, MsgDiffType) = 0;
//	virtual void unpackMessage(Message&, MsgDiffType) = 0;
//};

#define _FACTORY_BAD_CLASS_ID -1;


/** 
* NOTE: developed using MSVC/14.39 . please check that it compiles first
* 
* PURPOSE
* a simple way to handle relfective instance creation and calling class specific
*	implimentations of a function without explicitly knowing the specific class
*	at compile time. (is able to call implimentations of intermediate classes).
* 
* HOW IT WORKS & CONTEXT
* this was made as a way to synch object instances across a network; where given 
*	some packet and knowing that the packet describes some derived class of a 
*	base(known at compile time) the Factory namespace can then be used to either
*	create a instance of the object described or call the pack/unpack method 
*	specific to the instances' class without knowing the specific class at 
*	compile time. this is done buy assigning each derived class a unique id that
*	can be a part of the data packet, this id can then be used to reconstruct
*	the class on the other end.
* this uses a gimic with templated constructors to assign unique numbers instead
*	of type_info because this generates the id's during dynamic initialization 
*	and not runtime. this removes the human responsibility to initialize things 
*	in a predefined order, it just happens automatically. 
* 
* STUFF TO KNOW
*	- all instances and the base class must have a default constructor.
*	- derived class are identified by a automatically generated `Class_Id` that 
*		is unique to the base class.`
*	- the unique `Class_Id` relies on a feature I know to be part of the 
*		MSVC/14.39 compiler, but not some others. (see `InstFactory` constructor)
*	- if getting vector access errors on run may be due to a gimic some compilers
*		that require the constructor of `FactoryInstable<T>` be reachable.
*		more of a me issue since i cant figure out the feature responsible for
*		that behavior.
* 
* HOW TO USE
*	must have a base class that impliments Messageable. the factory will return 
*	this class when dealing with derived classes.
*	e.g setup of 3 instances (Derived A, B, and C):
* 
*		struct Base : 
*			public Messageable,
*			public Factory::FactoryInstable<Base, Base>
*		{ ... }
*		struct DerivedA : 
*			public Base,
*			public Factory::FactoryInstable<Base, DerivedA>
*		{ ... }
*		struct DerivedB :
*			public Base,
*			public Factory::FactoryInstable<Base, DerivedB>
*		{ ... }
*		struct DerivedC :
*			public DerivedB,
*			public Factory::FactoryInstable<Base, DerivedC>
*		{ ... }
*	
* 
*	e.g making a instance and calling a specific implimentation of Messageable:
* 
*		//we want to send a message for some base class(known type)
*		// in this case were packing a instance of DerivedA 
*		//note that the varaible types are upto you to setup
*		MsgDiff msg_diff = _ALL;
*		Message msg;
*		DerivedA* obj = new DerivedA; 
*		Class_Id cid_of_obj = Base::factory.GetClassId(obj);
*		Factory::InstFactory<Base>::PackAs(cid_of_obj, msg_diff, msg, obj);
*
*		//dont forget to put the `Class_Id` in the packet!
*		msg.addCId(obj.GetClassID());	//demo function
*		
*		//now on the the remote programs end
*		//to get the object back
*		Message msg = NextMessage();		//demo function
*		Class_Id id = msg.getCId();			// ^
*		MsgDiff msg_diff = msg.getDiff();	// ^
*		
*		//make instance from the cid
*		std::shared_ptr<Base> ptr = Base::factory.GetInstance(id);
*		
*		//apply message to corrosponding function
*		Factory::InstFactory<Base>::UnpackAs(id, msg_diff, msg, ptr.get());
*		
*		//local obj is now up to date with what ever was in the message
*/
namespace Factory {
	template<typename BASE, typename DERIVED>
	class FactoryInstableHelper;

	template<typename BASE>
	class InstFactory;

	/**
	* adds unique item - derived form <BASE> - to factory of <BASE>s
	*/
	template<typename BASE, typename DERIVED>
	class FactoryInstable {
	public:
		const static InstFactory<BASE> factory;
		const static FactoryInstableHelper<BASE, DERIVED> _dummy;

		FactoryInstable() {
			static_assert(std::is_base_of_v<BASE, DERIVED>);

			factory;
			_dummy;
		}
	};

	/**
	* makes unique factory and promises packing/unpacking to <BASE>
	*/
	template<typename BASE>
	class InstFactory {
	public:
		typedef std::shared_ptr<BASE>
			BasePtr;
		typedef std::function<std::unique_ptr<BASE>()>
			InstanceConstructor;
		typedef std::function<void(Message&, MsgDiffType, BASE*)>
			Pakistan;//packs and unpakcs stuff

		const Class_Id class_id;
		static Class_Id next_class_id;

		template<typename DERIVED>
		InstFactory(DERIVED* dummy) : class_id(next_class_id++) {
			static_assert(std::is_base_of_v<Messageable, BASE>, "base must be messageable");
			static_assert(std::is_base_of_v<BASE, DERIVED>, "attempted to register non derived class");

			/**
			everythign after this comment is necessary junk.
			explanation: refrences to the packer/unpacker vectors are here since
				one of the processes that handles static initilization before
				runtime does weird stuff leading to the vectors being being empty
				unless there is a reachable non static refrence to them at runtime.
				idk with certainty but suspect its some compiler mumbo jumbo.
			*/
			Factory::FactoryInstable<BASE, DERIVED>::_dummy;
			Factory::InstFactory<BASE>::_instConsts;
			Factory::InstFactory<BASE>::_packers;
			Factory::InstFactory<BASE>::_unpackers;
			Factory::InstFactory<BASE>::typeToIdMap;
		}

		static void Register_Class(
			Class_Id,
			InstanceConstructor,
			Pakistan packer,
			Pakistan unpacker,
			std::type_index
		);
		static std::unique_ptr<BASE> GetInstance(Class_Id);
		static void UnpackAs(Class_Id, Message&, BASE*, MsgDiffType = DEFAULT_MsgDiff_EVERYTHING);
		static void PackAs(Class_Id, Message&, BASE*, MsgDiffType = DEFAULT_MsgDiff_EVERYTHING);
		static Class_Id GetClassId(const std::type_info& info) {
			auto it = typeToIdMap.find(std::type_index{ info });

			if (it == typeToIdMap.end())
				return BAD_CLASS_ID;

			return it->second;
		}
		template<typename T>
		static Class_Id GetClassId(T* t) {
			static_assert(std::is_base_of_v<BASE, T>);
			if(t)
				return GetClassId(typeid(*t));

			return BAD_CLASS_ID;
		}
		template<typename T>
		static Class_Id GetClassId() {
			static_assert(std::is_base_of_v<BASE, T>);

			return GetClassId(typeid(T));
		}

	private:
		static inline std::unordered_map<std::type_index, Class_Id> typeToIdMap{};
		static std::vector<InstanceConstructor> _instConsts;
		static std::vector<Pakistan> _packers;
		static std::vector<Pakistan> _unpackers;
	};

	template<typename BASE, typename DERIVED>
	class FactoryInstableHelper {
	public:
		FactoryInstableHelper() {
			InstFactory<BASE>::Register_Class(
				FactoryInstable<BASE, DERIVED>::factory.class_id,
				[] { return std::make_unique<DERIVED>(); }, //instance constructor
				[](Message& msg, MsgDiffType diff, BASE* go) { //packer
					static_cast<DERIVED*>(go)->DERIVED::packMessage(msg, diff);
				},
				[](Message& msg, MsgDiffType diff, BASE* go) { //unpacker
					static_cast<DERIVED*>(go)->DERIVED::unpackMessage(msg, diff);
				},
				std::type_index{ typeid(DERIVED) }
			);
		}
	};

	template<typename BASE, typename DERIVED>
	const InstFactory<BASE> FactoryInstable<BASE, DERIVED>::factory{ static_cast<DERIVED*>(nullptr) };
	template<typename BASE, typename DERIVED>
	const FactoryInstableHelper<BASE, DERIVED> FactoryInstable<BASE, DERIVED>::_dummy{};

	template<typename BASE> Class_Id InstFactory<BASE>::next_class_id = 0;
	template<typename BASE> std::vector<std::function<std::unique_ptr<BASE>()>>
	InstFactory<BASE>::_instConsts{};
	template<typename BASE> std::vector<std::function<void(Message&, MsgDiffType, BASE*)>>
	InstFactory<BASE>::_packers{};
	template<typename BASE> std::vector<std::function<void(Message&, MsgDiffType, BASE*)>>
	InstFactory<BASE>::_unpackers{};

	template<typename BASE> void InstFactory<BASE>::Register_Class(
		Class_Id cid,
		InstanceConstructor ic,
		Pakistan packer,
		Pakistan unpacker,
		std::type_index typeIdx
	) {
		if (_instConsts.size() < next_class_id) {
			_instConsts.resize(next_class_id);
			_packers.resize(next_class_id);
			_unpackers.resize(next_class_id);
		}

		_instConsts[cid] = ic;
		_packers[cid] = packer;
		_unpackers[cid] = unpacker;
		typeToIdMap[typeIdx] = cid;
	}
	template<typename BASE> std::unique_ptr<BASE> InstFactory<BASE>::GetInstance(Class_Id cid) {
		return _instConsts[cid]();
	}
	template<typename BASE> void InstFactory<BASE>::PackAs(Class_Id cid, Message& msg, BASE* b, MsgDiffType mdt) {
		_packers[cid](msg, mdt, b);
	}
	template<typename BASE> void InstFactory<BASE>::UnpackAs(Class_Id cid, Message& msg, BASE* b, MsgDiffType mdt) {
		_unpackers[cid](msg, mdt, b);
	}

};