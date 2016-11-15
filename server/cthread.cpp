#include "cthread.h"

void* CThread::threadFunc(void* param)
{
	CThread* pthread = (CThread*)param;
	pthread->run();

	pthread_exit(NULL);
}

void CThread::start()
{
	pthread_create(&tid, NULL, &threadFunc, (void*)this);
}