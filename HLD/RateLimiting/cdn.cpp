#include <iostream>
#include <string>
#include <unordered_map>
using namespace std;

class CDN {
private:
    unordered_map<string, string> cache;
public:
    string getContent(string url) {
        // Check whether content exists in CDN cache
        if (cache.find(url) != cache.end()) {
            cout << "CDN Cache HIT: " << url << "\n";
            return cache[url];
        }

        // Simulate fetching content from the origin server
        cout << "CDN Cache MISS: " << url << "\n";
        cout << "Fetching from Origin Server...\n";

        string content = "Content for " + url;

        // Store content in CDN cache
        cache[url] = content;

        return content;
    }
};
int main() {
    CDN cdn;

    cout << cdn.getContent("/images/logo.png") << "\n\n";
    cout << cdn.getContent("/images/logo.png") << "\n\n";
    cout << cdn.getContent("/videos/demo.mp4") << "\n";

    return 0;
}
