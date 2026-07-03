#include<iostream>
#include<string>

using namespace std;

/*
ImageProxy

↓

Load image

↓

Display

STEP-2
ImageProxy

↓

Already loaded

↓

Display
*/
class IImage { 
public:
    virtual void display() = 0;
    virtual ~IImage() = default;
};

class RealImage : public IImage {
    string filename;
public:
    RealImage(string file) {
        this->filename = file;
        // Heavy Operation
        cout << "[RealImage] Loading image from disk: " << filename << "\n";
    }

    void display() override {
        cout << "[RealImage] Displaying " << filename << "\n";
    }
};
    
class ImageProxy : public IImage {
    RealImage* realImage;
    string filename;
public:
    ImageProxy(string file) {
        this->filename = file;
        realImage = nullptr;
    }

    void display() override {
        // Lazy initialization of RealImage
        if (!realImage) {
            realImage = new RealImage(filename);
        }
        realImage->display();
    }
};

int main() {

    IImage* image1 = new ImageProxy("sample.jpg");
    image1->display();

    
}
    
/*
| Protection Proxy           | Remote Proxy                         | Virtual Proxy                           |
| -------------------------- | ------------------------------------ | --------------------------------------- |
| Controls access            | Represents a remote object           | Delays object creation                  |
| Checks permissions         | Handles networking                   | Performs lazy initialization            |
| Real object already exists | Real object may be on another server | Real object is created only when needed |
| Example: Premium users     | Example: Web service                 | Example: Large image                    |

*/