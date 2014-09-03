#include "RGBGridPatchDatabase.h"

#include <stdlib.h> /* srand, rand */

RGBGridPatchDatabase::RGBGridPatchDatabase() : images_(), PatchDatabase() {

}

RGBGridPatchDatabase::~RGBGridPatchDatabase() {

}

void RGBGridPatchDatabase::add(const cv::Mat image) {
    images_.push_back(image);
    // split into sub cells for average colour 
    // return back image descriptor
    std::vector<int> imageDescriptor(RGBGridPatchDatabase::describeImage(image));
    descriptors_.push_back(imageDescriptor);
}

const cv::Mat RGBGridPatchDatabase::find(const cv::Mat image) const {
    // Get avg colour
    std::vector<int> imageDescriptor(describeImage(image));

    // find an image that has a similar colour match
    int photoIndex = RGBGridPatchDatabase::imagePicker(imageDescriptor);

    // return image to replace tile
    return images_.at(photoIndex);
}

size_t RGBGridPatchDatabase::size() const {
    return images_.size();
}

int RGBGridPatchDatabase::imageMatch(const cv::Mat image) const {
    // Get avg colour
    std::vector<int> imageDescriptor(describeImage(image));

    // find an image that has a similar colour
    int photoIndex = RGBGridPatchDatabase::imagePicker(imageDescriptor);

    int diffValue = 0;
    for (int i = 0; i < imageDescriptor.size(); ++i)
    {
        diffValue += abs(imageDescriptor.at(i) - descriptors_.at(photoIndex).at(i));
    }

    //std::cout << diffValue << std::endl;

    if (diffValue < 700)
    {
        return photoIndex;
    }else{
        return -1; // unsure if allowed
    }
}

const cv::Mat RGBGridPatchDatabase::imageAtIndex(int index) const {
    return images_.at(index);
}

const std::vector<int> RGBGridPatchDatabase::describeImage(const cv::Mat image) const {
    std::vector<int> imageDescriptor;
    int tileSize = image.size().width/4;

    for (int x = 0; x <= image.size().width-tileSize; x += tileSize) {
        for (int y = 0; y <= image.size().height-tileSize; y += tileSize) {
            cv::Rect roi(x,y,tileSize,tileSize);
            cv::Mat partialImage(image(roi));
            //std::cout << "before avgColour calc" << std::endl;
            std::vector<int> tempValues(averageColour(partialImage));
            //std::cout << "after avgColour calc" << std::endl;

            imageDescriptor.push_back(tempValues.at(0));
            imageDescriptor.push_back(tempValues.at(1));
            imageDescriptor.push_back(tempValues.at(2));
        }
    }

    return imageDescriptor;
}

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

    std::vector<int> topValues;

    for (int i = 0; i < diffValues.size(); ++i)
    {
        if(diffValues.at(i)>= bestDiffValue && diffValues.at(i)<= bestDiffValue + 2 )
        {
            topValues.push_back(i);
        }
    }

    // std::cout << "Matching Diff Values with diff 5" << std::endl;
    // std::cout << topValues.size() << std::endl;

    if (topValues.size() < 5)
    {
        int lambda = 10;
        while(topValues.size() < 5)
        {
            topValues.clear(); // to besure we have no duplicates
            for (int i = 0; i < diffValues.size(); ++i)
            {
                if(diffValues.at(i)>= bestDiffValue && diffValues.at(i)<= bestDiffValue + lambda )
                {
                    topValues.push_back(i);
                }
            }
            lambda += 10;
        }
    }



    // std::cout << "Matching Diff Values RECOUNT" << std::endl;
    // std::cout << topValues.size() << std::endl;

    // std::cout << "Print TopValues Vector" << std::endl;
    // for( std::vector<int>::const_iterator i = topValues.begin(); i != topValues.end(); ++i)
    //     std::cout << *i << ' ';
    // std::cout << std::endl;

    /* initialize random seed: */
    int randValue = rand()%topValues.size();

    // std::cout << "random index to use = " << randValue << std::endl;
    // std::cout << "random value =  randFuncMod " << topValues.size() << std::endl;
    // std::cout << std::endl;
    // std::cout << std::endl;

    return topValues.at(randValue);
}