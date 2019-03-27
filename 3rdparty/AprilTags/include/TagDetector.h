#ifndef TAGDETECTOR_H
#define TAGDETECTOR_H

#include <vector>

#include "opencv2/opencv.hpp"

#include "TagDetection.h"
#include "TagFamily.h"
#include "FloatImage.h"

namespace AprilTags {

class TagDetector {
public:

	TagFamily tagFamily;

	//! Constructor
  // note: TagFamily is instantiated here from TagCodes
	TagDetector(const TagFamily& tagFam) : tagFamily(tagFam) {};


	std::vector<TagDetection> extractTags(const cv::Mat& image);

	//! Gaussian smoothing kernel applied to image (0 == no filter).
	/*! Used when sampling bits. Filtering is a good idea in cases
	* where A) a cheap camera is introducing artifical sharpening, B)
	* the bayer pattern is creating artifcats, C) the sensor is very
	* noisy and/or has hot/cold pixels. However, filtering makes it
	* harder to decode very small tags. Reasonable values are 0, or
	* [0.8, 1.5].
	*/
	float sigma = 0;

	//! Gaussian smoothing kernel applied to image (0 == no filter).
	/*! Used when detecting the outline of the box. It is almost always
	* useful to have some filtering, since the loss of small details
	* won't hurt. Recommended value = 0.8. The case where sigma ==
	* segsigma has been optimized to avoid a redundant filter
	* operation.
	*/
	float segSigma = 0.5f;

private:
};

} // namespace

#endif
