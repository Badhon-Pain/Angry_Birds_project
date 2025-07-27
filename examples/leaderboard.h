#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
using namespace std;

struct PlayerScore {
    string name;
    int score;
};

vector<PlayerScore> leaderboard;

// Load leaderboard from file
void loadLeaderboard(const string& filename) {
    leaderboard.clear();
    ifstream fin(filename);
    string name;
    int score;
    while (fin >> name >> score) {
        leaderboard.push_back({name, score});
    }
    fin.close();
}

// Save leaderboard to file
void saveLeaderboard(const string& filename) {
    ofstream fout(filename);
    for (const auto& entry : leaderboard) {
        fout << entry.name << " " << entry.score << endl;
    }
    fout.close();
}

// Add a new score
void addScore(const string& name, int score) {
    leaderboard.push_back({name, score});
    sort(leaderboard.begin(), leaderboard.end(), [](const PlayerScore& a, const PlayerScore& b) {
        return b.score < a.score; // Descending order
    });
    if (leaderboard.size() > 10) leaderboard.resize(10); // Keep top 10
}

// Display leaderboard
void displayLeaderboard() {
    cout << "Leaderboard:" << endl;
    int rank = 1;
    for (const auto& entry : leaderboard) {
        cout << rank++ << ". " << entry.name << ": " << entry.score << endl;
    }
}