#include "IFInspector.hpp"

IFInspector::IFInspector() {
	//a cluttered mess but at least it has a clear meaning 

	Component btn_track = _MakeResolveableBtn(
		"trk",
		[this](Client& c) -> bool {
			if (this)
				c.cam.tracking = this->getInspectedObject();
			else
				c.cam.tracking.reset();
			return true;
		},
		ButtonOption::Ascii());
	static Component btn_stopTrack = _MakeResolveableBtn(
		"stp-trk",
		[](Client& c) -> bool {
			c.cam.tracking.reset();
			return true;
		},
		ButtonOption::Ascii());
	Component btn_removeFromSelection = _MakeResolveableBtn(
		"x",
		[this](Client& c) -> bool {
			if (!this) return true;

			if (auto s = this->getInspectedObject().lock()) {
				for (auto i = c.selectedGOs.begin(); i != c.selectedGOs.end();)
					if (auto v = (*i).lock()) {
						if (v.get() == s.get()) {
							c.selectedGOs.erase(i);
							return true;
						}
						else
							++i;
					}else
						c.selectedGOs.erase(i);
			}

			return true;
		},
		ButtonOption::Ascii());
	Component btn_addToSelection = _MakeResolveableBtn(
		"+",
		[this](Client& c) -> bool {
			if (!this) return true;
			//add if not already in there
			if (auto s = this->getInspectedObject().lock()) {
				for (auto i = c.selectedGOs.begin(); i != c.selectedGOs.end();)
					if (auto v = (*i).lock()) {
						if (v.get() == s.get())
							return true;
						++i;
					} else
						c.selectedGOs.erase(i);
	
				c.selectedGOs.push_back(s);
			}
			return true;
		},
		ButtonOption::Ascii());
	Component btn_nextInSelection = _MakeResolveableBtn(
		">",
		[this](Client& c) -> bool {
			if (!this) return true;

			auto s = this->getInspectedObject().lock();

			for (int j = 0; j < c.selectedGOs.size(); j++) {
				int k = (j + currentSelectionIdx) % c.selectedGOs.size();
				if (auto v = c.selectedGOs[k].lock()) {

					if (!s || v.get() != s.get()) {
						this->setInspectedObject(v);
						currentSelectionIdx = k;
					}
				}
			}

			return true;
		},
		ButtonOption::Ascii());
	Component btn_formerInSelection = _MakeResolveableBtn(
		"<",
		[this](Client& c) -> bool {
			if (!this) return true;

			auto s = this->getInspectedObject().lock();

			for (int j = 0; j < c.selectedGOs.size(); j++) {
				int k = (currentSelectionIdx - j) % c.selectedGOs.size();
				if (auto v = c.selectedGOs[k].lock()) {

					if (!s || v.get() != s.get()) {
						this->setInspectedObject(v);
						currentSelectionIdx = k;
					}
				}
			}

			return true;
		},
		ButtonOption::Ascii());

	Component overviewRenderer = Renderer([&] {
		std::string name, id;
		int targType = -1;

		if (auto go = selectedObject.lock()) {
			id = std::to_string(go->id());
			name = go->getDisplayName();
			targType = static_cast<int>(go->GetClassId());
		}
		else {
			id = "N/A";
			name = "no selection";
		}

		return vbox(
			{
				text(name),
				text(id),
				text("targType:" + std::to_string(targType)),
			});
		});

	Add(Container::Vertical({
		overviewRenderer,
		Container::Horizontal({
			btn_track,
			btn_stopTrack,
			btn_removeFromSelection,
			btn_formerInSelection,
			btn_addToSelection,
			btn_nextInSelection,
		}) | bgcolor(Color::GrayLight) | color(Color::Black)
	}));
}

void IFInspector::setInspectedObject(std::weak_ptr<GameObject> go) {
	this->selectedObject = go;
}

std::weak_ptr<GameObject> IFInspector::getInspectedObject() const
{
	return selectedObject;
}
