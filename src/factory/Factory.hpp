#pragma once

#include <iostream>
#include <cassert>

typedef int Class_Id;
typedef int MsgDiffType;
typedef int Message;

#define _FACTORY_BAD_CLASS_ID -1;

class Messageable {
	virtual void packMessage(Message&, MsgDiffType) = 0;
	virtual void unpackMessage(Message&, MsgDiffType) = 0;
};

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
*
* STUFF TO KNOW
*	- all instances and the base class must have a default constructor.
*	- derived class are identified by a `Class_Id` that is unique to the base 
*		class.
*	- the unique `Class_Id` relies on a feature I know to be part of the 
*		MSVC/14.39 compiler, but not some others.
*	- if getting vector access errors on run may be due to a gimic some compilers
*		that require the constructor of `FactoryInstable<T>` be reachable.
*		more of a me issue since i cant figure out the feature responsible for
*		that behavior.
*	- it gets tricky to handle the diamond problem regardign static funcitons
*		but that can be fixed in design, theres nothign wrong with Factory.
* 
* HOW TO USE
*	must have a base class that impliments Messageable. the factory will return 
*	this class when dealing with derived classes.
*	e.g setup of 3 instances (Derived A, B, and C):
*		struct Base : 
*			public Messageable
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
*			//ambigous conflict when calling GetClassID() for DerivedC
*			//instead use the expanded version
*			//	`Factory::FactoryInstable<Base, DerivedC>::GetClassID()`
* 
*		//now when we want to send a message for some instance(known type)
*		// in this case were packing a instance of DerivedA 
*		//note that the varaible types are upto you to setup
*		MsgDiff msg_diff = _ALL;
*		Message msg;
*		DerivedA* obj = new DerivedA; 
*		Factory::InstFactory<Base>::PackAs(obj.GetClassID(), msg_diff, msg, obj);
*
*		//dont forget to put the `Class_Id` in the packet!
*		msg.addCId(obj.GetClassID());
*		
*		//now on the the remote programs end
*		//to get the object back
*		Message msg = NextMessage();		//pretend this exists
*		Class_Id id = msg.getCId();			// ^
*		MsgDiff msg_diff = msg.getDiff();	// ^
*		
*		//make instance if needed
*		std::shared_ptr<Base> ptr = Factory::InstFactory<BaseA>::GetInstance(id);
*		
*		//apply to instance
*		Factory::InstFactory<Base>::UnpackAs(id, msg_diff, msg, ptr.get());
*		
*		//local obj is now up to date with what ever was in the message
*/
namespace Factory {
	template<typename BASE, typename LEAF>
	class FactoryInstableHelper;

	template<typename BASE>
	class InstFactory;

	/**
	* adds unique item - derived form <BASE> - to factory of <BASE>s
	*/
	template<typename BASE, typename LEAF>
	class FactoryInstable {
	public:
		const static InstFactory<BASE> factory;
		const static FactoryInstableHelper<BASE, LEAF> _dummy;

		FactoryInstable() {
			static_assert(std::is_base_of_v<BASE, LEAF>);
		}

		static Class_Id GetClassID() { 
			return factory.class_id;
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
		typedef std::function<BasePtr()>
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

				tried: moved refrences to a private function
				result: needs a bit of patch work to get a pointer to _dummy but 
					it dosent work anyways.
				takeaway: needs to be somewhere thats reachable but not static

				tried: moved to namepsace
				result: works
				takeaway: works BUT would then require a manual instantation in 
					the namespace, a macro can be used but then it gets messy.

				with it as is, the compiler likely just removes it (unconfirmed).
				 side effect for having this in that this constructor (ANY 
				 version of it) must be reachable in the program.
			*/
			Factory::FactoryInstable<BASE, DERIVED>::_dummy;
			Factory::InstFactory<BASE>::_instConsts;
			Factory::InstFactory<BASE>::_packers;
			Factory::InstFactory<BASE>::_unpackers;
		}

		static void Register_Class(
			Class_Id,
			InstanceConstructor,
			Pakistan packer,
			Pakistan unpacker
		);
		static BasePtr GetInstance(Class_Id);
		static void UnpackAs(Class_Id, MsgDiffType, Message&, BASE*);
		static void PackAs(Class_Id, MsgDiffType, Message&, BASE*);

	private:
		static std::vector<InstanceConstructor> _instConsts;
		static std::vector<Pakistan> _packers;
		static std::vector<Pakistan> _unpackers;
	};

	template<typename BASE, typename LEAF>
	class FactoryInstableHelper {
	public:
		FactoryInstableHelper() {
			InstFactory<BASE>::Register_Class(
				FactoryInstable<BASE, LEAF>::factory.class_id,
				[] { return std::make_shared<LEAF>(); }, //instance constructor
				[](Message& msg, MsgDiffType diff, BASE* go) { //packer
					((LEAF*)go)->LEAF::packMessage(msg, diff);
				},
				[](Message& msg, MsgDiffType diff, BASE* go) { //unpacker
					((LEAF*)go)->LEAF::unpackMessage(msg, diff);
				}
			);
		}
	};

	template<typename BASE, typename LEAF>
	const InstFactory<BASE> FactoryInstable<BASE, LEAF>::factory{ static_cast<LEAF*>(nullptr) };
	template<typename BASE, typename LEAF>
	const FactoryInstableHelper<BASE, LEAF> FactoryInstable<BASE, LEAF>::_dummy{};

	template<typename BASE> Class_Id InstFactory<BASE>::next_class_id = 0;
	template<typename BASE> std::vector<std::function<std::shared_ptr<BASE>()>>
	InstFactory<BASE>::_instConsts{};
	template<typename BASE> std::vector<std::function<void(Message&, MsgDiffType, BASE*)>>
	InstFactory<BASE>::_packers{};
	template<typename BASE> std::vector<std::function<void(Message&, MsgDiffType, BASE*)>>
	InstFactory<BASE>::_unpackers{};

	template<typename BASE> void InstFactory<BASE>::Register_Class(
		Class_Id cid,
		InstanceConstructor ic,
		Pakistan packer,
		Pakistan unpacker
	) {
		auto nxt_cid = next_class_id;
		auto _instcon_size = _instConsts.size();
		auto _packer_size = _packers.size();
		auto _unpacker_size = _unpackers.size();

		if (_instConsts.size() < next_class_id) {
			_instConsts.resize(next_class_id);
			_packers.resize(next_class_id);
			_unpackers.resize(next_class_id);
		}

		_instConsts[cid] = ic;
		_packers[cid] = packer;
		_unpackers[cid] = unpacker;

	}
	template<typename BASE> InstFactory<BASE>::BasePtr InstFactory<BASE>::GetInstance(Class_Id cid) {
		auto size = _instConsts.size();
		auto nxt_class_id = next_class_id;
		return _instConsts[cid]();
	}
	template<typename BASE> void InstFactory<BASE>::PackAs(Class_Id cid, MsgDiffType mdt, Message& msg, BASE* b) {
		_packers[cid](msg, mdt, b);
	}
	template<typename BASE> void InstFactory<BASE>::UnpackAs(Class_Id cid, MsgDiffType mdt, Message& msg, BASE* b) {
		_unpackers[cid](msg, mdt, b);
	}
};