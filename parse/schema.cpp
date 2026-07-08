#include "schema.h"

namespace parse {

schema::schema() {
}

schema::~schema() {
}

bool schema::empty() const {
	return label.empty();
}

}
