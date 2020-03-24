#include <menu>
#include <utils>

#include <tonc.h>

#include <algorithm>

int Menu::add(const item_type &value) {
	items.push_back(value);
	return items.size() - 1;
}

int Menu::prompt(Background &bg, Background &text) {
	int maxLen = std::max_element(items.begin(), items.end(), [](const auto &a, const auto &b) {return a.size() < b.size();})->size();

	std::pair<int, int> upperLeft = {(SCR_WT - maxLen) / 2, (SCR_HT - items.size()) / 2}; // x, y

	// Draw background and frame
	int left   = upperLeft.first - 1;
	int right  = upperLeft.first + maxLen;
	int top    = upperLeft.second;
	int bottom = upperLeft.second + items.size();
	se_rect(bg.getSbb(),   left,   top,    right,   bottom,   tileSet.background);

	se_rect(text.getSbb(), left,   top-1,  right,   top,      tileSet.horzEdge);
	se_rect(text.getSbb(), left,   bottom, right,   bottom+1, set_vflip(tileSet.horzEdge, true));
	se_rect(text.getSbb(), left-1, top,    left,    bottom,   tileSet.vertEdge);
	se_rect(text.getSbb(), right,  top,    right+1, bottom,   set_hflip(tileSet.vertEdge, true));

	se_plot(text.getSbb(), left-1, top-1,  tileSet.corner);
	se_plot(text.getSbb(), right,  top-1,  set_hflip(tileSet.corner, true));
	se_plot(text.getSbb(), left-1, bottom, set_vflip(tileSet.corner, true));
	se_plot(text.getSbb(), right,  bottom, set_hflip(set_vflip(tileSet.corner, true), true));

	// Draw items
	for(size_t i = 0; i < items.size(); i++) {
		for(size_t t = 0; t < items[i].size(); t++) {
			se_plot(text.getSbb(), upperLeft.first + t, upperLeft.second + i, items[i][t]);
		}
	}

	int selected = 0;
	bool exit = false;

	while(!exit) {
		// Draw cursor
		se_plot(text.getSbb(), upperLeft.first - 1, upperLeft.second + selected, tileSet.cursor);

		key_wait_till_hit(KEY_UP | KEY_DOWN | KEY_A);
		if (key_is_down(KEY_A)) {
			exit = true;
		} else {
			se_plot(text.getSbb(), upperLeft.first - 1, upperLeft.second + selected, tileSet.background);
			selected = clamp(selected + key_tri_vert(), 0, items.size());
		}
	}

	return selected;
}

Menu& Menu::setTileSet(const MenuTileSet &value) {
	tileSet = value;
	return *this;
}