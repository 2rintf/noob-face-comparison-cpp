#include "faceCompareWrap.h"

void face_compare_alg::initialize()
{
	cv::TickMeter tic;
	tic.start();
	deserialize("shape_predictor_68_face_landmarks.dat") >> face_sp;
	deserialize("dlib_face_recognition_resnet_model_v1.dat") >> face_net;
	tic.stop();

	cout << "Iniatialize success. It cost: "<<tic.getTimeSec() << "s" << endl;
}

void face_compare_alg::detectTestFunc(std::string img_path, std::vector<matrix<rgb_pixel>>& get_faces,
	cv::Mat& img_encoding)
{
	image_window win, win_faces, win_facechip;

	string filePath = img_path;

	matrix<rgb_pixel> img;
	load_image(img, filePath);
	//pyramid_up(img);

	win.clear_overlay();
	win.set_image(img);


	cv::TickMeter tic;
	tic.start();
	std::vector<dlib::rectangle> dets = face_detector(img);
	tic.stop();
	cout << "Detect cost: " << tic.getTimeSec() << "s" << endl;

	std::vector<full_object_detection> shapes;
	std::vector<matrix<rgb_pixel>> faces; // save the detected face that
										  // have been normalized to 150x150 and appropriately rotated
										  // and centered.

	matrix<rgb_pixel> face_chip;
	for (rectangle face : dets)
	{
		auto shape = face_sp(img, face);
		win.add_overlay(render_face_detections(shape));
		win_faces.set_image(img);
		win_faces.add_overlay(face);

		extract_image_chip(img, get_face_chip_details(shape, 150, 0.25), face_chip);
		win_facechip.set_image(face_chip);

		faces.push_back(move(face_chip));

	}
	cout << "Hit enter to process the next image..." << endl;
	cin.get();

	// 将faces拷贝到get_faces，此时faces为空。
	get_faces.swap(faces);
	//---------------------------------------------------------------------------

	cv::TickMeter tic2;
	tic2.start();
	// 128D vector of face detected.
	std::vector<matrix<float, 0, 1>> face_descriptors = face_net(faces);
	tic2.stop();
	cout << "Get 128D vector cost: " << tic2.getTimeSec() << "s" << endl;

	cout << face_descriptors.size() << endl;


	for (auto descriptor : face_descriptors)
	{
		//cout << descriptor << endl;

		// dlib::matrix to cv::Mat. 
		cv::Mat encoding(int(descriptor.nr()), int(descriptor.nc()), CV_32FC1, descriptor.begin());
		cout << encoding << endl;

		// 因为项目中上传图片只允许有一张脸，所以此处只会迭代一次，故直接取循环中的encoding即可。 
		img_encoding = encoding.clone();

		// cv::Mat to dlib::matrix.
		matrix<float, 0, 1> dlib_from_cv;
		cv_image<float> tmp = encoding;
		assign_image(dlib_from_cv, tmp);
		cout << dlib_from_cv << endl;
	}


	
}

std::string face_compare_alg::compare_face_on_json(std::string face_img_path, std::string json_path)
{
	std::vector<face_info> read_faces;
	matrix<rgb_pixel> upload_face_img;
	load_image(upload_face_img, face_img_path);
	matrix<float, 0, 1> upload_face_encoding;
	if (!getFaceEncoding(upload_face_img, upload_face_encoding))
		return "";

	//todo: C++的map是有序的，默认对Key值降序排列。所以利用这一点，将distance
	std::map<int, float> results;
	
	//todo: upload_img_encoding

	cv::FileStorage fs1(json_path, cv::FileStorage::READ);
	if (!fs1.isOpened())
	{
		cout << "Error: 数据库json打开失败！" << endl;
		return "";
	}
	cv::FileNode fn1 = fs1.root();
	int total = fn1.size();
	cout << "json文件中所包含的人脸数量: " << total << endl;
	cv::FileNodeIterator iter = fn1.begin(), iter_end = fn1.end();

	for (; iter != iter_end; ++iter)
	{
		face_info temp;
		*iter >> temp;
		read_faces.push_back(temp);
	}

	fs1.release();

	//! 开始比较
	std::string results_file_path = "results_test_file.json";
	cv::FileStorage fs2(results_file_path, cv::FileStorage::WRITE | cv::FileStorage::FORMAT_JSON);

	for (auto face_from_lib : read_faces)
	{
		matrix<float, 0, 1> temp_encoding = cvMat2dlibMatrix(face_from_lib.encoding);
		float distance = getDistanceOfTwoFace(upload_face_encoding, temp_encoding);
	
		results.insert(std::map<int, float>::value_type(face_from_lib.index, distance));
	}

	//! map转pair，再转vector，以便使用sort.
	std::vector<std::pair<int, float>> tmp;
	for (auto& i : results)
		tmp.push_back(i);

	//! 升序排列
	std::sort(tmp.begin(), tmp.end(),
		[=](std::pair<int, float>& a, std::pair<int, float>& b) { return a.second < b.second; });

	//! 写入结果文件
	for (auto it = tmp.begin(); it != tmp.end(); ++it)
	{
		fs2 << "result_" + std::to_string(it->first) << "{" << "distance" << it->second 
			<<"index"<< it->first
			<< "}";
	}
	fs2.release();

	return results_file_path;
}

//std::vector<face_info> face_compare_alg::read_result_from_json(std::string result_path, std::string json_path)
//{
//	return std::vector<face_info>();
//}

bool face_compare_alg::getFaceEncoding(matrix<rgb_pixel> face_img, matrix<float, 0, 1>& face_encoding)
{
	cv::TickMeter tic;
	tic.start();
	std::vector<dlib::rectangle> dets = face_detector(face_img);
	tic.stop();
	cout << "Detect cost: " << tic.getTimeSec() << "s" << endl;

	if (dets.size()==0)
	{
		cout << "Error: face_img has no faces detected!" << endl;
		return false;
	}

	//! 具体针对本项目，此处faces实际上始终只有一张face。
	//  用于存检测到的face。
	std::vector<matrix<rgb_pixel>> faces;
	matrix<rgb_pixel> face_chip;
	for (rectangle face : dets)
	{
		auto shape = face_sp(face_img, face);
		extract_image_chip(face_img, get_face_chip_details(shape, 150, 0.25), face_chip);

		faces.push_back(move(face_chip));

	}

	cv::TickMeter tic2;
	tic2.start();
	// 128D vector of face detected.
	std::vector<matrix<float, 0, 1>> face_descriptors = face_net(faces);
	tic2.stop();
	cout << "Get 128D vector cost: " << tic2.getTimeSec() << "s" << endl;

	cout << face_descriptors.size() << endl;

	//! 具体针对本项目，上传图片只允许有一张脸，所以此处只会迭代一次，故直接取descriptor即可。 
	for (auto descriptor : face_descriptors)
	{
		face_encoding.swap(descriptor);

		/**
		----------------------------------------------
		| 参考：										 |
		| dlib中的matrix和OpenCV的Mat之间的转换。       |
		----------------------------------------------
		// dlib::matrix to cv::Mat.
		cv::Mat encoding(int(descriptor.nr()), int(descriptor.nc()), CV_32FC1, descriptor.begin());
		cout << encoding << endl;

		// cv::Mat to dlib::matrix.
		matrix<float, 0, 1> dlib_from_cv;
		cv_image<float> tmp = encoding;
		assign_image(dlib_from_cv, tmp);
		cout << dlib_from_cv << endl;
		*/
		
	}
	return true;


}

float face_compare_alg::getDistanceOfTwoFace(matrix<float, 0, 1> encoding1, matrix<float, 0, 1> encoding2)
{
	float distance = length(encoding1 - encoding2);
	return distance;
}

matrix<float, 0, 1> face_compare_alg::cvMat2dlibMatrix(cv::Mat mat_cv)
{
	matrix<float, 0, 1> dlib_from_cv;
	cv_image<float> tmp = mat_cv;
	assign_image(dlib_from_cv, tmp);

	return dlib_from_cv;
}

void face_compare_alg::dlibMatrix2cvMat(matrix<float, 0, 1> mat_dlib, cv::Mat& mat_cv)
{
	cv::Mat encoding(int(mat_dlib.nr()), int(mat_dlib.nc()), CV_32FC1, mat_dlib.begin());
	mat_cv = encoding.clone();
}
