#pragma once

#include <string>
#include <optional>
#include <type_traits>

#include <ftxui/component/event.hpp>
#include <ftxui/component/component.hpp>

#include "../Camera.hpp"

class Client;

class InputControl_Base;
typedef std::shared_ptr<InputControl_Base> InputController;

/*
*	double check the naming used, its not the best
*/

/**
* used to indicate the user is preforming some sort of selective option.
*	this intercepts and process events and overlays stuff on the canvas,
*	before what ever the client processes want. these are intended to
*	only stay around as long as it takes for the user to do their thing.
* 
*	this is intended for simple operations that dont require supporting
*	ui.
* 
*	child classes can do what ever to get thie stuff done, you may see 
*	that most of them have a state machine, this is not necessary; just
*   makes for cleaner code.
* 
* e.g: selecting a rectangular area on the grid.
*	this is a complex process that has to respond to mutiple user inputs.
*	it could be done with a statemachine in the Client class but would
*	be unscaleable and messy. Instead a InputControl can be made to get
*	rectangles from the user, it can intercept mouse events to get a area
*	while also providing a visual and textual indicators of the area 
*	during selection
*/
class InputControl_Base {
public:
	std::function<void(InputControl_Base*, Client&)> onFinish;

	/**
	* account for dt during events, only occures when active
	*/
	virtual void Update(const float&);

	/**
	* draws the overlay
	*/
	virtual void Draw(Camera&, Canvas&) const;

	/** 
	* @return true if event should be presisted
	*/
	virtual bool OnEvent(Event&, Camera&);

	/**
	* description to indicate what the user is controling and their progress;
	* @return string description of current state
	*/
	const std::string& GetDescription() const { return description; }

	/**
	* @return true if the conroller is finished
	*/
	virtual bool IsDone() const;

	/**
	* @return true if the controller has finished, and done so successfuly.
	*/
	virtual bool IsSuccessful() const;

	virtual void reset();

	/** 
	* @return true if the controller is marked as finished
	*/
	bool IsFinished() { return forceDone || IsDone(); }

	void forceFinish(bool set = true) { forceDone = set; };

protected:
	bool forceDone = false;

	std::string description = "default input control description";
};

template<typename R>
class InputControl : virtual public InputControl_Base {
public:
	virtual std::unique_ptr<R> GetResult() { return std::unique_ptr<R>();};
};
