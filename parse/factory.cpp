#include "factory.h"

namespace parse {

factory::factory() = default;

factory::factory(schema sub, std::any data) : sub(sub), data(data) {
}

factory::~factory() = default;

void factory::register_syntax(tokenizer &tokens) const {
	sub.register_syntax(tokens);
}

void factory::expect(tokenizer &tokens) const {
	sub.expect(tokens, data);
}

parse::syntax *factory::produce(tokenizer &tokens) const {
	return sub.produce(tokens, data);
}

bool factory::is_next(tokenizer &tokens, int i) const {
	return sub.is_next(tokens, i, data);
}

bool factory::found(tokenizer &tokens) const {
	return sub.found(tokens);
}

bool factory::empty() const {
	return sub.empty();
}

}

