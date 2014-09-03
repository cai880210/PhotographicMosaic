#include "AveragePatchDatabase.h"

#include <stdlib.h>     /* srand, rand */

AveragePatchDatabase::AveragePatchDatabase() : images_(), PatchDatabase() {

}

AveragePatchDatabase::~AveragePatchDatabase() {

}

void AveragePatchDatabase::add(const cv::Mat image) {
    images_.push_back(image);
    std::vector<int> avgColour(AveragePatchDatabase::averageColour(image));
    descriptors_.push_back(avgColour);
}

const cv::Mat AveragePatchDatabase::find(const cv::Mat image) const {
    // Get avg colour
    std::vector<int> avgColour(AveragePatchDatabase::averageColour(image));

    //// creates an image of the average colour to return
    //cv::Mat cellColour(cv::Size(120,120), CV_8UC3);     
    //cellColour.setTo(cv::Scalar(avgColour.at(0), avgColour.at(1), avgColour.at(2)));   

    // find an image that has a similar colour match
    int photoIndex = AveragePatchDatabase::imagePicker(avgColour);

    // return image to replace tile
    return images_.at(photoIndex);
}

size_t AveragePatchDatabase::size() const {
    return images_.size();
}

int AveragePatchDatabase::imagePicker(std::vector<int> avgColour) const{
    int bestDiffIndex;
    int bestDiffValue = 1000; //initialise to some large number
    int diffValue;
    std::vector<int> diffValues;

    // compares the difference between the averages of both image tiles
    for (int i = 0; i < descriptors_.size(); ++i)
    {
        std::vector<int> bgrImage (descriptors_.at(i));
        diffValue = 0;
        diffValue += abs(avgColour.at(0) - bgrImage.at(0));
        diffValue += abs(avgColour.at(1) - bgrImage.at(1));
        diffValue += abs(avgColour.at(2) - bgrImage.at(2));
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

    //std::cout << "Matching Diff Values with diff 5" << std::endl;
    //std::cout << topValues.size() << std::endl;

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



    //std::cout << "Matching Diff Values RECOUNT" << std::endl;
    //std::cout << topValues.size() << std::endl;

    //std::cout << "Print TopValues Vector" << std::endl;
    //for( std::vector<int>::const_iterator i = topValues.begin(); i != topValues.end(); ++i)
    //    std::cout << *i << ' ';
    //std::cout << std::endl;

    /* initialize random seed: */
    int randValue = rand()%topValues.size();

    // std::cout << "random index to use = " << randValue << std::endl;
    // std::cout << "random value =  randFuncMod " << topValues.size() << std::endl;
    // std::cout << std::endl;
    // std::cout << std::endl;

    return topValues.at(randValue);
}

const std::vector<int> AveragePatchDatabase::averageColour(const cv::Mat image) const{
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