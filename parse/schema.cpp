#include "schema.h"

namespace parse {

schema::schema() {
}

schema::~schema() {
}

void schema::expect(tokenizer &tokens, std::any data) {
	tokens.expect(label, data);
}

bool schema::found(tokenizer &tokens) const {
	return tokens.found(label);
}

bool schema::empty() const {
	return label.empty();
}

}
