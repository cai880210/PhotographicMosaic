#include "AveragePatchDatabase.h"

#include <stdlib.h>     /* srand, rand */

AveragePatchDatabase::AveragePatchDatabase() : images_(), PatchDatabase() {

}

AveragePatchDatabase::~AveragePatchDatabase() {

}
/*
    Adds an image to the databse and a descriptor for that image.
*/
void AveragePatchDatabase::add(const cv::Mat image) {
    images_.push_back(image);
    std::vector<int> avgColour(AveragePatchDatabase::averageColour(image));
    descriptors_.push_back(avgColour);
}

/*
    Finds an image that has a similar average colour to the passed image.
*/
const cv::Mat AveragePatchDatabase::find(const cv::Mat image) const {
    // Get avg colour
    std::vector<int> avgColour(AveragePatchDatabase::averageColour(image));

    // find an image that has a similar colour match
    int photoIndex = AveragePatchDatabase::imagePicker(avgColour);

    // return image to replace tile
    return images_.at(photoIndex);
}

/*
    Returns the size of the image database.
*/
size_t AveragePatchDatabase::size() const {
    return images_.size();
}

/*
    Returns and index to an image that matches well with the descriptors of passed image.
    Can return -1 if no match is found.
*/
int AveragePatchDatabase::imageMatch(const cv::Mat image) const {
    return -1;
}

/*
    Returns an image at a passed index.
*/
const cv::Mat AveragePatchDatabase::imageAtIndex(int index) const {
    return images_.at(index);
}

/*
    Returns an image from the database that has a close comparison to the descriptor passed.
    Image is not always best fit, but will be close.
*/
int AveragePatchDatabase::imagePicker(std::vector<int> avgColour) const{
    int bestDiffIndex;
    int bestDiffValue = 1000; //initialise to some large number
    int diffValue;
    std::vector<int> diffValues;

    // compares the difference between the averages of both image tiles
    for (int i = 0; i < descriptors_.size(); ++i){
        std::vector<int> bgrImage (descriptors_.at(i));
        diffValue = 0;
        diffValue += abs(avgColour.at(0) - bgrImage.at(0));
        diffValue += abs(avgColour.at(1) - bgrImage.at(1));
        diffValue += abs(avgColour.at(2) - bgrImage.at(2));
        diffValues.push_back(diffValue);
        if (bestDiffValue > diffValue) {
            bestDiffValue = diffValue;
            bestDiffIndex = i;
        }
    }

    std::vector<int> topValues; // vector of the best fit indexes

    // Find images with the closest match
    for (int i = 0; i < diffValues.size(); ++i) {
        if(diffValues.at(i)>= bestDiffValue && diffValues.at(i)<= bestDiffValue + 2 ) {
            topValues.push_back(i);
        }
    }

    // If not enough images were found, loosen criteria
    if (topValues.size() < 5) {
        int moe = 10; // margin of error
        while(topValues.size() < 5) {
            topValues.clear(); // to besure we have no duplicates
            for (int i = 0; i < diffValues.size(); ++i) {
                if(diffValues.at(i)>= bestDiffValue && diffValues.at(i)<= bestDiffValue + moe ) {
                    topValues.push_back(i);
                }
            }
            moe += 10;
        }
    }


    //initialize random seed
    int randValue = rand()%topValues.size();

    return topValues.at(randValue);
}

/*
    Returns a descriptor vector which describes the average colour of the passed image.
*/
const std::vector<int> AveragePatchDatabase::averageColour(const cv::Mat image) const{
    int redAvg = 0;
    int greenAvg = 0;
    int blueAvg = 0;

    // Find the average colour for the image
    for (int x = 0; x < image.size().width; ++x) {
        for (int y = 0; y < image.size().height; ++y) {
            blueAvg += image.at<cv::Vec3b>(y,x)[0];  //  Blue    
            greenAvg += image.at<cv::Vec3b>(y,x)[1];  //  Green   
            redAvg += image.at<cv::Vec3b>(y,x)[2];  //  Red 
        }
    }

    // Average the found colours
    int pixels = image.size().width * image.size().height;
    redAvg = redAvg / pixels;
    greenAvg = greenAvg / pixels;
    blueAvg = blueAvg / pixels;

    int myRGBValues[] = {blueAvg, greenAvg, redAvg};
    std::vector<int> avgColour (myRGBValues, myRGBValues + sizeof(myRGBValues) / sizeof(int) );
    return avgColour;
}