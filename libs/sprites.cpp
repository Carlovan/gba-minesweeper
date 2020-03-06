#include <sprites>

void initialize_sprites() {
	BIT_SET(REG_DISPCNT, DCNT_OBJ | DCNT_OBJ_1D);
	oam_init(obj_mem, SPRITES_COUNT);
}

std::vector<bool> used(SPRITES_COUNT, 0);

OBJ_ATTR& Sprite::get_object_data() {
	return obj_mem[index];
}

OBJ_ATTR Sprite::get_object_data() const {
	return obj_mem[index];
}

Sprite::Sprite(int index_, u16 priority_, u16 bpp_, u16 shape_, u16 size_, u16 tileIndex_) {
	index = index_;
	auto& objData = get_object_data();
	objData.attr0 = ATTR0_BUILD(0, shape_, static_cast<u16>(bpp_), 0, 0, 0, 0);
	objData.attr1 = ATTR1_BUILDR(0, size_, 0, 0);
	objData.attr2 = ATTR2_BUILD(tileIndex_, 0, priority_);
	used[index] = 1;
}

Sprite::~Sprite() {
	if (index != -1) {
		hidden(true);
		used[index] = 0;
	}
}

Sprite::Sprite(Sprite &&other) {
	*this = std::move(other);
}

Sprite& Sprite::operator=(Sprite &&other) {
	index = other.index;
	other.index = -1;
	return *this;
}

std::optional<Sprite> Sprite::create(int index, int priority, SprBitDepht bpp, SprSize area, int tileIndex) {
	if (index > 3 || priority > 3 || tileIndex > 1023) {
		return std::nullopt;
	}
	if (used[index]) {
		return std::nullopt;
	}

	u16 size  = static_cast<int>(area) % 4;
	u16 shape = static_cast<int>(area) / 4;

	return Sprite(index, priority, static_cast<u16>(bpp), shape, size, tileIndex);
}


int Sprite::x() const {
	return get_object_data().attr1 & ATTR1_X_MASK;
}

int Sprite::y() const {
	return get_object_data().attr0 & ATTR0_Y_MASK;
}

bool Sprite::hidden() const {
	return (get_object_data().attr0 & ATTR0_MODE_MASK) == 2;
}

void Sprite::position(int x, int y) {
	obj_set_pos(&get_object_data(), x, y);
}

void Sprite::x(int val) {
	position(val, y());
}

void Sprite::y(int val) {
	position(x(), val);
}

void Sprite::hidden(bool hide) {
	BIT_CLEAR(get_object_data().attr0, ATTR0_MODE_MASK);
	BIT_SET(get_object_data().attr0, hide ? ATTR0_MODE(2) : 0);
}