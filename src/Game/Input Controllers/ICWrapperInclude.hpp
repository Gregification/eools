#pragma once

#include "../Navigation.hpp"
#include "../../Client.hpp"

namespace Navigation {
	/**
	* wrapper for generic IC use, solves the issue of upcasting templates
	*/
	class ICNavWrapper : public InputControl<NavBase> {
	public:
		ICNavWrapper() {
			//static_assert(std::is_base_of<NavBase, T>::value)

			onFinish = [&](InputControl_Base*, Client& c) {
				//if the IC got something useable
				if (IsSuccessful()) {
					//apply to all ships
					for (auto v : c.GetSelectedShips())
						v->navinfo.setNavPattern(GetResult());
				}
			};
		}
	};
}
