#pragma once

class GOInit {
public:
	//I know this is a terrible way to do this but at this point i just want it to work
	// - see IdGen comments for the problem i just gave up on
	// - see this link for a actual solution. https://github.com/google/sanitizers/wiki/AddressSanitizerInitializationOrderFiasco
	static void init();
};
