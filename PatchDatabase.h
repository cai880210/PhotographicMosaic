#ifndef PATCHDATABASE_H
#define PATCHDATABASE_H

#include <opencv/cv.h>

class PatchDatabase {

public:

	PatchDatabase() {}
	virtual ~PatchDatabase() {}

	virtual void add(const cv::Mat image) = 0;
	virtual const cv::Mat find(const cv::Mat image) const = 0;
	virtual size_t size() const = 0;
    virtual int imageMatch(const cv::Mat image) const;
    virtual const cv::Mat imageAtIndex(int index) const;

private:

	// Hiding copy constructor and assignment makes this non-copyable
	PatchDatabase(const PatchDatabase&);
	PatchDatabase& operator=(const PatchDatabase&);


};
#endif