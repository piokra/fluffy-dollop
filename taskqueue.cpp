#include "taskqueue.h"

Task::Task()
{

}

Task::Task( const std::string& str, const StandardStream& stream )
{
	tag = str;
	arguments = stream;
}

TaskQueue::TaskQueue()
{
	//ctor
}

TaskQueue::~TaskQueue()
{
	//dtor
}
bool TaskQueue::getTask( Task& task )
{
	if( !m_tasks.size() ) return false;
	task = m_tasks.front();
	m_tasks.pop();
	return true;
}

bool TaskQueue::addTask( Task& task )
{
	m_tasks.push( task );
	return true;

}

