/*
    random.cpp: Loader for random data test.
    Copyright (C) 2012  Jeremy W. Murphy <jeremy.william.murphy@gmail.com>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "random.h"
#include "search.h"
#include "to_string.h"

#include <boost/heap/fibonacci_heap.hpp>
#include <boost/heap/pairing_heap.hpp>
#include <boost/heap/binomial_heap.hpp>
#include <boost/heap/d_ary_heap.hpp>

#include <iostream>
#include <sstream>
#include <locale>

using namespace jsearch;
using namespace std;

template <typename T>
using PriorityQueue = boost::heap::fibonacci_heap<T, boost::heap::compare<Dijkstra<Random>>>;

int main(int argc, char **argv)
{
	istringstream(argv[1]) >> max_nodes;
	Random::state INITIAL(B);
	Problem<Random, Distance, Neighbours, Visit, GoalTest> const PROBLEM(INITIAL);
	cout.imbue(locale(""));

	try
	{
		auto const SOLUTION = jsearch::best_first_search<PriorityQueue>(PROBLEM);
		cout << "Done.\n";
	}
	catch (goal_not_found const &ex)
	{
		cout << "No path from " << jwm::to_string(INITIAL) << " to the goal could be found!\n";
	}
	
}