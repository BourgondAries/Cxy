// Copyright © 2015-2016 Kevin Robert Stravers
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
#include "SymbolTable.hpp"


namespace tul { namespace sym {

bool SymbolTable::addModule
(
	const std::string &qualified_name,
	Module *mod_entry
)
{
	Module *mod = findModule(qualified_name);
	if (mod) return true;
	module_map[qualified_name] = mod_entry;
	return false;
}

Entry *SymbolTable::findEntry(const std::string &qualified_name)
{
	Module *module = findModule(qualified_name);
	return module->findEntry(qualified_name);
}

Module *SymbolTable::findModule(const std::string &qualified_name)
{
	std::map<std::string, Module *>::const_iterator mod_it
		= module_map.find(qualified_name);
	if (mod_it == module_map.cend())
		return nullptr;
	else
		return mod_it->second;
}

}}
