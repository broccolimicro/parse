#include "new_line.h"

#include "../token.h"

namespace parse {

new_line::new_line() {
	debug_name = "new_line";
}

new_line::~new_line() {
}

token new_line::consume(tokenizer &tokens, std::any data) {
	token result;
	result.type = tokens.token_type<new_line>();
	result.start = tokens.offset+1;

	tokens.next_char();

	result.end = tokens.offset+1;
	return result;
}

bool new_line::is_next(tokenizer &tokens, int i, std::any data) {
	return (tokens.peek_char(i) == '\n');
}

}
