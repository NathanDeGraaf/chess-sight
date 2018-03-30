#include "opencv2/dnn.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
using namespace cv;
using namespace dnn;

#include <fstream>
#include <iostream>
#include <cstdlib>
using namespace std;

/* Find best class for the blob (i. e. class with maximal probability) */
void getMaxClass(Blob &probBlob, int *classId, double *classProb)
{
    Mat probMat = probBlob.matRefConst().reshape(1, 1); //reshape the blob to 1x1000 matrix
    Point classNumber;

    minMaxLoc(probMat, NULL, classProb, NULL, &classNumber);
    *classId = classNumber.x;
}

vector<String> readClassNames(const char *filename = "synset_words.txt")
{
    vector<String> classNames;

    ifstream fp(filename);
    if (!fp.is_open())
    {
        cerr << "File with classes labels not found: " << filename << endl;
        exit(-1);
    }

    string name;
    while (!fp.eof())
    {
        getline(fp, name);
        if (name.length())
            classNames.push_back( name.substr(name.find(' ')+1) );
    }

    fp.close();
    return classNames;
}

int main(int argc, char **argv)
{
    String modelTxt = "bvlc_googlenet.prototxt";
    String modelBin = "bvlc_googlenet.caffemodel";
    String imageFile = (argc > 1) ? argv[1] : "space_shuttle.jpg";

    //! [Create the importer of Caffe model]
    Ptr<Importer> importer;
    try                                     //Try to import Caffe GoogleNet model
    {
        importer = createCaffeImporter(modelTxt, modelBin);
    }
    catch (const Exception &err)        //Importer can throw errors, we will catch them
    {
        cerr << err.msg << endl;
    }
    //! [Create the importer of Caffe model]

    if (!importer)
    {
        cerr << "Can't load network by using the following files: " << endl;
        cerr << "prototxt:   " << modelTxt << endl;
        cerr << "caffemodel: " << modelBin << endl;
        cerr << "bvlc_googlenet.caffemodel can be downloaded here:" << endl;
        cerr << "http://dl.caffe.berkeleyvision.org/bvlc_googlenet.caffemodel" << endl;
        exit(-1);
    }

    //! [Initialize network]
    Net net;
    importer->populateNet(net);
    importer.release();                     //We don't need importer anymore
    //! [Initialize network]

    //! [Prepare blob]
    Mat img = imread(imageFile);
    if (img.empty())
    {
        cerr << "Can't read image from the file: " << imageFile << endl;
        exit(-1);
    }

    resize(img, img, Size(224, 224));       //GoogLeNet accepts only 224x224 RGB-images
    Blob inputBlob = Blob(img);   //Convert Mat to Blob image batch
    //! [Prepare blob]

    //! [Set input blob]
    net.setBlob(".data", inputBlob);        //set the network input
    //! [Set input blob]

    //! [Make forward pass]
    net.forward();                          //compute output
    //! [Make forward pass]

    //! [Gather output]
    Blob prob = net.getBlob("prob");   //gather output of "prob" layer

    int classId;
    double classProb;
    getMaxClass(prob, &classId, &classProb);//find the best class
    //! [Gather output]

    //! [Print results]
    vector<String> classNames = readClassNames();
    cout << "Best class: #" << classId << " '" << classNames.at(classId) << "'" << endl;
    cout << "Probability: " << classProb * 100 << "%" << endl;
    //! [Print results]

    return 0;
} //main