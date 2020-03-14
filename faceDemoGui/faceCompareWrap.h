#pragma once
#include <opencv2/opencv.hpp>

#include <dlib/image_processing/frontal_face_detector.h>
#include <dlib/image_processing/render_face_detections.h>
#include <dlib/image_processing.h>
#include <dlib/clustering.h>
#include <dlib/dnn.h>
//#include <dlib/image_io.h>
#include <dlib/matrix.h>
#include <dlib/clustering.h>
#include <dlib/dnn.h>
#include <dlib/opencv.h>

using namespace std;
using namespace cv;
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

static frontal_face_detector detector = get_frontal_face_detector();
static shape_predictor sp;
//deserialize("shape_predictor_68_face_landmarks.dat") >> sp;

static anet_type net;
//deserialize("dlib_face_recognition_resnet_model_v1.dat") >> net;

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

	int last_index;


	void write(FileStorage& fs, int last_index) const //Write serialization for this class
	{
		//todo: 将人脸信息写入json.
		//! 未测试
		fs << to_string(last_index) << ":{"
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

	void read(const FileNode& node)
	{
		index = (int)node["index"];
		name = (string)node["name"];
		sex = (string)node["sex"];
		email = (string)node["email"];
		path = (string)node["path"];
		age = (int)node["age"];
		phone = (int)node["phone"];
		node["encoding"] >> encoding_vector;

		Mat temp = Mat(128, 1, CV_32FC1, encoding_vector.data());
		encoding = temp.clone();
	}
};

static void write(FileStorage& fs, const string&, const face_info& x)
{
	x.write(fs,x.last_index);
}


static void read(const FileNode& node, face_info& x, const face_info& default_value = face_info())
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
	static std::string compare_face_on_json(std::string face_img_path, std::string json_path);
	static std::vector<face_info> read_result_from_json(std::string result_path, std::string json_path);

	//todo: 想好参数！
	void getFaceEncoding();

};
