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
