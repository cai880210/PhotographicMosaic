#include "RandomPatchDatabase.h"

RandomPatchDatabase::RandomPatchDatabase() : images_(), PatchDatabase() {

}

RandomPatchDatabase::~RandomPatchDatabase() {

}

void RandomPatchDatabase::add(const cv::Mat image) {
	images_.push_back(image);
}

const cv::Mat RandomPatchDatabase::find(const cv::Mat image) const {
	size_t ix = rand()%images_.size();
	return images_[ix];
}

size_t RandomPatchDatabase::size() const {
	return images_.size();
}

/*
    Returns and index to an image that matches well with the descriptors of passed image.
    Can return -1 if no match is found.
*/
int RandomPatchDatabase::imageMatch(const cv::Mat image) const {
    return -1;
}

/*
    Returns an image at a passed index.
*/
const cv::Mat RandomPatchDatabase::imageAtIndex(int index) const {
    return images_.at(index);
}