#include "RGBGridPatchDatabase.h"

#include <stdlib.h> // rand

RGBGridPatchDatabase::RGBGridPatchDatabase() : images_(), PatchDatabase() {

}

RGBGridPatchDatabase::~RGBGridPatchDatabase() {

}

/*
    Adds an image to the databse and a descriptor for that image.
*/
void RGBGridPatchDatabase::add(const cv::Mat image) {
    images_.push_back(image);
    std::vector<int> imageDescriptor(RGBGridPatchDatabase::describeImage(image));
    descriptors_.push_back(imageDescriptor);
}

/*
    Finds an image that has a similar average colour to the passed image.
*/
const cv::Mat RGBGridPatchDatabase::find(const cv::Mat image) const {
    // Get avg colour
    std::vector<int> imageDescriptor(describeImage(image));

    // find an image that has a similar colour match
    int photoIndex = RGBGridPatchDatabase::imagePicker(imageDescriptor);

    // return image to replace tile
    return images_.at(photoIndex);
}

/*
    Returns the size of the image database.
*/
size_t RGBGridPatchDatabase::size() const {
    return images_.size();
}

/*
    Returns and index to an image that matches well with the descriptors of passed image.
    Can return -1 if no match is found.
*/
int RGBGridPatchDatabase::imageMatch(const cv::Mat image) const {
    // Get avg colour
    std::vector<int> imageDescriptor(describeImage(image));

    // find an image that has a similar colour
    int photoIndex = RGBGridPatchDatabase::imagePicker(imageDescriptor);


    // Calculate the difference value
    int diffValue = 0;
    for (int i = 0; i < imageDescriptor.size(); ++i) {
        diffValue += abs(imageDescriptor.at(i) - descriptors_.at(photoIndex).at(i));
    }

    // if the difference value is below the range, return the index
    if (diffValue < 700) {// 700 was a good average number looking at the dataset
        return photoIndex;
    }else {
        return -1; // no index found
    }
}

/*
    Returns an image at a passed index.
*/
const cv::Mat RGBGridPatchDatabase::imageAtIndex(int index) const {
    return images_.at(index);
}

/*
    Splits a passed image into 16 square pieces in a 4x4 grid.
    Returns a descriptor vector containing the RGB data for each grid patch.
*/
const std::vector<int> RGBGridPatchDatabase::describeImage(const cv::Mat image) const {
    std::vector<int> imageDescriptor;
    int tileSize = image.size().width/4; // calculates the tile size to split into a 4x4 grid

    // cycles over each patch to find the RGB values and adds them to the images descriptor
    for (int x = 0; x <= image.size().width-tileSize; x += tileSize) {
        for (int y = 0; y <= image.size().height-tileSize; y += tileSize) {
            cv::Rect roi(x,y,tileSize,tileSize);
            cv::Mat partialImage(image(roi));

            std::vector<int> tempValues(averageColour(partialImage));

            imageDescriptor.push_back(tempValues.at(0));
            imageDescriptor.push_back(tempValues.at(1));
            imageDescriptor.push_back(tempValues.at(2));
        }
    }

    return imageDescriptor;
}

/*
    Returns a descriptor vector which describes the average colour of the passed image.
*/
const std::vector<int> RGBGridPatchDatabase::averageColour(const cv::Mat image) const {
    int redAvg = 0;
    int greenAvg = 0;
    int blueAvg = 0;
    // Find the average colour for the image
    for (int x = 0; x < image.size().width; ++x)
    {
        for (int y = 0; y < image.size().height; ++y)
        {
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

/*
    Returns an image from the database that has a close comparison to the descriptor passed.
    Image is not always best fit, but will be close.
*/
int RGBGridPatchDatabase::imagePicker(std::vector<int> avgColour) const {
    int bestDiffIndex;
    int bestDiffValue = 1000; //initialise to some large number
    int diffValue;
    std::vector<int> diffValues;

    // compares the difference between the averages of both image tiles
    for (int i = 0; i < descriptors_.size(); ++i)
    {
        std::vector<int> bgrImage (descriptors_.at(i));
        diffValue = 0;

        for (int j = 0; j < avgColour.size(); ++j)
        {
            diffValue += abs(avgColour.at(j) - bgrImage.at(j));
        }

        diffValues.push_back(diffValue);

        if (bestDiffValue > diffValue)
        {
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