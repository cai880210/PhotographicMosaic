#include "PatchDatabase.h"

#include <vector>

class RandomPatchDatabase : public PatchDatabase {

public:

	RandomPatchDatabase();
	~RandomPatchDatabase();

	void add(const cv::Mat image);
	const cv::Mat find(const cv::Mat image) const;
	size_t size() const;

private:

	std::vector<const cv::Mat> images_;

};