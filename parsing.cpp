#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include <vector>

struct coords {
	std::string x;
	std::string y;
	std::string z;
	
	coords(std::string x, std::string y, std::string z) : x(x), y(y), z(z) {}
	
	bool operator==(const coords & c) {
		return x==c.x && y==c.y && z==c.z;
	}
	
	bool operator!=(const coords & c) {
		return x!=c.x && y!=c.y && z!=c.z;
	}
	
	friend std::ostream & operator<<(std::ostream & os, const coords & c) {
		os << "(" << c.x << ",  " << c.y << ",  " << c.z << ")";
		return os;
	}
};

struct atom {
	int count;
	std::vector<coords> cs;
	
	atom(int x) : count(x) {}
	
	void add_coords(coords c) {
		if((int)cs.size() < count)
			cs.push_back(c);
	}
	
	bool operator==(const atom & a) {
		if(this->count != a.count)
			return false;
		for(int i=0; i<count; i++)
			if(this->cs[i] != a.cs[i])
				return false;
		return true;
	}
	
	friend std::ostream & operator<<(std::ostream & os, const atom & at) {
		for(int i=0; i<at.count-1; i++) {
			os << at.cs[i] << ",\t";
		}
		os << at.cs[at.count-1] << std::endl;
		return os;
	}
};

coords parse_coords(std::string group);

atom parse_atom(std::string line) {
	std::regex finding_coords("(-?\\d,[ ]+-?\\d,[ ]+-?\\d,[ ]+-?\\d)");
	auto groups_begin = std::sregex_iterator(line.begin(), line.end(), finding_coords);
	auto groups_end = std::sregex_iterator();
	
	int atom_size = std::distance(groups_begin, groups_end);
	atom new_atom(atom_size);
	for(std::sregex_iterator i = groups_begin; i != groups_end; i++) {
		std::smatch match = *i;
		std::string match_str = match.str();
		coords found_coords = parse_coords(match_str);
		new_atom.add_coords(found_coords);
	}
	
	return new_atom;
}

coords parse_coords(std::string group) {
	std::regex splitting_to_ints("(-?\\d)");
	std::cout << group << std::endl;
	auto ints_begin = std::sregex_iterator(group.begin(), group.end(), splitting_to_ints);
	auto ints_end = std::sregex_iterator();
	
	int j=0;
	std::string t[3];
	for(std::sregex_iterator i = ints_begin; i!=ints_end,j<3; ++i) {
		std::smatch match = *i;
		std::string match_str = match.str();
		t[j] = match_str;
		j++;
	}

	coords c(t[0], t[1], t[2]);
	return c;
}

int main() {
	std::ifstream hamiltonian("hamiltonian.txt");
	std::ofstream output("output.txt");
	std::string line;
	std::regex group_start("([A-Za-z]*)_body");
	std::regex group_end("end_[A-Za-z]*_body");
	std::string group_title;
	std::vector<atom> unique_atoms;
	
	int i=0;
	
	while(std::getline(hamiltonian, line)) {
		if(line == "")
			;
		else if(std::regex_match(line, group_start)) {
			auto it = std::sregex_iterator(line.begin(), line.end(), group_start);
			std::smatch match = *it;
			group_title = match.str();
		}
		else if(std::regex_match(line, group_end)) {
			if(output.is_open()) {
				output << group_title << std::endl;
				for(atom at : unique_atoms) {
					output << "a" + std::to_string(i) << "\t";
					output << at << std::endl;
					i++;
				}
				unique_atoms.clear();
			}
		}
		else {
			atom at = parse_atom(line);
			bool is_unique = true;
			for(atom existing : unique_atoms)
				if(at == existing) {
					is_unique = false;
					break;
				}
			if(is_unique)
				unique_atoms.push_back(at);
		}
	}
	
	output.close();
	return 0;
}
