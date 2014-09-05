#include "PatchDatabase.h"

#include <vector>

class RandomPatchDatabase : public PatchDatabase {

public:

	RandomPatchDatabase();
	~RandomPatchDatabase();

	void add(const cv::Mat image);
	const cv::Mat find(const cv::Mat image) const;
	size_t size() const;
    int imageMatch(const cv::Mat image) const;
    const cv::Mat imageAtIndex(int index) const;

private:

	std::vector<const cv::Mat> images_;

};