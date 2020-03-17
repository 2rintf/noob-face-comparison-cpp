#pragma once
#include <opencv2/opencv.hpp>

#include <dlib/image_processing/frontal_face_detector.h>
#include <dlib/image_processing/render_face_detections.h>
#include <dlib/image_processing.h>
#include <dlib/clustering.h>
#include <dlib/dnn.h>
#include <dlib/image_io.h>
#include <dlib/matrix.h>
#include <dlib/clustering.h>
#include <dlib/dnn.h>
#include <dlib/opencv.h>

// For debug.
#include <dlib/gui_widgets.h>

using namespace std;
//using namespace cv;
using namespace dlib;

template <template <int, template<typename>class, int, typename> class block, int N, template<typename>class BN, typename SUBNET>
using residual = add_prev1<block<N, BN, 1, tag1<SUBNET>>>;

template <template <int, template<typename>class, int, typename> class block, int N, template<typename>class BN, typename SUBNET>
using residual_down = add_prev2<avg_pool<2, 2, 2, 2, skip1<tag2<block<N, BN, 2, tag1<SUBNET>>>>>>;

template <int N, template <typename> class BN, int stride, typename SUBNET>
using block = BN<con<N, 3, 3, 1, 1, relu<BN<con<N, 3, 3, stride, stride, SUBNET>>>>>;

template <int N, typename SUBNET> using ares = relu<residual<block, N, affine, SUBNET>>;
template <int N, typename SUBNET> using ares_down = relu<residual_down<block, N, affine, SUBNET>>;

template <typename SUBNET> using alevel0 = ares_down<256, SUBNET>;
template <typename SUBNET> using alevel1 = ares<256, ares<256, ares_down<256, SUBNET>>>;
template <typename SUBNET> using alevel2 = ares<128, ares<128, ares_down<128, SUBNET>>>;
template <typename SUBNET> using alevel3 = ares<64, ares<64, ares<64, ares_down<64, SUBNET>>>>;
template <typename SUBNET> using alevel4 = ares<32, ares<32, ares<32, SUBNET>>>;

using anet_type = loss_metric<fc_no_bias<128, avg_pool_everything<
	alevel0<
	alevel1<
	alevel2<
	alevel3<
	alevel4<
	max_pool<3, 3, 2, 2, relu<affine<con<32, 7, 7, 2, 2,
	input_rgb_image_sized<150>
	>>>>>>>>>>>>;
// -------------------------------------------------------

static frontal_face_detector face_detector = get_frontal_face_detector();
static shape_predictor face_sp;
static anet_type face_net;

class face_info {

public:
	int index;
	std::string name;
	std::string path;
	std::string sex;
	int age;
	int phone;
	std::string email;
	std::vector<float> encoding_vector;
	cv::Mat encoding;
	
	face_info():index(0),age(0),phone(0),name(""),path(""),
		sex(""),email(""){}


	void write(cv::FileStorage& fs) const //Write serialization for this class
	{
		//! 将人脸信息写入json.
		fs << "{"
			<< "name" << name
			<< "sex" << sex
			<< "index" << index
			<< "path" << path
			<< "age" << age
			<< "phone" << phone
			<< "email" << email
			<< "encoding" << encoding
			<< "}";
	}

	void read(const cv::FileNode& node)
	{
		index = (int)node["index"];
		name = (string)node["name"];
		sex = (string)node["sex"];
		email = (string)node["email"];
		path = (string)node["path"];
		age = (int)node["age"];
		phone = (int)node["phone"];

		node["encoding"] >> encoding_vector;
		cv::Mat temp = cv::Mat(128, 1, CV_32FC1, encoding_vector.data());
		encoding = temp.clone();
	}
};

static void write(cv::FileStorage& fs, const string&, const face_info& x)
{
	x.write(fs);
}

static void read(const cv::FileNode& node, face_info& x, const face_info& default_value = face_info())
{
	if (node.empty())
		x = default_value;
	else
		x.read(node);
}




class face_compare_alg
{
public:
	face_compare_alg();
	~face_compare_alg();

	static void initialize();
	//static frontal_face_detector getFaceDetector() { return face_detector; }
	//static shape_predictor getFaceSP() { return face_sp; }
	//static anet_type getFaceNet() { return face_net; }
	static void detectTestFunc(std::string img_path, std::vector<matrix<rgb_pixel>>& get_faces, cv::Mat& img_encoding);

	//! 项目所需功能
	static std::string compare_face_on_json(std::string face_img_path, std::string json_path);
	static std::vector<face_info> read_result_from_json(std::string result_path, std::string json_path);
	static bool add_new_face_to_json(face_info new_face, std::string json_path);

	//----------------------------------------------------------------------
	//! 重复性操作打包使用
	static bool getFaceEncoding(matrix<rgb_pixel> face_img, matrix<float, 0, 1>& face_encoding);
	static float getDistanceOfTwoFace(matrix<float, 0, 1> encoding1, matrix<float, 0, 1> encoding2);
	static matrix < float, 0, 1> cvMat2dlibMatrix(cv::Mat mat_cv);
	static cv::Mat dlibMatrix2cvMat(matrix<float, 0, 1> mat_dlib);
};
