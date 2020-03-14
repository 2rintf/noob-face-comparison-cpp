#include "faceCompareWrap.h"

void face_compare_alg::initialize()
{
	deserialize("shape_predictor_68_face_landmarks.dat") >> sp;
	deserialize("dlib_face_recognition_resnet_model_v1.dat") >> net;
}

std::string face_compare_alg::compare_face_on_json(std::string face_img_path, std::string json_path)
{
	std::vector<face_info> read_faces;
	//todo: upload_img_encoding

	FileStorage fs1(json_path, FileStorage::READ);
	FileNode fn1 = fs1.root();
	FileNodeIterator iter = fn1.begin(), iter_end = fn1.end();



	return std::string();
}

std::vector<face_info> face_compare_alg::read_result_from_json(std::string result_path, std::string json_path)
{
	return std::vector<face_info>();
}
