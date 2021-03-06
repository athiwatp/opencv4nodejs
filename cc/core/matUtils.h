#include <nan.h>
#include <opencv2/core.hpp>

#ifndef FF_MATUTILS_H_
#define FF_MATUTILS_H_

#define FF_MAT_AT(mat, val, get)	\
	val = get(mat, info[0]->Int32Value(), info[1]->Int32Value());

#define FF_MAT_SET(mat, val, put)	\
	put(mat, val, info[0]->Int32Value(), info[1]->Int32Value());

#define FF_MAT_FILL(mat, vec, put)				\
	for (int r = 0; r < mat.rows; r++) {		\
		for (int c = 0; c < mat.cols; c++) {	\
			put(mat, vec, r, c);								\
		}																			\
	}

#define FF_MAT_FROM_JS_ARRAY(mat, rowArray, put)																	\
	for (int r = 0; r < mat.rows; r++) {																						\
		v8::Local<v8::Array> colArray = v8::Local<v8::Array>::Cast(rowArray->Get(r));	\
		for (int c = 0; c < mat.cols; c++) {																					\
			put(mat, colArray->Get(c), r, c);																						\
		}																																							\
	}

#define FF_JS_ARRAY_FROM_MAT(mat, rowArray, get)										\
	for (int r = 0; r < mat.rows; r++) {															\
		v8::Local<v8::Array> colArray = Nan::New<v8::Array>(mat.cols);	\
		for (int c = 0; c < mat.cols; c++) {														\
			colArray->Set(c, get(mat, r, c));															\
		}																																\
		rowArray->Set(r, colArray);																			\
	}

#define FF_MAT_APPLY_TYPED_OPERATOR(mat, arg, type, ITERATOR, OPERATOR) {	\
	switch (type) {																													\
	case CV_8UC1:																														\
		ITERATOR(mat, arg, OPERATOR##Val<uchar>)															\
			break;																															\
	case CV_8UC2:																														\
		ITERATOR(mat, arg, OPERATOR##Vec2<uchar>)															\
			break;																															\
	case CV_8UC3:																														\
		ITERATOR(mat, arg, OPERATOR##Vec3<uchar>)															\
			break;																															\
	case CV_8UC4:																														\
		ITERATOR(mat, arg, OPERATOR##Vec4<uchar>)															\
			break;																															\
	case CV_8SC1:																														\
		ITERATOR(mat, arg, OPERATOR##Val<char>)																\
			break;\
	case CV_8SC2:\
		ITERATOR(mat, arg, OPERATOR##Vec2<char>)\
			break;\
	case CV_8SC3:\
		ITERATOR(mat, arg, OPERATOR##Vec3<char>)\
			break;\
	case CV_8SC4:\
		ITERATOR(mat, arg, OPERATOR##Vec4<char>)\
			break;\
	case CV_16UC1:\
		ITERATOR(mat, arg, OPERATOR##Val<ushort>)\
			break;\
	case CV_16UC2:\
		ITERATOR(mat, arg, OPERATOR##Vec2<ushort>)\
			break;\
	case CV_16UC3:\
		ITERATOR(mat, arg, OPERATOR##Vec3<ushort>)\
			break;\
	case CV_16UC4:\
		ITERATOR(mat, arg, OPERATOR##Vec4<ushort>)\
			break;\
	case CV_16SC1:\
		ITERATOR(mat, arg, OPERATOR##Val<short>)\
			break;\
	case CV_16SC2:\
		ITERATOR(mat, arg, OPERATOR##Vec2<short>)\
			break;\
	case CV_16SC3:\
		ITERATOR(mat, arg, OPERATOR##Vec3<short>)\
			break;\
	case CV_16SC4:\
		ITERATOR(mat, arg, OPERATOR##Vec4<short>)\
			break;\
	case CV_32SC1:\
		ITERATOR(mat, arg, OPERATOR##Val<int>)\
			break;\
	case CV_32SC2:\
		ITERATOR(mat, arg, OPERATOR##Vec2<int>)\
			break;\
	case CV_32SC3:\
		ITERATOR(mat, arg, OPERATOR##Vec3<int>)\
			break;\
	case CV_32SC4:\
		ITERATOR(mat, arg, OPERATOR##Vec4<int>)\
			break;\
	case CV_32FC1: case CV_64FC1:\
		ITERATOR(mat, arg, OPERATOR##Val<double>)\
			break;\
	case CV_32FC2: case CV_64FC2:\
		ITERATOR(mat, arg, OPERATOR##Vec2<double>)\
			break;\
	case CV_32FC3: case CV_64FC3:\
		ITERATOR(mat, arg, OPERATOR##Vec3<double>)\
			break;\
	case CV_32FC4: case CV_64FC4:\
		ITERATOR(mat, arg, OPERATOR##Vec4<double>)\
			break;\
	default:\
		Nan::ThrowError(Nan::New("invalid matType: " + std::to_string(type)).ToLocalChecked());\
		break;\
	}\
}

#define FF_MAT_DILATE_OR_ERODE(func)	\
	func(matSelf, FF_UNWRAP_MAT_AND_GET(jsMatDst), FF_UNWRAP_MAT_AND_GET(jsKernel), anchor, iterations, borderType, borderValue);

#define FF_ASSERT_CHANNELS(cn, have, what)																						\
	if (cn != have) {																																		\
		return Nan::ThrowError(FF_V8STRING(std::string(what) + " - expected vector with "	\
			+ std::to_string(cn) + " channels, have " + std::to_string(have)));							\
	}


namespace FF {
	template<typename type>
	static inline void matPutVal(cv::Mat mat, v8::Local<v8::Value> value, int r, int c) {
		mat.at<type>(r, c) = (type)value->NumberValue();
	}

	template<typename type>
	static inline void matPutVec2(cv::Mat mat, v8::Local<v8::Value> vector, int r, int c) {
		v8::Local<v8::Array> vec = v8::Local<v8::Array>::Cast(vector);
		mat.at< cv::Vec<type, 2> >(r, c) = cv::Vec<type, 2>(
			(type)vec->Get(0)->NumberValue(),
			(type)vec->Get(1)->NumberValue()
		);
	}

	template<typename type>
	static inline void matPutVec3(cv::Mat mat, v8::Local<v8::Value> vector, int r, int c) {
		v8::Local<v8::Array> vec = v8::Local<v8::Array>::Cast(vector);
		mat.at< cv::Vec<type, 3> >(r, c) = cv::Vec<type, 3>(
			(type)vec->Get(0)->NumberValue(),
			(type)vec->Get(1)->NumberValue(),
			(type)vec->Get(2)->NumberValue()
		);
	}

	template<typename type>
	static inline void matPutVec4(cv::Mat mat, v8::Local<v8::Value> vector, int r, int c) {
		v8::Local<v8::Array> vec = v8::Local<v8::Array>::Cast(vector);
		mat.at< cv::Vec<type, 4> >(r, c) = cv::Vec<type, 4>(
			(type)vec->Get(0)->NumberValue(),
			(type)vec->Get(1)->NumberValue(),
			(type)vec->Get(2)->NumberValue(),
			(type)vec->Get(3)->NumberValue()
		);
	}

	template<typename type>
	static inline v8::Local<v8::Value> matGetVal(cv::Mat mat, int r, int c) {
		return Nan::New(mat.at<type>(r, c));
	}

	template<typename type>
	static inline v8::Local<v8::Value> matGetVec2(cv::Mat mat, int r, int c) {
		v8::Local<v8::Array> vec = Nan::New<v8::Array>(2);
		vec->Set(0, Nan::New(mat.at< cv::Vec<type, 2> >(r, c)[0]));
		vec->Set(1, Nan::New(mat.at< cv::Vec<type, 2> >(r, c)[1]));
		return vec;
	}

	template<typename type>
	static inline v8::Local<v8::Value> matGetVec3(cv::Mat mat, int r, int c) {
		v8::Local<v8::Array> vec = Nan::New<v8::Array>(3);
		vec->Set(0, Nan::New(mat.at< cv::Vec<type, 3> >(r, c)[0]));
		vec->Set(1, Nan::New(mat.at< cv::Vec<type, 3> >(r, c)[1]));
		vec->Set(2, Nan::New(mat.at< cv::Vec<type, 3> >(r, c)[2]));
		return vec;
	}

	template<typename type>
	static inline v8::Local<v8::Value> matGetVec4(cv::Mat mat, int r, int c) {
		v8::Local<v8::Array> vec = Nan::New<v8::Array>(4);
		vec->Set(0, Nan::New(mat.at< cv::Vec<type, 4> >(r, c)[0]));
		vec->Set(1, Nan::New(mat.at< cv::Vec<type, 4> >(r, c)[1]));
		vec->Set(2, Nan::New(mat.at< cv::Vec<type, 4> >(r, c)[2]));
		vec->Set(3, Nan::New(mat.at< cv::Vec<type, 4> >(r, c)[3]));
		return vec;
	}

	static int reassignMatTypeIfFloat(int type) {
		switch (type) {
		case CV_32FC1: return CV_64FC1;
		case CV_32FC2: return CV_64FC2;
		case CV_32FC3: return CV_64FC3;
		case CV_32FC4: return CV_64FC4;
		default: return type;
		}
	}
}

#endif