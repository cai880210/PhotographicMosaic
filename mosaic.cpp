#include "RandomPatchDatabase.h"
#include "AveragePatchDatabase.h"
#include "RGBGridPatchDatabase.h"

#include <opencv/cv.h>
#include <opencv/highgui.h>

#include <fstream>
#include <iostream>
#include <string>

int main (int argc, char *argv[]) {

	// Read the command parameters
	if (argc != 6) {
		std::cerr << "ERROR: Expected 6 arguments:" << std::endl;
		std::cerr << "  mosaic <input image> <list of images> <tile size> <method> <output image>" << std::endl;
		return -1;
	}	
	std::string inputFileName = argv[1];
	std::string imageListFileName = argv[2];
	int tileSize = atoi(argv[3]);
	std::string method = argv[4];
	std::string outputFileName = argv[5];

	// The mosaic is based on the input image
	cv::Mat mosaic = cv::imread(inputFileName);
	if (!mosaic.data) {
		std::cerr << "ERROR: Could not read input image " << inputFileName << std::endl;
		exit(-1);
	}

	// We'll display the progress so we can see how it is going
	cv::namedWindow("Display");
	cv::imshow("Display", mosaic);
	cv::waitKey(5);


	// Create a database of images to use as mosaic patches
	PatchDatabase* patchDatabase;
	if (method == "random") {
		patchDatabase = new RandomPatchDatabase();
	} else if (method =="average") {
		patchDatabase = new AveragePatchDatabase();
	} else if (method =="RGBGrid") {
		patchDatabase = new RGBGridPatchDatabase();
	} else if (method =="adaptive") {
		patchDatabase = new RGBGridPatchDatabase();
	} else {
		std::cerr << "ERROR: Unrecognised method '" << method << "'" << std::endl;
		exit(-1);
	}

	// Fill the database from the list of files provided
	std::ifstream fin(imageListFileName);
	std::string patchImageFileName;
	while (std::getline(fin, patchImageFileName)) {
		cv::Mat patchImage = cv::imread(patchImageFileName);
		if (!patchImage.data) {
			std::cerr << "WARNING: Could not read from " << patchImageFileName << std::endl;
		} else {
			patchDatabase->add(patchImage);
		}
	}
	if (patchDatabase->size() == 0) {
		std::cerr << "ERROR: No images read into patch database" << std::endl;
		exit(-1);
	}

	if (method == "adaptive")
		/* TODO: Break this abomination down into easier to understand methods or a recursive method */
	{
		for (size_t x = 0; x <= mosaic.size().width-tileSize; x += tileSize) {
			for (size_t y = 0; y <= mosaic.size().height-tileSize; y += tileSize) {
				cv::Rect roi(x,y,tileSize,tileSize);
				
				int imageIndex = patchDatabase->imageMatch(mosaic(roi));
				if (imageIndex == -1) {
					// downsize to 16
					int xhalfConst = x + tileSize;
					int yhalfConst = y + tileSize;
					for (int xhalf = x; xhalf < xhalfConst; xhalf += tileSize/2) {
						for (int yhalf = y; yhalf < yhalfConst; yhalf += tileSize/2) {
							cv::Rect roi(xhalf,yhalf,tileSize/2,tileSize/2);
							imageIndex = patchDatabase->imageMatch(mosaic(roi));
							if (imageIndex == -1) {
								// downsize to 8
								int xqtrConst = xhalf + tileSize/2;
								int yqtrConst = yhalf + tileSize/2;
								for (int xqtr = xhalf; xqtr < xqtrConst; xqtr += tileSize/4) {
									for (int yqtr = yhalf; yqtr < yqtrConst; yqtr += tileSize/4) {
										std::cout << "--------------- QUATER size tiles" << std::endl;
										std::cout << "Xqtr:" << xqtr << "   Yqtr:" << yqtr << std::endl;
										cv::Rect roi(xqtr,yqtr,tileSize/4,tileSize/4);
										cv::Mat patchImage = patchDatabase->find(mosaic(roi));
										cv::resize(patchImage, mosaic(roi), roi.size());
										cv::imshow("Display", mosaic);
										cv::waitKey(5);

										imageIndex = -1; // reset!
									}
								}
							} else {
								std::cout << "--------------- HALF size tiles" << std::endl;
								std::cout << "Xhalf:" << xhalf << "   Yhalf:" << yhalf << std::endl;
								// turn into a draw image on canvas method
								cv::Mat patchImage = patchDatabase->imageAtIndex(imageIndex);
								cv::resize(patchImage, mosaic(roi), roi.size());
								cv::imshow("Display", mosaic);
								cv::waitKey(5);

								imageIndex = -1; // reset!
							}
						}
					}
				} else {
					std::cout << "--------------- FULL size tiles" << std::endl;
					std::cout << "X:" << x << "   Y:" << y << std::endl;
					// turn into a draw image on canvas method
					cv::Mat patchImage = patchDatabase->imageAtIndex(imageIndex);
					cv::resize(patchImage, mosaic(roi), roi.size());
					cv::imshow("Display", mosaic);
					cv::waitKey(5);

					imageIndex = -1; // reset!
				}
			}
		}

	} else{
		for (size_t x = 0; x <= mosaic.size().width-tileSize; x += tileSize) {
			for (size_t y = 0; y <= mosaic.size().height-tileSize; y += tileSize) {
				cv::Rect roi(x,y,tileSize,tileSize);
				cv::Mat patchImage = patchDatabase->find(mosaic(roi));
				cv::resize(patchImage, mosaic(roi), roi.size());
				cv::imshow("Display", mosaic);
				cv::waitKey(5);
			}
		}
	}

	
	cv::imwrite(outputFileName, mosaic);

	delete patchDatabase;

	return 0;
}