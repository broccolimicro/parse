#include "factory.h"

namespace parse {

factory::factory() : register_syntax(nullptr), expect(nullptr), produce(nullptr), data(nullptr) {
}

factory::factory(const Register register_syntax, const Expect expect, const Produce produce, void *data) :
	register_syntax(register_syntax), expect(expect), produce(produce), data(data) {
}

factory::~factory() {
}

factory::operator bool() const {
	return register_syntax != nullptr and expect != nullptr and produce != nullptr;
}

}

