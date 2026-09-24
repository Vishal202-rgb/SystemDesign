#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <memory> // Required for unique_ptr and make_unique
#include <utility> // Required for std::move

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

// Abstract base class for document elements
class DocumentElement {
public:
    virtual ~DocumentElement() = default;
    virtual string render() const = 0;
};

// Concrete class for text elements
class TextElement : public DocumentElement {
private:
    string text;
public:
    TextElement(string t) : text(std::move(t)) {}
    string render() const override {
        return text;
    }
};

// Concrete class for image elements
class ImageElement : public DocumentElement {
private:
    string imagePath;
public:
    ImageElement(string path) : imagePath(std::move(path)) {}
    string render() const override {
        return "[Image: " + imagePath + "]";
    }
};

// Document class to manage a collection of DocumentElements
class Document {
private:
    vector<unique_ptr<DocumentElement>> elements;

public:
    void addElement(unique_ptr<DocumentElement> element) {
        elements.push_back(std::move(element));
    }

    string render() const {
        string result;
        for (const auto& element : elements) {
            result += element->render() + "\n";
        }
        return result;
    }
};

// Abstract interface for persistence
class Persistence {
public:
    virtual ~Persistence() = default;
    virtual void save(const string& content) const = 0;
};

// Concrete implementation for file storage
class FileStorage : public Persistence {
private:
    string filename;
public:
    FileStorage(string fn) : filename(std::move(fn)) {}

    void save(const string& content) const override {
        ofstream file(filename);
        if (file.is_open()) {
            file << content;
            file.close();
            cout << "Document saved to " << filename << endl;
        } else {
            cout << "Error: Unable to open file '" << filename << "' for writing." << endl;
        }
    }
};

int main() {
    Document doc;
    doc.addElement(make_unique<TextElement>("Hello, world!"));
    doc.addElement(make_unique<ImageElement>("picture.jpg"));
    doc.addElement(make_unique<TextElement>("This is a document editor."));

    string renderedContent = doc.render();
    cout << renderedContent << endl;

    FileStorage fileSaver("document.txt");
    fileSaver.save(renderedContent);
    
    return 0;
}