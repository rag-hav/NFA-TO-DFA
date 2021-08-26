#include <bits/stdc++.h>
using namespace std;
class NFA {
    public:
        map<int, map<char, vector<int>>> transition_table;
        set<int> current_states;
        set<char> E;
        int initial_state;
        set<int> final_states;

        NFA(set<char> _E, int _initial_state, set<int> _final_states, map<int, map<char, vector<int>>> _transition_table)
        {
                E = _E;
                current_states.insert(_initial_state);
                initial_state = _initial_state;
                transition_table = _transition_table;
                final_states = _final_states;
        }

        void next_state(char inp)
        {
                set<int> next_states;
                for (auto q : current_states) {
                        next_states.insert(transition_table[q][inp].begin(), transition_table[q][inp].end());
                        // epsilon is null character or zero
                        next_states.insert(transition_table[q][0].begin(), transition_table[q][0].end());
                }

                current_states = next_states;

                bool result = 0;
                cout << "Current states are ";
                for (auto q : current_states) {
                        cout << q << ' ';
                        if (!result)
                                result |= final_states.find(q) != final_states.end();
                }
                cout << endl;
        }

        void run()
        {
                cout << "Initial state " << *current_states.begin() << endl;
                char x;
                while (cin >> x)
                        next_state(x);
        }
};

class DFA {
    public:
        map<int, map<char, int>> transition_table;
        set<int> final_states;
        set<char> E;
        int current_state;

        DFA(set<char> _E, int initial_state, set<int> _final_states, map<int, map<char, int>> _transition_table)
        {
                E = _E;
                current_state = initial_state;
                transition_table = _transition_table;
                final_states = _final_states;
        }

        void next_state(char inp)
        {
                current_state = transition_table[current_state][inp];
                cout << "Current state " << current_state << endl;
                if (final_states.find(current_state) != final_states.end())
                        cout << "yes" << endl;
                else
                        cout << "no" << endl;
        }
        void run()
        {
                cout << "Initial state " << current_state << endl;
                char x;
                while (cin >> x)
                        next_state(x);
        }
};

DFA nfatodfa(NFA nfa)
{

        set<int> final_states;
        map<int, map<char, int>> transition_table;
        int n = nfa.transition_table.size();

        for (int subset = 0; subset < (1 << n); subset++) {
                for (int i = 0; i < n; i++) {
                        if ((subset & (1 << i)) && nfa.final_states.find(i) != nfa.final_states.end())
                                final_states.insert(subset);
                }

                for (auto inp : nfa.E) {
                        int next_states = 0;

                        for (int i = 0; i < n; i++) {
                                if ((subset & (1 << i))) {
                                        for (auto next_state : nfa.transition_table[i][inp]) {
                                                next_states |= 1 << next_state;
                                                for (auto epsilon_next_state : nfa.transition_table[next_state][0])
                                                        next_states |= 1 << epsilon_next_state;
                                        }
                                }
                        }

                        transition_table[subset][inp] = next_states;
                }
        }

        return DFA(nfa.E, (1 << nfa.initial_state), final_states, transition_table);
}

int main()
{
        NFA nfa({ 'a', 'b' }, 0, { 2 },
            {
                { 0, { { 'a', { 0, 1 } }, { 'b', { 2 } } } },
                { 1, { { 'a', { 0 } }, { 'b', { 1 } } } },
                { 2, { { 'b', { 0, 1 } } } },
            });

        DFA dfa = nfatodfa(nfa);

        nfa.run();
}
