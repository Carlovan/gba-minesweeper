#include <menu>

#include <algorithm>

int Menu::add(const item_type &value) {
	items.push_back(value);
	return items.size() - 1;
}

int Menu::prompt(Background &bg) {
	se_fill(bg.getSbb(), tileSet.background);

	int maxLen = std::max_element(items.begin(), items.end(), [](const auto &a, const auto &b) {return a.size() < b.size();})->size();

	std::pair<int, int> upperLeft = {(SCR_WT - maxLen) / 2, (SCR_HT - items.size()) / 2}; // x, y

	// Draw items
	for(size_t i = 0; i < items.size(); i++) {
		for(size_t t = 0; t < items[i].size(); t++) {
			se_plot(bg.getSbb(), upperLeft.first + t, upperLeft.second + i, items[i][t]);
		}
	}

	int selected = 0;
	bool exit = false;

	while(!exit) {
		// Draw cursor
		se_plot(bg.getSbb(), upperLeft.first - 1, upperLeft.second + selected, tileSet.cursor);

		key_wait_till_hit(KEY_UP | KEY_DOWN | KEY_A);
		if (key_is_down(KEY_A)) {
			exit = true;
		} else {
			se_plot(bg.getSbb(), upperLeft.first - 1, upperLeft.second + selected, tileSet.background);
			selected = clamp(selected + key_tri_vert(), 0, items.size());
		}
	}

	return selected;
}

Menu& Menu::setTileSet(const MenuTileSet &value) {
	tileSet = value;
	return *this;
}