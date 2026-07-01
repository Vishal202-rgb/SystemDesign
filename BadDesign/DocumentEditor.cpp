#include <iostream>
#include <vector>
#include <string>
#include <fstream>

using namespace std;
/*
Summary of Design Issues
Problem	                  Explanation
1.SRP Violation	    DocumentEditor stores data, renders it, identifies element types, and saves files—all in one class.
2.OCP Violation	    Every new document element (PDF, Video, Table, Audio, etc.) requires modifying renderDocument().
3.No Polymorphism	Elements are stored as string, so behavior is decided using if-else instead of virtual functions.
4.DIP Violation	    DocumentEditor directly depends on ofstream, making it difficult to switch to another storage mechanism (database, cloud, PDF, etc.).
5.Weak Type Safety	An image is identified by checking its filename extension (.jpg, .png) instead of representing it as an Image object.
*/
class DocumentEditor {
private:
    vector<string> documentElements;
    string renderedDocument;

public:
    // Adds text as a plain string
    void addText(string text) {
        documentElements.push_back(text);
    }

    // Adds an image represented by its file path
    void addImage(string imagePath) {
        documentElements.push_back(imagePath);
    }

    // Renders the document by checking the type of each element at runtime
    string renderDocument() {
        if(renderedDocument.empty()) {
            string result;
            for (auto element : documentElements) {
                if (element.size() > 4 && (element.substr(element.size() - 4) == ".jpg" ||
                 element.substr(element.size() - 4) == ".png")) {
                    result += "[Image: " + element + "]" + "\n";
                } else {
                    result += element + "\n";
                }
            }
            renderedDocument = result;
        }
        return renderedDocument;
    }

    void saveToFile() {
        ofstream file("document.txt");
        if (file.is_open()) {
            file << renderDocument();
            file.close();
            cout << "Document saved to document.txt" << endl;
        } else {
            cout << "Error: Unable to open file for writing." << endl;
        }
    }
};

int main() {
    DocumentEditor editor;
    editor.addText("Hello, world!");
    editor.addImage("picture.jpg");
    editor.addText("This is a document editor.");

    cout << editor.renderDocument() << endl;

    editor.saveToFile();
    
    return 0;
}