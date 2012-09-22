/*
    search.h: Best-first search algorithm, taken from Russell & Norvig's AIMA.
    Copyright (C) 2012  Jeremy Murphy <jeremy.william.murphy@gmail.com>

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

#ifndef SEARCH_H
#define SEARCH_H

#include "problem.h"
#include "evaluation.h"

#include <queue>
#include <set>
#include <algorithm>
#include <stdexcept>

#ifndef NDEBUG
#include <iostream>
#endif


namespace jsearch
{
	class GoalNotFound : public std::exception {};
	
	template <typename Traits,
			template <typename PathCost, typename State, typename Action> class StepCostPolicy,
			template <typename State, typename Action> class ActionsPolicy,
			template <typename State, typename Action> class ResultPolicy,
			template <typename State> class GoalTestPolicy,
			template <typename Traits,
				template <typename PathCost, typename State, typename Action> class StepCostPolicy,
				template <typename State, typename Action> class ResultPolicy >
					class ChildPolicy = DefaultChildPolicy,

			template <typename PathCost, typename State> class HeuristicPolicy = ZeroHeuristic,
			template <typename State, typename PathCost> class PathCostPolicy = DefaultPathCost,
			template <typename Traits,
				template <typename State, typename PathCost> class PathCostPolicy,
				template <typename PathCost, typename State> class HeuristicPolicy> class Comparator = AStarNodeComparator>
	typename Traits::node search(Problem<Traits, StepCostPolicy, ActionsPolicy, ResultPolicy, GoalTestPolicy, ChildPolicy> const &PROBLEM, Evaluation<HeuristicPolicy, PathCostPolicy, Comparator> const &, bool const TREE = false)
	{
		typedef typename Traits::node Node;
		typedef typename Traits::state State;
		typedef typename Traits::action Action;
		typedef typename Traits::pathcost PathCost;

		typedef std::set<Node, Comparator<Traits, PathCostPolicy, HeuristicPolicy>> OpenList;
		typedef std::set<State> ClosedList;


		// std::priority_queue<Node, std::vector<Node>, Comparator<Traits, PathCostPolicy, HeuristicPolicy>> open;
		OpenList open;
		ClosedList closed; // TODO: Make the closed list optional for combinatorial search.
		open.insert(Node(PROBLEM.initial(), nullptr, Action(), 0));

		while(!open.empty())
		{
			typename OpenList::const_iterator F = std::begin(open);
			Node const &S = *F; // BUG: Lifetime??
			open.erase(F);
			// open.pop();
			
			if(PROBLEM.goal_test(S.state))
			{
#ifndef NDEBUG
				std::cerr << "open: " << open.size() << ", closed: " << closed.size() << "\n";
#endif
				return S; // OK, I don't like non-local returns, but what else?
			}
			else
			{
				closed.insert(S.state);
				std::set<Action> const actions = PROBLEM.actions(S.state);
				auto const beginning = std::begin(actions), ending = std::end(actions);
				std::for_each(beginning, ending, [&](typename std::set<Action>::const_reference ACTION)
				{
					auto const child = Node(PROBLEM.result(S.state, ACTION), &S, ACTION, S.path_cost + PROBLEM.step_cost(S.state, ACTION));

					if(!TREE)
					{
						// TODO: Check if it is in open or closed.  Could someone else deal with this, please?  :)
						auto low = std::lower_bound(std::begin(open), std::end(open), child, Comparator<Traits, PathCostPolicy, HeuristicPolicy>());
					}
					else
					{
						/*	The combinatorial search spaces in mind are a tree with no repeating nodes,
							so the algorithm is optimized to not worry about checking in open or closed.	*/
						open.insert(child);
					}

				});
			}

		}
		throw GoalNotFound();
	}
}

#endif // SEARCH_H
