#include <backgrounds>
#include <tonc.h>

u16 bgEnableBit(int index) {
	return BIT(8+index);
}

Background::Background(int index_, u16 priority, u16 cbb, u16 sbb, u16 bpp, u16 size) {
	index = index_;
	REG_CNT = &REG_BGCNT[index];
	*REG_CNT = BG_PRIO(priority) | BG_CBB(cbb) | BG_SBB(sbb) | bpp | size;
	BIT_SET(REG_DISPCNT, DCNT_BG0);
}

Background::~Background() {
	if (index != -1) {
		BIT_CLEAR(REG_DISPCNT, bgEnableBit(index));
	}
}

Background::Background(Background &&other) {
	*this = std::move(other);
}

Background& Background::operator=(Background &&other) {
	index = other.index;
	REG_CNT = other.REG_CNT;
	other.index = -1;
	return *this;
}

std::optional<Background> Background::create(int index, int priority, int cbb, int sbb, BgBitDepht bpp, BgSize size) {
	if (index > 3 || priority > 3 || cbb > 3 || sbb > 31) {
		return std::nullopt;
	}
	if (!BG_IS_AVAIL(index)) {
		return std::nullopt;
	}

	return Background(index, priority, cbb, sbb, (u16)bpp, (u16)size);
}

// Getters
int Background::bgIndex() const {
	return index;
}

SCREENBLOCK& Background::getSbb() const {
	return se_mem[(*REG_CNT & BG_SBB_MASK) >> BG_SBB_SHIFT];
}