#ifndef TASKQUEUE_H
#define TASKQUEUE_H

#include "datastream.h"
#include <string>
#include <queue>
struct Task
{
	Task();
	Task( const std::string& str, const StandardStream& stream );
	std::string tag;
	StandardStream arguments;
};


class TaskQueue
{
public:
	TaskQueue();
	virtual ~TaskQueue();

	bool getTask( Task& task );
	bool addTask( Task& task );


protected:

private:
	std::queue<Task> m_tasks;


};


#endif // TASKQUEUE_H
