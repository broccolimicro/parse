# parse

This library contains everything you need to easily write a good parser. This includes
keeping track of an import tree, tokenizing a string, and handling and reporting parsing errors.
It is very flexible, allowing you to define your own basic token type and build on them to form
a grammar. The only drawback to this library is that it is unable to automatically resolve ambiguous
grammars. So your grammar needs to be determinable by looking at most a few tokens ahead.

**Dependencies**:

 - common

## License

Licensed by Cornell University under GNU GPL v3.

Written by Ned Bingham.
Copyright © 2020 Cornell University.

Haystack is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Haystack is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

A copy of the GNU General Public License may be found in COPYRIGHT.
Otherwise, see <https://www.gnu.org/licenses/>.

