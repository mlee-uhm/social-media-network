#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <iterator>

using namespace std;

// User structure to store user attributes
struct User {
    int id;
    int age;
    string gender;
    string region;
};

// Post structure to store post content
struct Post {
    int id;
    int userId;
    string content;
};

// Function to clean and tokenize
vector<string> tokenize(const string& text) {
    string cleanedText;
    for (char c : text) {
        if (isalnum(c) || isspace(c)) {
            cleanedText += tolower(c);
        }
    }

    istringstream stream(cleanedText);
    return vector<string>(istream_iterator<string>{stream}, istream_iterator<string>{});
}

// Function filter posts based on user attributes and keywords
vector<Post> filterPosts(const vector<Post>& posts,
                         const unordered_map<int, User>& users,
                         const unordered_set<string>& keywords,
                         const unordered_set<string>& excludeKeywords,
                         int minAge, int maxAge,
                         const string& gender,
                         const string& region) {
    vector<Post> filteredPosts;

    for (const auto& post : posts) {
        // Check user attributes
        const User& user = users.at(post.userId);
        if (user.age < minAge || user.age > maxAge) continue;
        if (!gender.empty() && user.gender != gender) continue;
        if (!region.empty() && user.region != region) continue;

        // Check keywords
        vector<string> words = tokenize(post.content);
        bool include = false;

        for (const auto& word : words) {
            if (keywords.count(word)) {
                include = true;
            }
            if (excludeKeywords.count(word)) {
                include = false;
                break;
            }
        }

        // Add post if pass both filters
        if (include) {
            filteredPosts.push_back(post);
        }
    }

    return filteredPosts;
}

// Function count word frequencies in filtered posts
unordered_map<string, int> countWordFrequencies(const vector<Post>& posts) {
    unordered_map<string, int> wordCount;
    unordered_set<string> stopWords = {"the", "and", "is", "in", "of", "a", "to"}; // Add more stop words as needed

    for (const auto& post : posts) {
        vector<string> words = tokenize(post.content);
        for (const auto& word : words) {
            if (!stopWords.count(word)) {
                wordCount[word]++;
            }
        }
    }

    return wordCount;
}

int main() {
    // Users
    unordered_map<int, User> users = {
            {1, {1, 25, "male", "North America"}},
            {2, {2, 30, "female", "Europe"}},
            {3, {3, 21, "female", "North America"}}
    };

    // Posts
    vector<Post> posts = {
            {1, 1, "Social media algorithms are fascinating"},
            {2, 2, "Algorithms help us find patterns"},
            {3, 3, "Social connections are essential"}
    };

    // Filters
    unordered_set<string> includeKeywords = {"algorithms"};
    unordered_set<string> excludeKeywords = {};  // Empty for now
    int minAge = 18, maxAge = 30;
    string gender = "female";
    string region = "North America";

    // Filter posts
    vector<Post> filteredPosts = filterPosts(posts, users, includeKeywords, excludeKeywords, minAge, maxAge, gender, region);

    // Count word frequencies
    unordered_map<string, int> wordFrequencies = countWordFrequencies(filteredPosts);

    // Display word frequencies
    cout << "Word Frequencies:" << endl;
    for (const auto& [word, count] : wordFrequencies) {
        cout << word << ": " << count << endl;
    }

    return 0;
}
