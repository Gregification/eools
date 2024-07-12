#include "IFHelm.hpp"

IFHelm::IFHelm() {
	auto shipView = Renderer([&] {
		return canvas([&](Canvas& c) {
			
			});
		});
	
}

void IFHelm::Of(std::shared_ptr<Ship> sp) {
	of = sp;
}