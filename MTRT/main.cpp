// MTRT.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#define new new( _CLIENT_BLOCK, __FILE__, __LINE__)

static _CrtMemState s1, s2, s3;

#include "ray_tracer.h"


int main()
{
	_CrtMemCheckpoint(&s1);
	RayTracer::DrawPicture();
	_CrtMemCheckpoint(&s2);
	if (_CrtMemDifference(&s3, &s1, &s2))
		_CrtMemDumpStatistics(&s3);

	_CrtDumpMemoryLeaks();
	return 0;
}