#include "PatchDatabase.h"

#include <vector>

class RGBGridPatchDatabase : public PatchDatabase {

public:
    RGBGridPatchDatabase();
    ~RGBGridPatchDatabase();

    void add(const cv::Mat image);
    const cv::Mat find(const cv::Mat image) const;
    size_t size() const;
    int imageMatch(const cv::Mat image) const;
    const cv::Mat imageAtIndex(int index) const;

private:

    const std::vector<int> describeImage(const cv::Mat image) const;
    const std::vector<int> averageColour(const cv::Mat image) const;
    int imagePicker(std::vector<int> avgColour) const;
    std::vector<const cv::Mat> images_;
    std::vector<const std::vector<int> > descriptors_;

};