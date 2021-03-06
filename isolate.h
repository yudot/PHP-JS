/**
 *  isolate.h
 *
 *  Simple getter for the (unique) v8 isolate. This
 *  method makes sure that a single isolate gets
 *  constructed and that the v8 engine is properly
 *  initialized once.
 *
 *  Note that this is explicitly not thread-safe,
 *  but it is fast. Since none of our other extensions
 *  are properly thread-safe, this is an acceptable
 *  limitation
 *
 *  @copyright 2015 Copernica B.V.
 */

/**
 *  Include guard
 */
#pragma once

/**
 *  Dependencies
 */
#include <v8-platform.h>
#include <chrono>
#include <v8.h>
#include <map>

/**
 *  Start namespace
 */
namespace JS {

/**
 *  Private class
 */
class Isolate final
{
private:
    /**
     *  The underlying isolate
     *  @var    v8::Isolate*
     */
    v8::Isolate *_isolate;

    /**
     *  List of tasks to execute
     *  @var    std::multimap<std::chrono::system_clock::time_point, std::unique_ptr<v8::Task>>
     */
    std::multimap<std::chrono::system_clock::time_point, std::unique_ptr<v8::Task>> _tasks;

    /**
     *  Perform all waiting tasks for this isolate
     */
    void runTasks();
public:
    /**
     *  Constructor
     */
    Isolate();

    /**
     *  Destructor
     */
    ~Isolate();

    /**
     *  Schedule a task to be executed
     *
     *  @param  isolate The isolate to schedule the task under
     *  @param  task    The task to execute
     *  @param  delay   Number of seconds to wait before executing
     */
    static void scheduleTask(v8::Isolate *isolate, v8::Task *task, double delay);

    /**
     *  Get the isolate for this thread
     *
     *  @return The thread-local isolate instance
     */
    static v8::Isolate *get();

    /**
     *  Clean up the isolate - if any - for this thread
     */
    static void destroy();

    /**
     *  Cast to the underlying isolate
     *
     *  @return v8::Isolate*
     */
    operator v8::Isolate* () const;
};

/**
 *  End namespace
 */
}
