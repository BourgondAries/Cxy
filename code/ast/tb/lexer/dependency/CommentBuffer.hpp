// Copyright © 2015 Kevin Robert Stravers
/*
This file is part of Cxy Compiler Reference Implementation (Cxy CRI).

Cxy CRI is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Cxy CRI is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Cxy CRI.  If not, see <http://www.gnu.org/licenses/>.
*/
#pragma once

#include "utilities/RingNumber.hpp"

#include <cstdint>


namespace tul { namespace dependency {

class CommentBuffer
{
public:

	CommentBuffer();

	void putCharInto(char char_);
	char getCharFrom(uint8_t location_);

private:

	static constexpr const std::size_t CYCLE_BUFFER_SIZE = 2;
	utilities::RingNumber<uint8_t> cycle_buffer;
	char cycle_array[CYCLE_BUFFER_SIZE];

};

}}
