# libparse

This library contains everything you need to easily write a good parser. This includes
keeping track of an import tree, tokenizing a string, and handling and reporting parsing errors.
It is very flexible, allowing you to define your own basic token type and build on them to form
a grammar. The only drawback to this library is that it is unable to automatically resolve ambiguous
grammars. So your grammar needs to be determinable by looking at most a few tokens ahead.

**Dependencies**:

 - common
